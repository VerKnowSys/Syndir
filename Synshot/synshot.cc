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
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
        QStringList args = app.arguments();

        /* Logger setup */
        ConsoleAppender *consoleAppender = new ConsoleAppender();
        Logger::registerAppender(consoleAppender);
        consoleAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] <%c:(%F:%i)> %m\n");
        consoleAppender->setDetailsLevel(Logger::Trace);

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