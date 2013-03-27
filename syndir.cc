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
        exit(0);
    }
    QString sourceDir = args.at(1);
    QString destinationDir = args.at(2);
    qDebug() << "Watching:" << sourceDir << "with sync to:" << destinationDir;

    QList<FileWatcher *> fileWatchers;

    const QStack<QString> *files = scanDir(QDir(sourceDir));
    qDebug() << "Total files:" << files->size();
    qDebug() << "Creating file watch for each file…";
    for (int i = 0; i < files->size(); ++i) {
        auto entry = files->at(i);
        auto watcher = new FileWatcher(entry);
        watcher->addPath(entry);
        qDebug() << "Adding:" << entry;
        fileWatchers << watcher;
    }

    return app.exec();
}
