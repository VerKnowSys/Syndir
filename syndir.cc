/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include <QtCore>
#include <libssh2.h>

#include "file_watcher.h"

using namespace std;


#define APP_VERSION "0.1.0"
#define COPYRIGHT "Copyright © 2o13 VerKnowSys.com - All Rights Reserved."
#define DEFAULT_STRING_CODEC "UTF8"


/* by tallica */
const QStack<QString> *scanDir(QDir dir) {
    QStack<QString> *files = new QStack<QString>();
    dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(dir, QDirIterator::Subdirectories);

    files->push(dir.absolutePath());

    while (it.hasNext())
        files->push(it.next());

    return files;
}


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
    QString destinationDir = args.at(2);

    QStringList sshDirPartial = destinationDir.split(":");
    QStringList sshUserServerPartial = sshDirPartial.value(0).split("@"); /* first part is user@host */
    QString remotePath = sshDirPartial.value(1); /* last one is a remote path */
    QString userName = sshUserServerPartial.value(0);
    QString hostName = sshUserServerPartial.value(1);

    qDebug() << "Remote setup inspection:\nRemote user:" << userName << "\nRemote host:" << hostName << "\nRemote path:" << remotePath;

    if (userName.isEmpty()) {
        qDebug() << "No user name specified, trying to get it from ENV.";
        userName = getenv("USER");
    }

    if (hostName.isEmpty() or userName.isEmpty()) {
        qDebug() << "Invalid (empty) host or user name.";
        usage();
        exit(1);
    }
    qDebug() << "Watching:" << sourceDir << "with sync to:" << destinationDir;

    QList<FileWatcher *> fileWatchers;

    const QStack<QString> *files = scanDir(QDir(sourceDir));
    qDebug() << "Total files:" << files->size();

    qDebug() << "Creating file watches recursively…";
    for (int i = 0; i < files->size(); ++i) {
        auto entry = files->at(i);
        auto watcher = new FileWatcher(entry, remotePath);
        watcher->addPath(entry);
        // qDebug() << "Adding:" << entry;
        fileWatchers << watcher;
    }

    return app.exec();
}
