/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"
#include "worker_thread.h"


void usage() {
    qDebug() << "Usage:" << endl
             << "syndir ~/Projects/MyProjectDir dmilith@myhost.com:/remote/destination/of/MyProjectDir [ more.host.com:/also/here/MyProjectDir.copy ]";
}


int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
    QStringList args = app.arguments();

    QStringList remotes;
    QList<WorkerThread*> workers;

    /* handle bad arguments */
    if (args.size() < 3) {
        usage();
        exit(1);
    }
    QString sourceDir = args.at(1);

    for (int i = 2; i < args.length(); i++) {
        remotes << args.at(i);
    }
    cout << "Sofin v" << APP_VERSION << " - © 2o13 - VerKnowSys" << endl << COPYRIGHT << endl << endl;

    for (int i = 0; i < remotes.length(); i++) {
        qDebug() << "Creating thread for remote:" << remotes.at(i);
        workers << new WorkerThread(sourceDir, remotes.at(i));
        workers.at(i)->start();
    }

    return app.exec();
}
