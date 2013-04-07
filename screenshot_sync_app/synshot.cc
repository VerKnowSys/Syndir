/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */


#include <QtGui/QtGui>

#include "../syndir.h"
#include "../worker_thread.h"
#include "synshot_config_widget.h"


#ifdef GUI_ENABLED

    static void loadDefaultSettings() {
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
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
        QStringList args = app.arguments();

        QStringList remotes;
        QList<WorkerThread*> workers;

        /* init instance of config window for GUI */
        ConfigWindow *configWindow = new ConfigWindow();

        /* handle bad arguments */
        QString sourceDir;
        if (args.size() < 3) { /* standalone default mode */
            qDebug() << "Standalone app spawned with default arguments";
            sourceDir = settings.value("source_dir", DEFAULT_SOURCE_DIR).toString();
            remotes << settings.value("destination_dir", DEFAULT_DESTINATION_DIR).toString();
        } else {
            sourceDir = args.at(1);

            for (int i = 2; i < args.length(); i++) {
                remotes << args.at(i);
            }
            cout << "Sofin v" << APP_VERSION << endl << COPYRIGHT << endl << endl;
        }

        for (int i = 0; i < remotes.length(); i++) {
            qDebug() << "Creating thread for remote:" << remotes.at(i);
            sourceDir = sourceDir.replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir */
            workers << new WorkerThread(configWindow, sourceDir, remotes.at(i), true);
            workers.at(i)->start();
        }

        return app.exec();
    }

#endif
