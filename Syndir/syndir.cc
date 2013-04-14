/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"
#include "worker_thread.h"


void usage() {
    logDebug() << "Usage:" << endl
             << "syndir ~/Projects/MyProjectDir dmilith@myhost.com:/remote/destination/of/MyProjectDir [ more.host.com:/also/here/MyProjectDir.copy ]";
}


static void loadDefaultSettings() {
    QSettings settings;

    if (settings.value("source_dir").isNull())
        settings.setValue("source_dir", DEFAULT_SOURCE_DIR);

    if (settings.value("ssh_port").isNull())
        settings.setValue("ssh_port", SSH_PORT);

    if (settings.value("ssh_password").isNull())
        settings.setValue("ssh_password", SSH_PASSWORD);

    if (settings.value("allowed_file_types").isNull())
        settings.setValue("allowed_file_types", ALLOWED_FILE_TYPES);
}


int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("VerKnowSys");
    QCoreApplication::setOrganizationDomain("verknowsys.com");
    QCoreApplication::setApplicationName("Syndir");
    loadDefaultSettings();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
    QStringList args = app.arguments();

    QStringList remotes;
    QList<WorkerThread*> workers;

    /* Logger setup */
    ConsoleAppender *consoleAppender = new ConsoleAppender();
    Logger::registerAppender(consoleAppender);
    consoleAppender->setFormat("%t{dd-HH:mm:ss} [%-7l] <%c:(%F:%i)> %m\n");
    consoleAppender->setDetailsLevel(Logger::Trace);

    /* handle bad arguments */
    if (args.size() < 3) {
        usage();
        exit(1);
    }
    QString sourceDir = args.at(1);

    for (int i = 2; i < args.length(); i++) {
        remotes << args.at(i);
    }
    cout << "Syndir v" << APP_VERSION << " - " << COPYRIGHT << endl << endl;

    for (int i = 0; i < remotes.length(); i++) {
        logDebug() << "Creating thread for remote:" << remotes.at(i);
        workers << new WorkerThread(sourceDir, remotes.at(i));
        workers.at(i)->start();
    }

    return app.exec();
}
