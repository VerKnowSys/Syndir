/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


void usage() {
    qDebug() << "Usage:" << endl
             << "Syndir /source/directory/name user@destination_server:/destination_path";
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

    qDebug() << "Watching:" << sourceDir << "with sync to:" << fullDestinationSSHPath;
    qDebug() << "Creating file watches recursively…";
    new FileWatchersManager(sourceDir, fullDestinationSSHPath);

    return app.exec();
}
