# Syndir - SCP file synchronization utility.
# Synshot - GUI Screenshot utility for power users.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] verknowsys.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


## Contributors:
* Michał (tallica) Lipski


## Syndir features:
* Synchronize your files fast. Works with SCP.
* Live file monitoring. Syndir by default runs in foreground as blocking application, watching directories recursively. On each file change it will automatically upload changed file to remote host in destination folder.
* Any amount of remote hosts supported. Just give them as additional params.
* Automatic creation of remote non existent remote directories.
* Full support for File & Directory creation/ deletion.
* Asynchronous, parallel transfers for multiple hosts at once.
* Uses almost no memory (< 5MiB RSS for 64bit app, even when syncing huge files)


## Synshot features:
* Uses Syndir core. Uses SSH public key authentication.
* Simple, small tray application for OSX (and probably Linux/FreeBSD too).
* Blazing fast. It reuses and maintains single connection to SCP host.
* Works under huge IO/Network loads.
* Security through obscurity. Each screenshot file is renamed on the fly to SHA1 of it's name.
* After each upload automatically copies destination link to system clipboard.
* Tested by a power user. Not kids from marketing toy store.
* Supports Growl notifications under OSX.
* Supports Sound notifications.
* Requires a simple Nginx config to bind remote address to your server. I use:

```nginx
server {
   listen s.verknowsys.com;
   server_name s.verknowsys.com;
   root /home/dmilith/Web/Public/Sshots/;
   autoindex off;
}
```


## Requirements:
* [Qt4 4.8.x](http://qt-project.org/downloads) (only QtCore part of framework for Syndir, QtGui for Synshot)
* [LibSSH2 1.4.x](http://www.libssh2.org)


## Preffered/Optional requirements.:
* [Sofin 0.46.x](http://verknowsys.github.com/sofin/) - Because it has predefined "qt" (with minimal build of Qt4 for servers) and "libssh2" definitions. Sofin is used by default in Syndir's installation script: [make_all.sh](https://github.com/VerKnowSys/Syndir/blob/master/make_all.sh).
* Qt4 GUI libraries to build Synshot (screenshot sync utility)
* [Growl](http://growl.info) - When available, will be used for notifications in GUI app. If unavailable, you'll get only sound notifications.


## Building:
```sh
./make_all.sh # standard build.
```
```sh
PACKAGE=YES ./make_all.sh # will also create standalone Synshot.app with all requirements bundled.
```


## Examples:
```sh
syndir /Volumes/Projects/MyProjectDir myhost.com:/existing/destination/MyProjectDir
```
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir
```
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir more.hosts:/somewhere and.even.more:/copies/myproject (...)
```
Will launch Syndir in watch mode, automatically uploading each modified file from MyProjectDir(s) to destination MyProjectDir. It works recursively thoughout directory structure for each given remote location.


## OSX binary builds download:
* [Synshot binary builds](http://dmilith.verknowsys.com/Public/Synshot-releases)
* Current version: [Synshot 0.15.x](http://dmilith.verknowsys.com/Public/Synshot-releases)


## KNOWN BUGS:
* Synshot/ Syndir are currently unable to gracefully handle "too many open files" error on deep/big source directories.
* Currently no SSH keys are working. For now only password auth is supported (it requires some more work)


## License:
* Released under the [BSD](http://opensource.org/licenses/BSD-2-Clause) license.


## Used 3rd party software and licenses info:
* PTssh implementation by Paul Thomas (BSD licensed)
* CuteLogger MT logger implementation by Boris Moiseev (LGPL licensed)
* QT4 4.8.x implementation (LGPL licensed)
