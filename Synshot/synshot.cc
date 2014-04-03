/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */


#include <QtGui/QtGui>

#include "../Syndir/syndir.h"
#include "../Syndir/worker_thread.h"
#include "synshot_config_widget.h"
#include "synshot.h"


#ifdef GUI_ENABLED

    void loadDefaultSettings() {
        QSettings settings;

        if (settings.value("source_dir").isNull())
            settings.setValue("source_dir", DEFAULT_SOURCE_DIR);

        if (settings.value("destination_dir").isNull())
            settings.setValue("destination_dir", DEFAULT_DESTINATION_DIR);

        if (settings.value("remote_path").isNull())
            settings.setValue("remote_path", REMOTE_PATH);

        // if (settings.value("sound_file").isNull())
        //     settings.setValue("sound_file", DEFAULT_SOUND_FILE);

        if (settings.value("ssh_port").isNull())
            settings.setValue("ssh_port", SSH_PORT);

        if (settings.value("ssh_password").isNull())
            settings.setValue("ssh_password", SSH_PASSWORD);

        if (settings.value("allowed_file_types").isNull())
            settings.setValue("allowed_file_types", ALLOWED_FILE_TYPES);
    }


    int main(int argc, char *argv[]) {

        QApplication app(argc, argv);
        QCoreApplication::setOrganizationName("VerKnowSys");
        QCoreApplication::setOrganizationDomain("verknowsys.com");
        QCoreApplication::setApplicationName("Synshot");
        loadDefaultSettings();
        QSettings settings;
        settings.setValue("lastRun", QDateTime::currentDateTime());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
        QStringList args = app.arguments();

        QRegExp rxEnableForeground("-f"); /* run in foreground */
        QRegExp rxEnableDebug("-d");
        QRegExp rxEnableTrace("-t");

        bool debug = true, trace = false, background = true;
        for (int i = 1; i < args.size(); ++i) {
            if (rxEnableDebug.indexIn(args.at(i)) != -1 ) {
                debug = true;
            }
            if (rxEnableForeground.indexIn(args.at(i)) != -1 ) {
                background = false;
            }
            if (rxEnableTrace.indexIn(args.at(i)) != -1 ) {
                debug = true;
                trace = true;
            }
        }

        /* Logger setup */
        if (not background) {
            ConsoleAppender *consoleAppender = new ConsoleAppender();
            Logger::registerAppender(consoleAppender);
            consoleAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] <%c:(%F:%i)> %m\n");
            if (trace && debug)
                consoleAppender->setDetailsLevel(Logger::Trace);
            else if (debug && !trace)
                consoleAppender->setDetailsLevel(Logger::Debug);
            else {
                consoleAppender->setDetailsLevel(Logger::Info);
                consoleAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] %m\n");
            }

        } else {
            FileAppender *fileAppender;
            fileAppender = new FileAppender(QString(getenv("HOME")) + DEFAULT_SYNSHOT_LOG_FILE);

            Logger::registerAppender(fileAppender);
            fileAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] <%c:(%F:%i)> %m\n");
            if (trace && debug)
                fileAppender->setDetailsLevel(Logger::Trace);
            else if (debug && !trace)
                fileAppender->setDetailsLevel(Logger::Debug);
            else {
                fileAppender->setDetailsLevel(Logger::Info);
                fileAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] %m\n");
            }
        }

        logInfo() << "Launching Synshot v" + QString(APP_VERSION);

        /* autogen of ppk ssh key */
        QString sourcePrivateKey = QString(getenv("HOME")) + ID_SSH_DIR + ID_RSA;
        QString destPrivateKey = QString(getenv("HOME")) + ID_SSH_DIR + ID_RSA_PPK;
        if (QFile::exists(sourcePrivateKey)) {
            logInfo() << "RSA SSH key found.";
            if (not QFile::exists(destPrivateKey)) {
                logInfo() << "PPK key not found. Autogenerating from available RSA key";
                QString puttyGen = QString("/Applications/Synshot.app/Contents/MacOS/") + PUTTY_GEN;
                if (not QFile::exists(puttyGen))
                    puttyGen = PUTTY_GEN; /* if not found in app dir, use puttygen from PATH */
                QStringList processArguments;
                processArguments << sourcePrivateKey << "-o" << destPrivateKey;
                QProcess *process = new QProcess();
                process->start(puttyGen, processArguments);
                process->waitForReadyRead(5000); /* XXX: hardcoded 5s */
                process->terminate();
                process->deleteLater();
            } else
                logInfo() << "PPK SSH key found.";
        }

        QStringList remotes;

        /* init instance of config window for GUI */
        ConfigWindow *configWindow = new ConfigWindow();

        /* read config values */
        QString sourceDir = settings.value("source_dir").toString().trimmed().replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir */
        QString destination = settings.value("destination_dir").toString().trimmed();

        auto worker = new WorkerThread(configWindow, sourceDir, destination);
        worker->start();

        logDebug() << "Synshot initialized.";

        return app.exec();
    }

#endif
