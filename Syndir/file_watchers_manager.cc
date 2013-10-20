/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "file_watchers_manager.h"


FileWatchersManager::FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath) {

    logDebug() << "Starting recursive watch on dir:" << sourceDir << "with sync to remote:" << fullDestinationSSHPath;
    this->baseCWD = sourceDir;
    this->fullDestinationSSHPath = fullDestinationSSHPath; /* f.e: someuser@remotehost:/remote/path */

    loadSettings();
    QSettings settings;

    // signal(SIGPIPE, SIG_IGN); /* ignore SIGPIPE error */

    /* connect hooks to invokers */
    #ifndef GUI_ENABLED /* file changed emmiter isn't necessary in Synshot */
        /// consider moving file /dir watchers to qApp
        connect(this, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    #endif
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));

    scanDir(QDir(sourceDir)); /* will fill up manager 'files' field */
}


#ifdef GUI_ENABLED
    void FileWatchersManager::setConfigWindow(ConfigWindow *config) {
        if (config == NULL)
            this->configWindow = new ConfigWindow();
        else
            this->configWindow = config;

        /* tray icon change trigger */
        connect(this, SIGNAL(setWork(int)), this->configWindow, SLOT(doingWork(int)));

    }
#endif


void FileWatchersManager::loadSettings() {
    QSettings settings;

    if (settings.value("source_dir").isNull())
        settings.setValue("source_dir", DEFAULT_SOURCE_DIR);

    if (settings.value("destination_dir").isNull())
        settings.setValue("destination_dir", DEFAULT_DESTINATION_DIR);

    if (settings.value("remote_path").isNull())
        settings.setValue("remote_path", REMOTE_PATH);

    if (settings.value("sound_file").isNull())
        settings.setValue("sound_file", DEFAULT_SOUND_FILE);

    if (settings.value("ssh_port").isNull())
        settings.setValue("ssh_port", SSH_PORT);

    if (settings.value("ssh_password").isNull())
        settings.setValue("ssh_password", SSH_PASSWORD);

    if (settings.value("allowed_file_types").isNull())
        settings.setValue("allowed_file_types", ALLOWED_FILE_TYPES);

    /* set values of current watchers manager: */
    this->sshPort = settings.value("ssh_port", SSH_PORT).toInt();

    /* user might be implicit: */
    QStringList sshDirPartial = fullDestinationSSHPath.split(":");
    QString userWithHost = sshDirPartial.value(0);
    QStringList sshUserServerPartial = userWithHost.split("@"); /* first part is user@host */
    QRegExp emailSign("@");
    if (not userWithHost.contains(emailSign)) {
        this->userName = getenv("USER"); /* set username if not given explicitly */
        this->hostName = sshUserServerPartial.value(0);
        // logDebug() << "Implicit" << userName << hostName;
    } else { /* or explicit */
        // logDebug() << "Explicit";
        this->userName = sshUserServerPartial.value(0);
        this->hostName = sshUserServerPartial.value(1);
    }
    this->remotePath = sshDirPartial.value(1); /* last one is a remote path */

    if (this->userName.isEmpty()) {
        logDebug() << "No user name specified, trying to get it from ENV.";
        this->userName = getenv("USER");
    }

    if (this->hostName.isEmpty() or this->userName.isEmpty()) {
        logDebug() << "Invalid (empty) host or user name.";
        #ifndef GUI_ENABLED
            usage();
        #endif
        exit(1); // XXX: FIXME: shouldn't just silent fail, but retry
    }
}


void FileWatchersManager::connectToRemoteHost() {
    int result;
    // if (not connection or not connection->isConnected() or not connection->isAuthenticated()) {
        loadSettings();
        QSettings settings;
        QString sshPass = settings.value("ssh_password", SSH_PASSWORD).toString();

        logDebug() << "Creating new SSH connection to host on port:" << sshPort;
        if (connection != NULL) {
            disconnectSSHSession();
        }
        connection = ptssh_create();

        //Set the logging level
        connection->setLogLevel(LL_debug1);
        if (connection and
            connection->init(userName.toUtf8().constData(), hostName.toUtf8().constData(), sshPort) != PTSSH_SUCCESS ) {

            logDebug() << "Epic fail of SSH new.";
            usleep(DEFAULT_CONNECTION_TIMEOUT * 1000);
            logDebug() << "Retrying";
            connectToRemoteHost();
            return;
        }

        if (connection->isConnected()) {
            logDebug() << "Connected to:" << userName << "@" << hostName << ":" << sshPort;
        }

        result = connection->connectUp();
        if (result < 0) {
            logDebug() << "Connection failed. Retrying.";
            #ifdef GUI_ENABLED
                notify("Connection to server failed.");
                emit setWork(ERROR);
            #endif
            disconnectSSHSession();
            connectToRemoteHost();
            return;
        }

        bool passSupport = false;
        connection->isAuthSupported(PTsshAuth_Password, passSupport);
        logDebug() << "Password support on server side:" << passSupport;

        bool pubKeySupport = false;
        connection->isAuthSupported(PTsshAuth_PublicKey, pubKeySupport);
        logDebug() << "Public key support on server side:" << pubKeySupport;

        // // using rsa keys to perform auth
        // auto pubkey = QString(keysLocation + "/id_rsa.pub");
        // QFile fileA(pubkey);
        // if (!fileA.open(QIODevice::ReadOnly)) {
        //     logDebug() << "Failed to read SSH public key!";
        // }
        // QByteArray buffer = fileA.readAll();
        // fileA.close();

        // auto privkey = QString(keysLocation + "/id_rsa");
        // QFile fileB(privkey);
        // if (!fileB.open(QIODevice::ReadOnly)) {
        //     logDebug() << "Failed to read SSH private key!";
        // }
        // QByteArray bufferB = fileB.readAll(); //.replace("-----END RSA PRIVATE KEY-----", "").replace("-----BEGIN RSA PRIVATE KEY-----", "").trimmed();
        // fileB.close();

        // QByteArray d1 = buffer;
        // QByteArray d2 = bufferB;
        // const uint8 *a = (uint8*)(QString(d1).toStdString().c_str());
        // const uint8 *b = (uint8*)(QString(d2).toStdString().c_str());

        // logDebug() << "\nPUB:" << d1 << "\nPRIV:" << d2;
        // int result = connection->authByPublicKey(
        //     a, (uint32)strlen(buffer),
        //     b, (uint32)strlen(bufferB),
        //     NULL);


        /* XXX: FIXME: using plain passwords indicates LOW security: */
        if (sshPass == QString(SSH_PASSWORD)) {
            logDebug() << "Password auth was disabled! Detected password default value (after reset).";
            #ifdef GUI_ENABLED
                notify("Password wasn't set.");
                emit setWork(ERROR);
            #endif
            disconnectSSHSession();
            // if (connection)
            //     ptssh_destroy(&connection); /* destroys connection threads */
            usleep(DEFAULT_CONNECTION_TIMEOUT * 1000);
            connectToRemoteHost();
            return;
        }

        if (not passSupport and not connection->isAuthenticated()) {
            logDebug() << "Server disallows password use, but key auth failed. Will retry anyway";
            #ifdef GUI_ENABLED
                notify("Server disallows password use, but key auth failed.");
                emit setWork(ERROR);
            #endif
            disconnectSSHSession();
            usleep(DEFAULT_CONNECTION_TIMEOUT * 1000);
            connectToRemoteHost();
            return;
        }

        result = connection->authByPassword(sshPass.toUtf8());
        if (result != PTSSH_SUCCESS) {
            logDebug() << "Incorrect password!";
            #ifdef GUI_ENABLED
                notify("Incorrect password given");
                emit setWork(ERROR);
            #endif
            disconnectSSHSession();
            // if (connection)
            //     ptssh_destroy(&connection); /* destroys connection threads */
            usleep(DEFAULT_CONNECTION_TIMEOUT * 1000);
            connectToRemoteHost();
            return;
        } else {
            QString notf = "ScreenShot link copied to clipboard."; // as: " + userName + "@" + hostName;
            #ifdef GUI_ENABLED
                notify(notf);
                emit setWork(OK);
            #endif
            logDebug() << "Connection estabilished.";
        }

    // } else {
    //     logDebug() << "Still connected";
    // }
}


/* by tallica & dmilith */
void FileWatchersManager::scanDir(QDir dir) {
    logDebug() << "Scanning:" << dir.absolutePath();

    this->oldFiles = this->files;
    // this->files = QStringList();
    for (int index = 0; index < files.length(); index++) {
        // removePath(files.at(index));
        files.removeAt(index);
    }

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(dir, QDirIterator::Subdirectories);
    files << dir.absolutePath(); /* required for dir watches */

    while (it.hasNext()) {
        QString nextOne = it.next();
        if (QDir(nextOne).exists()) {
            // logDebug() << "DIRECTORY:" << nextOne;
            files << nextOne;
        } else {
            QRegExp matcher(settings.value("allowed_file_types", ALLOWED_FILE_TYPES).toString());
            if (nextOne.contains(matcher)) {
                // logDebug() << "Found match:" << nextOne;
                files << nextOne;
            } else {
                removePath(nextOne);
            }
        }
    }
    files.removeDuplicates();

    addPaths(files);
    logDebug() << "Total files and dirs on watch:" << files.size();
    logDebug() << "Ready.";
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    /* compare file name and modification date. Ignore doubles */
    if ((last != file) || (QFileInfo(file).created() != this->lastModified)) {
        this->lastModified = QFileInfo(file).created();
        this->last = file;
        logDebug() << "Invoked copy to remote host for file:" << file;
        copyFilesToRemoteHost(QStringList(file));
    }
}


QStringList FileWatchersManager::removeFromList(QStringList& list, const QStringList& toDelete) {
    QStringListIterator i(toDelete);
    while (i.hasNext()) {
        list.removeAll(i.next());
    }
    return list;
}


void FileWatchersManager::executeRemoteCommand(const QString& command) {
    if (connection) {
        logDebug() << "Executing remote command:" << command;
        uint32 channel = PTSSH_BAD_CHANNEL_NUMBER;
        int result = connection->createChannel_session(channel);
        if (result != PTSSH_SUCCESS) {
            logDebug() << "Creation of remote session failed for command:" << command;
            return;
        }
        result = connection->channelRequest_exec(channel, command.toUtf8());
        if (result != PTSSH_SUCCESS) {
            logDebug() << "Remote command failed:" << command;
        }
        ptssh_closeChannel(connection, channel);
    } else {
        logDebug() << "Failed to execute remote command:" << command;
    }
}


void FileWatchersManager::disconnectSSHSession() {
    if (connection != NULL) {
        int result = connection->disconnect();
        while (result != PTSSH_SUCCESS) {
            logDebug() << "Disconnecting…";
            sleep(1);
        }
        ptssh_destroy(&connection); /* destroys connection threads */
        // delete connection;
    }
}


bool FileWatchersManager::sendFileToRemote(PTssh* connection, const QString& file, const QString& destinationFile) {

    FILE *pFileHandle = NULL;
    pFileHandle = fopen(file.toUtf8(), "rb");
    if (not pFileHandle) {

        logDebug() << "Can't open local file:" << file << "Permissions problem? Cannot continue";
        return false;

    } else {
        /* create missing directories in remote path */
        auto finalPath = remotePath.toUtf8();
        QString fileDirName = QFileInfo(file).absolutePath();
        QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));

        executeRemoteCommand("/bin/mkdir -p " + finalPath);
        if (not preDirs.isEmpty()) { /* sub dirs in path */
            auto elems = preDirs.toString().split("/");
            Q_FOREACH(auto elem, elems) {
                finalPath += "/" + elem;
                if (not elem.isEmpty())
                    executeRemoteCommand("/bin/mkdir -p " + finalPath);
            }
        }

        /* Get the file info and use it to create remote copy of that file */
        uint32 cNum = -1, optimalSize = 0, totalBytesQueued = 0;

        struct stat fileInfo, nextFileInfo;
        stat(file.toUtf8(), &fileInfo);
        usleep(FILE_SYNC_TIMEOUT*2); // XXX: pause for a second (IO wait, cause we have no idea how big the file is)
        stat(file.toUtf8(), &nextFileInfo);

        if (fileInfo.st_size != nextFileInfo.st_size) {
            fclose(pFileHandle);
            logTrace() << "Waiting for file to be synced:" << file;
            return sendFileToRemote(connection, file, destinationFile);
        }

        int result = ptssh_scpSendInit(connection, cNum, optimalSize, destinationFile.toUtf8(), fileInfo.st_size);
        if (result == PTSSH_SUCCESS) {
            uint32
                fileSize = (uint32)fileInfo.st_size;

            /* If you want to be nice to the library, you can ask for the optimal data
             * size that you can send on a channel. If you then write to the channel
             * and keep your channel writes at this size, this will be the most efficient.
             * Otherwise if a packet is too big, the underlying library will split it, which
             * will incurr an overhead for allocating memory for a few smaller packets.
             * Sending larger or smaller packets doesn't hurt anything, but may not be as fast
             * and will likely use more CPU power. */
            ptssh_getOptimalDataSize(connection, cNum, optimalSize);
            char *pBuf = new char[optimalSize];
            if (pBuf) {
                bool bKeepGoing = true;
                int32 bytesRead = 1;

                logDebug() << "Queueing" << fileSize/1024 << "KiB (" << fileSize << "bytes) for sending";
                while ( bytesRead > 0) {
                    bytesRead = fread(pBuf, 1, optimalSize, pFileHandle);

                    /* Writing to a channel is normally EXTREMELY quick. Underneath
                     * it all, the pointer to the buffer is wrapped up in a SSH
                     * BinaryPacket and then queued for sending. If there isn't room
                     * in the queue, this function will then block until room is available
                     * or until an error occurs... like the remote end disconnects
                     * unexpectedly. The queue size can be increased if needed
                     * in PTsshConfig.h. Default is about 4MB. This works really well for
                     * gigabit networks while keeping memory usage down. */
                    if (bytesRead > 0) {
                        result = ptssh_channelWrite(connection, cNum, pBuf, bytesRead);
                        if ( result != PTSSH_SUCCESS) {
                            logDebug() << "Failed to write channel data. Error:" << result;
                            delete pBuf;
                            return false;
                        } else {
                            totalBytesQueued += bytesRead;
                        }
                    }
                }
                fclose(pFileHandle);
                logDebug() << "Done queueing" << fileSize/1024 << "KiB for sending";

                /* do cleanup */
                result = ptssh_scpSendFinish(connection, cNum);
                if ( result == PTSSH_SUCCESS) {
                    logDebug() << "File synchronized successfully:" << file << "to remote:" << destinationFile;
                    delete pBuf;
                    return true;
                } else {
                    logDebug() << "File synchronization FAILURE!" << file << "to" << fullDestinationSSHPath << "Error:" << result;
                    delete pBuf;
                    return false;
                }
            }
        }
    }
    return false;
}


void FileWatchersManager::copyFilesToRemoteHost(const QStringList& fileList, bool hashFile) {

    int result;
    QTime myTimer;
    myTimer.start();

    connectToRemoteHost();

    Q_FOREACH(QString file, fileList) {

        QString fileDirName = QFileInfo(file).absolutePath();
        QStringRef prePath(&file, baseCWD.size(), (file.size() - baseCWD.size()));
        // QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));
        QString chopFileName = prePath.toUtf8();
        QString fullDestPath = remotePath + chopFileName; /* standard file name without rename */
        /* escape possible spaces in file name */
        fullDestPath = fullDestPath.replace(" ", "\\ ");

        #ifdef GUI_ENABLED
            logDebug() << "GUI enabled.";
            auto fileInfo_q = QFileInfo(file);
            auto dir = fileInfo_q.absolutePath();
            auto extension = fileInfo_q.suffix();

            auto hash = new QCryptographicHash(QCryptographicHash::Sha1);
            hash->addData(file.toUtf8(), file.length() + 1);
            auto resultSHA1 = hash->result().toHex();
            delete hash;

            auto renamedFile = dir + "/" + resultSHA1 + "." + extension;
            auto clipboard = QApplication::clipboard();
            QSettings settings;
            clipboard->setText(settings.value("remote_path", REMOTE_PATH).toString() + resultSHA1 + "." + extension);

            if (hashFile) { /* if using sha1 replacement in name */
                fullDestPath = remotePath + "/" + resultSHA1 + "." + extension;
            }
        #endif

        /* deal with deletion of local file with remote sync */
        if (not QFile::exists(file)) {

            logDebug() << "Deletion detected:" << file;
            logDebug() << "Synced deletion of remote file:" << fullDestPath;
            executeRemoteCommand("/bin/rm -f " + fullDestPath.toUtf8());
            removePath(file);
            files = removeFromList(files, QStringList(file));
            logDebug() << "Total files and dirs on watch:" << files.size();

        } else {

            logDebug() << endl << "Detected modification of:" << file << "Syncing to:" << hostName;

            while (not sendFileToRemote(connection, file, fullDestPath)); /* send file until success, never give up */

            if (hashFile) {
                #ifdef GUI_ENABLED
                    QSettings settings;
                    // NOTE: WTF?! this is causing SERIOUS memory leaks when using JackD sound system.
                    // QSound::play(settings.value("sound_file", DEFAULT_SOUND_FILE).toString());
                    notify("File transfer finished.");
                    emit setWork(OK);
                #endif
            }
        }
    }
    logDebug() << "Done. Time elapsed:" << myTimer.elapsed() << "miliseconds";
    logDebug() << "Total files and dirs on watch:" << files.size();

    disconnectSSHSession();

    logDebug() << "Disconnected SSH connection (TEMPORARY TO SAVE CPU TIME)";
    // QTimer::singleShot(ICON_BACK_TO_IDLE_TIMEOUT, this, SLOT(connectToRemoteHost()));
}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    if (not QDir(dir).exists()) {
        logDebug() << "Dir has gone. Assuming directory deletion of:" << dir;
        removePath(dir);
        scanDir(QDir(dir + "/.."));
    } else {
        logDebug() << "Dir changed:" << dir;
        scanDir(QDir(dir)); /* don't scan non existent directories */

        /* show number of differences */
        auto diffs = abs(files.length() - oldFiles.length());
        logDebug() << "Found" << diffs << "change(s)";

        if (diffs > 0) {
            /* scan for new files by file list diff */
            QStringList gatheredList = QStringList();
            Q_FOREACH(QString nextOne, files) {
                if (QFile::exists(nextOne)) {
                    if (not QDir(nextOne).exists()) {
                        // logDebug() << "Traversing next file:" << nextOne;
                        if (not oldFiles.contains(nextOne)) {
                            logDebug() << "New file?:" << nextOne;
                            gatheredList << nextOne;
                        }
                    }
                } else {
                    logDebug() << "File was REMOVED:" << nextOne;
                    // usleep(SOME_ADDITIONAL_PAUSE); /* XXX: HACK: this way we avoid thread race without additional sync */
                    gatheredList << nextOne;
                }
            }

            #ifdef GUI_ENABLED
                emit setWork(WORKING);
                copyFilesToRemoteHost(gatheredList, true);
            #else
                copyFilesToRemoteHost(gatheredList);
            #endif
        }

    }

}
