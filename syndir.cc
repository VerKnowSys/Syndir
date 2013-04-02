/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


void usage() {
    qDebug() << "Usage:" << endl
             << "syndir ~/Projects/MyProjectDir dmilith@myhost.com:/remote/destination/of/MyProjectDir [ more.host.com:/also/here/MyProjectDir.copy ]";
}


int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(DEFAULT_STRING_CODEC));
    QStringList args = app.arguments();

    /* handle bad arguments */
    if (args.size() < 3) {
        usage();
        exit(1);
    }
    QString sourceDir = args.at(1);
    QStringList remotes;
    for (int i = 2; i < args.length(); i++) {
        remotes << args.at(i);
    }
    cout << "Sofin v" << APP_VERSION << " - © 2o13 - VerKnowSys" << endl << COPYRIGHT << endl << endl;

    for (int i = 0; i < remotes.length(); i++) {
        qDebug() << "Loading remote:" << remotes.at(i);
        new FileWatchersManager(sourceDir, remotes.at(i));
    }

    return app.exec();
}
