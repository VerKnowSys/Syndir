/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "synshot.h"


void usage() {
    qDebug() << "Usage:" << endl
             << "synshot ~/Projects/MyProjectDir dmilith@myhost.com:/remote/destination/of/MyProjectDir [ more.host.com:/also/here/MyProjectDir.copy ]";
}


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
    QStringList args = app.arguments();

    QStringList remotes;
    QList<WorkerThread*> workers;

    /* handle bad arguments */
    QString sourceDir;
    if (args.size() < 3) { /* standalone default mode */
        qDebug() << "Standalone app spawned with default arguments";
        sourceDir = DEFAULT_SOURCE_DIR;
        remotes << DEFAULT_DESTINATION_DIR;
    } else {
        sourceDir = args.at(1);

        for (int i = 2; i < args.length(); i++) {
            remotes << args.at(i);
        }
        cout << "Sofin v" << APP_VERSION << endl << COPYRIGHT << endl << endl;
    }

    for (int i = 0; i < remotes.length(); i++) {
        qDebug() << "Creating thread for remote:" << remotes.at(i);
        workers << new WorkerThread(sourceDir, remotes.at(i), true);
        workers.at(i)->start();
    }

    return app.exec();
}
