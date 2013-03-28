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


bool sshConnectionTest(const QString& remoteSSHPath) {
    QStringList sshDirPartial = remoteSSHPath.split(":");

    /* user might be implicit: */
    QString userWithHost = sshDirPartial.value(0);
    QStringList sshUserServerPartial = userWithHost.split("@"); /* first part is user@host */
    QRegExp emailSign("@");
    QString userName, hostName;
    if (!userWithHost.contains(emailSign)) {
        userName = getenv("USER"); /* set username if not given explicitly */
        hostName = sshUserServerPartial.value(0);
        qDebug() << "Implicit" << userName << hostName;
    } else { /* or explicit */
        qDebug() << "Explicit";
        userName = sshUserServerPartial.value(0);
        hostName = sshUserServerPartial.value(1);
    }
    QString remotePath = sshDirPartial.value(1); /* last one is a remote path */

    try {
        QString keysLocation = QString(getenv("HOME")) + "/.ssh";
        qDebug() << "Trying to login to" << hostName << "as user" << userName;
        qDebug() << "Looking for SSH keys in:" << keysLocation;
        Connection connection(hostName.toStdString(), 22, true);
        connection.setKeyPath(keysLocation.toStdString());

        UserInfo ui = connection.getUserInfo();
        cout    << "User infos:\n"
                << "   login: " << ui.getUserName() << endl
                << "    home: " << ui.getHomeDir() << endl
                << "   shell: " << ui.getUserShell() << endl;

        connection.mkConnection();

        if (connection.isSessionValid())
            qDebug() << "Connection OK";

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
    qDebug() << "Watching:" << sourceDir << "with sync to:" << fullDestinationSSHPath;
    qDebug() << "Creating file watches recursively…";
    new FileWatchersManager(sourceDir, fullDestinationSSHPath);


    return app.exec();
}
