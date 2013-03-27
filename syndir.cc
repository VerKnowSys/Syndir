/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


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


bool sshConnectionTest(const QString& remoteSSHPath) {
    QStringList sshDirPartial = remoteSSHPath.split(":");
    QStringList sshUserServerPartial = sshDirPartial.value(0).split("@"); /* first part is user@host */
    QString remotePath = sshDirPartial.value(1); /* last one is a remote path */
    QString userName = sshUserServerPartial.value(0);
    QString hostName = sshUserServerPartial.value(1);

    try {
        Connection connection(hostName.toStdString(), 22, true);
        connection.setKeyPath(string("/Users/") + userName.toStdString() + string("/.ssh"));

        UserInfo ui = connection.getUserInfo();

        // cout    << "User infos:\n"
        //         << "   login: " << ui.getUserName() << endl
        //         << "    home: " << ui.getHomeDir() << endl
        //         << "   shell: " << ui.getUserShell() << endl;

        connection.mkConnection();

        if (connection.isSessionValid())
            qDebug() << "Connection OK";

        connection  >> "whoami"
                    >> "id"
                    >> "echo 'test' > msg"
                    >> "cat msg";

        QString received_out = connection.getLastOutput().c_str();
        qDebug() << "Received output:\n" << received_out;

        return true;

    } catch (Exception & e) {
        qDebug() << "Exception caught: " << e.what();
    }
    return false;
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

    if (!sshConnectionTest(fullDestinationSSHPath)) {
        qDebug() << "SSH Connection test failed! Check your public key configuration or look for typo in cmdline";
        exit(1);
    }
    QList<FileWatcher *> fileWatchers;
    const QStack<QString> *files = scanDir(QDir(sourceDir));
    qDebug() << "Watching:" << sourceDir << "with sync to:" << fullDestinationSSHPath;
    qDebug() << "Total files:" << files->size();

    qDebug() << "Creating file watches recursively…";
    for (int i = 0; i < files->size(); ++i) {
        auto entry = files->at(i);
        fileWatchers << new FileWatcher(entry, fullDestinationSSHPath);
    }

    return app.exec();
}
