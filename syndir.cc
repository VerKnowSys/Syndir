/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


void usage() {
    qDebug() << "Usage:" << endl
             << "syndir ~/Projects/MyProjectDir dmilith@myhost.com:/remote/destination/of/MyProjectDir";
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
    QString fullDestinationSSHPath = args.at(2);
    cout << "Sofin v" << APP_VERSION << " - © 2o13 - VerKnowSys" << endl << COPYRIGHT << endl << endl;
    new FileWatchersManager(sourceDir, fullDestinationSSHPath);

    return app.exec();
}
