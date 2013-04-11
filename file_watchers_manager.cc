/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "file_watchers_manager.h"


FileWatchersManager::FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath) {
    qDebug() << "Starting recursive watch on dir:" << sourceDir << "with sync to remote:" << fullDestinationSSHPath;
    this->baseCWD = sourceDir;
    this->fullDestinationSSHPath = fullDestinationSSHPath; /* f.e: someuser@remotehost:/remote/path */

    /* user might be implicit: */
    QStringList sshDirPartial = fullDestinationSSHPath.split(":");
    QString userWithHost = sshDirPartial.value(0);
    QStringList sshUserServerPartial = userWithHost.split("@"); /* first part is user@host */
    QRegExp emailSign("@");
    if (not userWithHost.contains(emailSign)) {
        this->userName = getenv("USER"); /* set username if not given explicitly */
        this->hostName = sshUserServerPartial.value(0);
        // qDebug() << "Implicit" << userName << hostName;
    } else { /* or explicit */
        // qDebug() << "Explicit";
        this->userName = sshUserServerPartial.value(0);
        this->hostName = sshUserServerPartial.value(1);
    }
    this->remotePath = sshDirPartial.value(1); /* last one is a remote path */

    if (this->userName.isEmpty()) {
        qDebug() << "No user name specified, trying to get it from ENV.";
        this->userName = getenv("USER");
    }

    if (this->hostName.isEmpty() or this->userName.isEmpty()) {
        qDebug() << "Invalid (empty) host or user name.";
        #ifndef GUI_ENABLED
            usage();
        #endif
        exit(1); // XXX: FIXME: shouldn't just silent fail, but retry
    }

    signal(SIGPIPE, SIG_IGN); /* ignore SIGPIPE error */

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


void FileWatchersManager::connectToRemoteHost() {
    int result;
    if (connection == NULL) {
        QSettings settings;

        qDebug() << "Creating new SSH connection";
        connection = ptssh_create(); //new PTssh();

        // qDebug() << "Parsing SSH config values";
        uint8 sshPort = settings.value("ssh_port", SSH_PORT).toUInt();
        if (connection and
            connection->init(userName.toUtf8().constData(), hostName.toUtf8().constData(), sshPort) != PTSSH_SUCCESS ) {
            // ptssh_destroy(&connection);
            qDebug() << "Epic fail of SSH new";
            return;
        }

        //Set the logging level
        ptssh_setLogLevel(connection, LL_debug4);

        // using rsa keys to perform auth
        auto pubkey = QString(keysLocation + "/id_rsa.pub");
        QFile fileA(pubkey);
        if (!fileA.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to read SSH public key!";
        }
        QByteArray buffer = fileA.readAll().toBase64();
        fileA.close();

        auto privkey = QString(keysLocation + "/id_rsa");
        QFile fileB(privkey);
        if (!fileB.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to read SSH private key!";
        }
        QByteArray bufferB = fileB.readAll().replace("-----END RSA PRIVATE KEY-----", "").replace("-----BEGIN RSA PRIVATE KEY-----", "").trimmed().toBase64();
        fileB.close();

        QByteArray d1 = buffer;
        QByteArray d2 = bufferB;
        const uint8 *a = (uint8*)(QString(d1).toStdString().c_str());
        const uint8 *b = (uint8*)(QString(d2).toStdString().c_str());

        qDebug() << "\nPUB:" << d1 << "\nPRIV (only length):" << d2.length();
        // int result = ptssh_authByPublicKey(
        //     connection,
        //     QString(buffer).toUtf8(), strlen(QString(buffer).toUtf8()),
        //     QString(bufferB).toUtf8(), strlen(QString(bufferB).toUtf8()),
        //     NULL); /* no passphrase */


        // result = ptssh_connect(connection);
        // if (result < 0) {
        //     qDebug() << "Failed to ssdkoif onn\n";
        //     return;
        // }

        qDebug() << "Conecting to:" << connection->getUsername() << "@" << connection->getRemoteHostAddress() << ":" << connection->getRemoteHostPort();

        result = connection->connectUp();
        if (result < 0) {
            qDebug() << "Connection failed. Aborting.";
            return;
        }

        if (connection->isAuthenticated()) {
            qDebug() << "Authenticated to server.";
        }
        if (connection->isConnected()) {
            qDebug() << "Connected to server.";
        }


        // result = ptssh_authByPassword(connection, (const char * )"q", (const char *)"q");
        // while (result != PTSSH_SUCCESS) {
            // result = ptssh_authByPassword(connection, "q"); // connection->authByPassword("q");
            // result = connection->authByPassword("q");
            // result = connection->authByPublicKey(
                // a, strlen(QString(buffer).toUtf8()) * 2,
                // b, strlen(QString(bufferB).toUtf8()) * 2);

            // qDebug() << "Authentication failed\n:" << result;
            // usleep(1000000);
            // return;
        // }


        //     connection->disconnect();
        //     delete connection;
        //     delete sftp;

        //     return connectToRemoteHost();

        // result = ptssh_connect(connection);
        // if ( result < 0) {
        //     printf("Failed to connect 2\n" );
        //     return connectToRemoteHost();;
        // }

        // std::string privateKey(data);
        // const uint8 *pPublicKeyBlob64 = QString("").toAscii().constData();







        // if (connection != NULL)
        //     connection->deleteLater();
        // connection = new LibsshQtClient(this);  //ssh_new();
        // connection.auth_state = SSH_AUTH_STATE_NONE;
        // if (!connection) {
        //     exit(-1);
        // }

        /* syndir/synsheot config has higher priority over ~/.ssh/config values */


        // connection->setDebug(false);
        // connection->setPort(sshPort);
        // connection->setUsername(userName);
        // connection->setHostname(hostName);
        // connection->setVerbosity(LibsshQtClient::LogFunction);



        /* parse ~/.ssh/config */
        // ssh_options_parse_config(connection->sshSession(), (QString(getenv("HOME")) + "/.ssh/config").toUtf8());

        // qDebug() << "Setting SSH options. User:" << userName << "Host:" << hostName << "Port:" << sshPort;
        // ssh_string publicKey =
        //     publickey_from_file(connection, (QString(getenv("HOME")) + "/.ssh/id_rsa.pub").toUtf8(), NULL);

        // ssh_private_key privateKey = privatekey_from_file(connection, (QString(getenv("HOME")) + "/.ssh/id_rsa").toUtf8(), SSH_KEYTYPE_RSA, NULL);

        // ssh_userauth_none(connection, userName.toUtf8());
        // auto auth = ssh_userauth_pubkey(connection, userName.toUtf8(), publicKey, privateKey);

        //(connection, NULL); // NOTE: NULL is a passphrase of key here
        //ssh_userauth_password(connection, getenv("USER"), "q");
        //
        // if (auth == SSH_AUTH_SUCCESS) {
        //     qDebug() << "Connected.";
        //     // return;
        // } else if (auth == SSH_AUTH_DENIED) {
        //     fprintf(stderr, "Authentication failed\n");
        // } else {
        //     fprintf(stderr, "Error while authenticating : %s\n", ssh_get_error(connection));
        // }

        // qDebug() << "Connecting to SSH server";
        // connection->connectToHost(hostName, sshPort);
        // ssh_connect(connection);

        // if (connection != NULL)
        //     delete connection;
        // connection = new Connection(hostName.toStdString(), settings.value("ssh_port", SSH_PORT).toInt(), true);
        // connection->setKeyPath(keysLocation.toStdString());
        // connection->setCredentials(userName.toStdString(), "");
        // connection->mkConnection();



        QString notf = "Connected as: " + userName + "@" + hostName;
        // qDebug() << notf;
        #ifdef GUI_ENABLED
            notify(notf);
        #endif

        // if (connection == NULL) {
        //     qDebug() << "Something went wrong. No connection";
        //     // connection = NULL;
        //     // sftp = NULL;
        //     return connectToRemoteHost();
        // }










        // sftp = (struct sftp_session_struct*)malloc(sizeof(struct sftp_session_struct));
        // sftp = sftp_new(connection->sshSession());
        // sftp_init(sftp);

        // qDebug() << "SFTP server version:" << sftp->server_version;
        // qDebug() << "SFTP channel:" << sftp->channel;
        // qDebug() << "SFTP version:" << sftp->version;
        // if (ssh_is_connected(connection) == 1) {
        //     qDebug() << "Connection state OK";
        // }
        // } catch (Exception& e) {
        //     QString notf = "Error connecting to remote host: " + QString(e.what()) + ". Will retry!";
        //     qDebug() << notf;
        //     #ifdef GUI_ENABLED
        //         notify(notf);
        //     #endif
        //     sleep(1);
        //     return connectToRemoteHost();
        // }
        // return;
    }
}


/* by tallica & dmilith */
void FileWatchersManager::scanDir(QDir dir) {
    qDebug() << "Scanning:" << dir.absolutePath();

    this->oldFiles = this->files;
    for (int index = 0; index < files.size(); index++) {
    //     removePath(files.at(index));
        files.removeAt(index);
    }

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(dir, QDirIterator::Subdirectories);
    files << dir.absolutePath(); /* required for dir watches */

    while (it.hasNext()) {
        QString nextOne = it.next();
        if (QDir(nextOne).exists()) {
            // qDebug() << "DIRECTORY:" << nextOne;
            files << nextOne;
        } else {
            QRegExp matcher(settings.value("allowed_file_types", ALLOWED_FILE_TYPES).toString());
            if (nextOne.contains(matcher)) {
                // qDebug() << "Found match:" << nextOne;
                files << nextOne;
            } else {
                removePath(nextOne);
            }
        }
    }
    files.removeDuplicates();

    addPaths(files);
    qDebug() << "Total files and dirs on watch:" << files.size();
    qDebug() << "Ready.";
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    /* compare file name and modification date. Ignore doubles */
    if ((last != file) || (QFileInfo(file).created() != this->lastModified)) {
        this->lastModified = QFileInfo(file).created();
        this->last = file;
        qDebug() << "Invoked copy to remote host for file:" << file;
        copyFileToRemoteHost(file);
    }
}


QStringList FileWatchersManager::removeFromList(QStringList& list, const QStringList& toDelete) {
    QStringListIterator i(toDelete);
    while (i.hasNext()) {
        list.removeAll(i.next());
    }
    return list;
}


void FileWatchersManager::copyFileToRemoteHost(const QString& sourceFile, bool hashFile) {
    int result;
    QTime myTimer;
    myTimer.start();

    QString file = sourceFile;
    #ifdef GUI_ENABLED
        auto fileInfo = QFileInfo(file);
        auto dir = fileInfo.absolutePath();
        auto extension = fileInfo.suffix();

        auto hash = new QCryptographicHash(QCryptographicHash::Sha1);
        hash->addData(file.toUtf8(), file.length() + 1);
        auto resultSHA1 = hash->result().toHex();
        delete hash;

        auto renamedFile = dir + "/" + resultSHA1 + "." + extension;
        auto clipboard = QApplication::clipboard();
        clipboard->setText(settings.value("remote_path", REMOTE_PATH).toString() + resultSHA1 + "." + extension);
    #endif

    QString fileDirName = QFileInfo(file).absolutePath();
    QStringRef prePath(&file, baseCWD.size(), (file.size() - baseCWD.size()));
    QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));
    QString chopFileName = prePath.toUtf8();
    QString fullDestPath = remotePath + chopFileName;

    /* creating dir recursively for a single destination file */
    auto finalPath = remotePath.toUtf8();
    while (!connection or !connection->isConnected()) { // or !connection->isAuthenticated()
        connectToRemoteHost();
        usleep(5000000);
        cout << ".";
    }
    qDebug() << "Connection estabilished.";


    /* initialize SFTP subsystem */
    if (sftp != NULL) {
        qDebug() << "Cleaning up old SFTP connections";
        delete sftp;
        sftp = NULL;
    }
    // result = ptssh_initSftp(connection); //connection->getSftpObj();
    sftp = ptssh_getSftpObj(connection);
    // if (result != PTSSH_SUCCESS) {
    //     qDebug() << "Error init'ing sftp subsystem. Error" << result;
    //     return;
    // }
    // sftp->init();
     // {

    if (sftp) {
        qDebug() << "SFTP object created!";
        qDebug() << "SFTP version:" << sftp->getSftpVersion();
        // SftpAttrs attrs;
        // SftpDirHandle *pSftpDirHandle = NULL;
        // SftpDirHandle *pSDH = NULL;

        /* deal with directories first */
        SFTP_W_ATTR attrs;

        // mallocatrs, 0x0, sizeof(SftpAttrs*));
        // atrs->permissions(0755);
        // sftp->makeDir(finalPath, );
        memset(&attrs, 0x0, sizeof(SFTP_W_ATTR));
        attrs.permissions =
            FP_USR_RWX |
            FP_GRP_RWX |
            FP_OTH_RWX;

            //Sets permissions to 0777 (Unix style)
            // atrs->permissions =
            //     FP_USR_RWX |
            //     FP_GRP_RWX |
            //     FP_OTH_RWX;  //Sets permissions to 0777 (Unix style)

            qDebug() << "Trying to make dir:" << finalPath;
            // ptssh_openDir(sftp, &pSDH, finalPath);
            //
            // result = sftp->makeDir(finalPath, atrs);
           // / delete atrs;

            result = ptssh_makeDir(sftp, finalPath, &attrs);
            if (result != PTSSH_SUCCESS) {
                qDebug() << "Mkdir failed!:" << finalPath;
                // printf("Error making directory %s. Error %d\n", pRemoteDirToCreate, result);
                // return;
            }
            if (not preDirs.isEmpty()) { /* sub dirs in path */
                auto elems = preDirs.toString().split("/");
                for (int i = 0; i < elems.length(); i++) {
                    auto elem = elems.at(i);
                    finalPath += "/" + elem;
                    if (not elem.isEmpty()) {
                        // ssh_scp_push_directory(scp, finalPath, 0755);
                        // sftp_mkdir(sftp, finalPath, 0755);
                        result = ptssh_makeDir(sftp, finalPath, &attrs);
                        if (result != PTSSH_SUCCESS) {
                            qDebug() << "Mkdir inner fail!:" << finalPath;
                            // printf("Error making directory %s. Error %d\n", pRemoteDirToCreate, result);
                            // return;
                        }
                    }
                }
            }


            // ssh_get_disconnect_message

            /* create session here */

            qDebug() << endl << "Detected modification of:" << file << "Syncing to:" << hostName;
            FILE *fileHandler = NULL;
            SftpFileHandle *fileWriteHandler = NULL;
            uint8 *pTmpBuf = NULL; //4KB temporary buffer
            pTmpBuf = new uint8[TEMP_BUFFER_SIZE];
            if (!pTmpBuf) {
                qDebug() << "tmpbuf fail";
                return;
            }
            memset(pTmpBuf, 0x0, TEMP_BUFFER_SIZE);


            struct stat fileInfo;

            fileHandler = fopen(file.toUtf8(), "rb");
            if (fileHandler) {
                uint32 bytesRead = 0;
                uint64 totalBytesRead = 0;
                stat(file.toUtf8(), &fileInfo);
                #ifdef SHOW_STATISTICS
                    start = clock();
                #endif

                result = ptssh_openFile(sftp, &fileWriteHandler, fullDestPath.toUtf8().constData(), FO_CREATE | FO_RDWR);
                if (result != PTSSH_SUCCESS) {
                    qDebug() << "Karubito dest:" << fullDestPath << ", source:" << file;
                    // printf("Error creating file %s. Error %d\n", pRemoteFileToWrite, result);
                    // return;
                }

                // printf("Using temp bufer size 0x%X (%d)\n", TEMP_BUFFER_SIZE, TEMP_BUFFER_SIZE);
                while (totalBytesRead < (uint64)fileInfo.st_size) {
                    bytesRead = fread(pTmpBuf, 1, TEMP_BUFFER_SIZE, fileHandler);
                    if ( bytesRead > 0) {
                        int result = ptssh_writeFile(fileWriteHandler, pTmpBuf, bytesRead);
                        if (result != PTSSH_SUCCESS) {
                            qDebug() << "Fejld write of:" << file << "to" << fullDestPath.toUtf8().constData() << "result:" << result << "BytesRead:" << bytesRead;

                            // printf("Error writing to file %s. Error %d\n", fullDestPath.toUtf8(), result);
                            return;
                        }

                        totalBytesRead += bytesRead;
                    }
                }
            }
            fclose(fileHandler);

            result = sftp->closeFile(&fileWriteHandler); // ptssh_closeFile(sftp, &fileWriteHandler);
            if ( result != PTSSH_SUCCESS) {
                qDebug() << "Kraczor";
                // printf("Error closing file %s. Error %d\n", fullDestPath.toUtf8(), result);
                // return;
            }

            if (not QFile::exists(file)) {
                qDebug() << "Deletion detected:" << file;
                qDebug() << "Synced deletion of remote file:" << fullDestPath;
                // XXX: FIXME:
                // sftp_unlink(sftp, fullDestPath.toUtf8());
                sftp->deleteFile(fullDestPath.toUtf8().constData());// ptssh_deleteFile(sftp, fullDestPath.toUtf8().constData());
                removePath(file);
                files = removeFromList(files, QStringList(file));
                // sftp_free(sftp);
                // if (scp != NULL) {
                //     ssh_scp_close(scp);
                //     ssh_scp_free(scp);
                // }
                qDebug() << "Total files and dirs on watch:" << files.size();
                return;
                }
        }

        // if (result != PTSSH_SUCCESS) {
        //     qDebug() << "Failed to init SFTP stuff. Error " << result;
        //     return;
        // }
    // }



    // delete pTmpBuf;
    // qDebug() << "Shutting down sftp session.";
    // ptssh_shutdownSftp(connection);
    // sftp = NULL;







    // while (connection == NULL) {
    //     if (!connection)
    //         connectToRemoteHost();
    //     cout << "." << fflush(stdout);
    //     usleep(1000);
    // }
    //
    // sftp = sftp_new(connection->sshSession());

    // sftp->ext = sftp_ext_new();
    // int rc = sftp_init(sftp);

    // while (sftp == NULL) {
    //     if (!connection or !connection->sshSession()) {
    //         // sftp_free(sftp);
    //         connectToRemoteHost();
    //     }
    //     sftp = sftp_new(connection->sshSession());
    //     usleep(100000); /* 100ms is enough */
    //     cout << "…";
    //     fflush(stdout);

    //     // cout << " \r";
    // }

    // int rc = sftp_init(sftp);
    // while (rc < 0) {
    //     fprintf(stderr, "csync_sftp - error initialising sftp: %s\n", ssh_get_error(connection->sshSession()));
    //     rc = sftp_init(sftp);
    //     // sftp_free(sftp);
    //     // return;
    // }

        // cout << "-";
        // fprintf(stderr, "Error allocating SFTP session: %s\n", ssh_get_error(connection));
        // return SSH_ERROR;
        // sftp_free(sftp);
        // return;
    // }
    // auto error = sftp_get_error(sftp);
    // qDebug() << "ERROR:" << error;

    // LIBSSH2_SFTP *sftp_session = NULL;

    //     /* connect if not connected */
    //     sftp_session = libssh2_sftp_init(connection->session);
    //     cout << ".";
    //     fflush(stdout);
    //     usleep(100);
    // }

    // libssh2_session_set_timeout(connection->session, DEFAULT_SESSION_TIMEOUT);
    // libssh2_session_set_blocking(connection->session, 1); /* set session to blocking */

    // const ssh_session aSession = (ssh_session)malloc(sizeof(struct ssh_session_struct*));
    // ssh_init();
    //
    //
    //
    // sftp_session sftp;
    // ssh_session session;
    // session = ssh_new();
    // // sftp = (sftp_session)malloc(sizeof(struct sftp_session_struct));
    // sftp_init(sftp);
    // sftp = sftp_new(session);




    /* Read permissions of a source file */
    // const auto dup = connection->sshSession();
    // ssh_scp scp;
    // sftp = sftp_new(connection->sshSession());
    // sftp_init(sftp);
    // scp = ssh_scp_new(connection->sshSession(), SSH_SCP_WRITE | SSH_SCP_READ | SSH_SCP_RECURSIVE, finalPath);
    // ssh_scp_init(scp);
    // ssh_scp_push_directory(scp, finalPath, 0755);
    // sftp_mkdir(sftp, finalPath, 0755);
    // SftpAttrs *dirPerms = new SftpAttrs();


    /* Read permissions of a source file */
    // struct stat results;
    // stat(file.toUtf8(), &results);

    /* Request a file via SFTP */
    // sftp_file sftp_handle;
    // while (sftp_handle == NULL) { /* it's case when network connection is overloaded */
    //
    //

    /* NOTE: we redeclaring SFTP session */
    // if (sftp != NULL) {
    //     sftp_free(sftp);
    //     sftp = NULL; // necessary?
    // }
    // sftp = sftp_new(connection->sshSession());
        // cout << "'";
        // fflush(stdout);
        // usleep(100);
    // }

    // if (scp != NULL) {
    //     ssh_scp_close(scp);
    //     ssh_scp_free(scp);
    // }
    // scp = ssh_scp_new(connection->sshSession(), SSH_SCP_WRITE, fullDestPath.toUtf8());
    // ssh_scp_init(scp);
    // terminate_ssh_session();
    // ssh_free(session);
    // if (sftp != NULL) {
    //     qDebug() << "sftp NOT null";
    //     sftp_free(sftp);
    // }

    // sftp = sftp_new(ssh_new());
    // sftp = (struct sftp_session_struct*)malloc(sizeof(struct sftp_session_struct));
    // sftp_init(sftp);

    // sftp_file sftp_handle = sftp_open(sftp, fullDestPath.toUtf8(), results.st_mode, results.st_mode);













    // SftpAttrs attrs;
    // SftpFileHandle *pSftpFileHandle = NULL;
    // uint32 bytesRead = 0, bufLen = 0;
    // uint8 *pBuf = NULL;

    // int result = ptssh_openFile(sftp, &pSftpFileHandle, file.toUtf8(), FO_RDONLY);
    // if ( result != PTSSH_SUCCESS) {
    //     qDebug() << "Errorzz" << result << file;
    //     return;
    // }
    // result = sftp->getFileAttributes(file.toUtf8(), false,  &attrs);
    // if ( result != PTSSH_SUCCESS) {
    //     qDebug() << "Error bo wujek Stefan";
    //     return;
    // }

    // memset( &attrs, 0x0, sizeof(SFTP_W_ATTR));
    // // attrs.permissions =
    // //     FP_USR_RWX |
    // //     FP_GRP_RWX |
    // //     FP_OTH_RWX;  //Sets permissions to 0777 (Unix style)

    // bufLen = (uint32)attrs.fileSize();
    // memset(pBuf, 0x0, bufLen);

    // #ifdef SHOW_STATISTICS
    //     start = clock();
    // #endif

    // //Create a buffer big enough to hold the file
    // pBuf = new uint8[ (uint32)attrs.fileSize()];
    // if (! pBuf) {
    //     qDebug() << "Fukof";
    //     return;
    // }


    // result = pSftpFileHandle->read(pBuf, bufLen, 0, bytesRead);
    // if ( result != PTSSH_SUCCESS)
    // {
    //     qDebug() << "Blablof";
    //     return;
    // }
    // printf("Read %d bytes\n", bytesRead);

    // #ifdef SHOW_STATISTICS
    //     {
    //         stop = clock();

    //         uint32
    //             KBs = ((uint32)bufLen) >> 10,
    //             MBs = ((uint32)bufLen) >> 20;

    //         double
    //             //elapsedTimeInSec = stop - start,
    //             elapsedTimeInSec = ((double)(stop - start)) / ((double)CLOCKS_PER_SEC),

    //             KbytesPerSec = KBs / (elapsedTimeInSec),
    //             MbytesPerSec = MBs / (elapsedTimeInSec);

    //         printf("SFTP transfered %u bytes in %4.2f sec (%4.2fKB/sec %4.2fMB/sec\n",
    //             bufLen, elapsedTimeInSec, KbytesPerSec, MbytesPerSec);
    //     }
    // #endif





    // ifstream fin(file.toUtf8(), ios::binary);
    // if (fin) {
    //     fin.seekg(0, ios::end);
    //     long bufsize = fin.tellg(); /* get file size in bytes */

        // long BUFF = bufsize;
        // if (bufsize > MAXBUF) /* set buffer to 12 KiB if file size exceeds limit */
            // BUFF = MAXBUF;

        // qDebug() << "File size:" << bufsize/1024 << "KiB. Memory buffer size:" << BUFF << "bytes";
        // fin.seekg(0); /* rewind to beginning of file */
        // qDebug() << "Streaming:" << file;

        // uint8* buf = new uint8[BUFF];
        // uint chunk = 0; /* used only to count % progress */
    //     while (fin.good()) {
    //         fin.read(buf, BUFF); /* read file contents into buffer; todo: sanity checks */
    //         // int result = ssh_scp_write(scp, buf, fin.gcount()); /* write to remote file */
    //         // int result = sftp_write(sftp_handle, buf, fin.gcount()); /* write to remote file */
    //         SftpFileHandle *fileHandle = new SftpFileHandle(manager, ChannelManager *const pChannelMgr, uint32 cNum, uint32 remoteChannelNum, uint8 sftpVer);

    //         fileHandle->write(buf, fin.gcount());

    //         ptssh_writeFile(sftp, buf, fin.gcount());
    //         // if (result < 0) {
    //         //     qDebug() << "Error allocating buffer with size:" << bufsize;
    //         //     break; // XXX: what now?
    //         // }
    //         chunk += 1;
    //         if (bufsize > 0) {
    //             int percent = (chunk * BUFF) * 100 / bufsize;
    //             cout << "(" << percent << "%) " << (chunk * BUFF)/1024 << '/' << bufsize/1024 << " KiB" << '\r';
    //             fflush(stdout);
    //         }
    //     }
    //     delete[] buf;
    //     if (bufsize < 1024)
    //         qDebug() << "\r(100%)" << bufsize << "Bytes sent.";
    //     else
    //         qDebug() << "\r(100%)" << bufsize/1024 << "KiB sent.";

    /* rename remote file to hash name */
    if (hashFile) {
        #ifdef GUI_ENABLED
            // qDebug() << "Remote rename from" << fullDestPath << "to" << remotePath + "/" + result + "." + extension;
            // libssh2_sftp_rename(sftp_session, fullDestPath.toUtf8(), (remotePath + "/" + result + "." + extension).toUtf8());
            QString destName = (remotePath + "/" + result + "." + extension).toUtf8();
            qDebug() << "Renaming" << file << "to" << destName;
            result = ptssh_renameFileOrDir(sftp, fullDestPath.toUtf8(), destName);
            if ( result != PTSSH_SUCCESS) {
                // printf("Error renaming directory from\n\t%s\n -to-\n\t%s\nError %d\n",
                //     pRemoteDirToCreate, pRemoteDirRenamed, result);
                //return -1;
                qDebug() << "Remote dir rename failed";
            }
        #endif
    }
    // }
    // fin.close();
    // sftp_close(sftp_handle);
    // sftp_free(sftp);
    // if (scp != NULL) {
    //     ssh_scp_close(scp);
    //     ssh_scp_free(scp);
    // }


    #ifdef GUI_ENABLED
        QSettings settings;
        QSound::play(settings.value("sound_file", DEFAULT_SOUND_FILE).toString());
        emit setWork(OK);
        notify("Screenshot uploaded. Link copied to clipboard");
    #endif
    qDebug() << "Total files and dirs on watch:" << files.size();
    qDebug() << "Done. Time elapsed:" << myTimer.elapsed() << "miliseconds";
}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    if (not QDir(dir).exists()) {
        qDebug() << "Dir has gone. Assuming directory deletion of:" << dir;
        removePath(dir);
        scanDir(QDir(dir + "/.."));
    } else {
        qDebug() << "Dir changed:" << dir;
        scanDir(QDir(dir)); /* don't scan non existent directories */

        /* scan for new files by file list diff */
        Q_FOREACH(QString nextOne, files) {
            if (QFile::exists(nextOne)) {
                if (not QDir(nextOne).exists()) {
                    // qDebug() << "Traversing next file:" << nextOne;
                    if (not oldFiles.contains(nextOne)) {
                        // qDebug() << "New file found in monitored dir:" << nextOne;
                        #ifdef GUI_ENABLED
                            emit setWork(WORKING);
                            copyFileToRemoteHost(nextOne, true);
                        #else
                            emit fileChangedSlot(nextOne);
                        #endif
                    }
                }
            } else {
                #ifdef GUI_ENABLED
                    emit setWork(DELETE);
                    copyFileToRemoteHost(nextOne, true);
                #else
                    emit fileChangedSlot(nextOne);
                #endif
            }
        }
    }
}
