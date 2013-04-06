# Syndir - SCP/SFTP file synchronization utility.
# Synshot - GUI Screenshot utility for power users.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] verknowsys.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


## Contributors:
* Michał (tallica) Lipski


## Syndir features:
* Synchronize your files with 0 setup. Works with SFTP. Requires SSH public key authentication.
* Live file monitoring. Syndir by default runs in foreground as blocking application, watching directories recursively. On each file change it will automatically upload changed file to remote host in destination folder.
* Any amount of remote hosts supported. Just give them as additional params.
* Automatic creation of remote non existent remote directories.
* Full support for File & Directory creation/ deletion.
* Asynchronous, parallel transfers for multiple hosts at once.
* Uses almost no memory (< 5MiB RSS for 64bit app, even when syncing huge files)


## Synshot features:
* Uses Syndir core. Uses SSH public key authentication.
* Simple, small tray application for OSX and Linux.
* Blazing fast. It reuses and maintains single connection to SFTP host.
* Works under huge IO/Network loads.
* Security through obscurity. Each screenshot is uploaded first, then renamed to SHA1 of it's name.
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
./make_all.sh
```
or
```sh
PACKAGE=YES ./make_all.sh # will also create standalone Synshot.app with all requirements bundled.
```


## Examples:
```sh
syndir /Volumes/Projects/MyProjectDir myhost.com:/existing/destination/MyProjectDir
```
or
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir
```
or
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir more.hosts:/somewhere and.even.more:/copies/myproject (...)
```
Will launch Syndir in watch mode, automatically uploading each modified file from MyProjectDir(s) to destination MyProjectDir. It works recursively thoughout directory structure for each given remote location.


## TODO:
* Gracefully handle "too many open files" error on deep/big source directories.


## License:
* Released under the [BSD](http://opensource.org/licenses/BSD-2-Clause) license.
