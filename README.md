# Syndir - SCP file synchronization utility.
# Synshot - GUI Screenshot utility for power users.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] me.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


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
* Uses Syndir core.
* Has Putty (*.ppk) SSH keys support. Synshot will generate `id_rsa.ppk` from your `id_rsa` automatically if it doesn't exists. Please note, that only unecrypted and passless keys are supported (for now).
* Simple, small tray application (< 25MiB RSS) for OSX (and probably Linux/FreeBSD too, but I support only OSX).
* Works under huge IO/Network loads on slow networks.
* It's secure by default. Each file is renamed on the fly to SHA1 of it's name. Try to guess that.
* After each upload, it automatically copies destination link to system clipboard, while uploading file in background.
* Supports Growl notifications.
* Supports any file synchronization (HD movies upload? why not?). Just put sync folder in your Dock. Then just drop any file there, and Synshot will do the rest. Easy.
* Tested by a power user. Not kids from marketing toy store.
* Requires a simple Nginx config to bind remote address to your server. I use:

```nginx
server {
   listen s.verknowsys.com;
   server_name s.verknowsys.com;
   root /home/dmilith/Web/Public/SyncFiles/;
   autoindex off;
}
```


## Requirements:
* [Qt4 4.8.x](http://qt-project.org/downloads) (only QtCore part of framework for Syndir, QtGui for Synshot)


## Preffered/Optional requirements.:
* [Sofin 0.51.x](http://verknowsys.github.com/sofin/)
* Qt4 GUI libraries to build Synshot (for GUI to file sync utility)
* [Growl](http://growl.info) - When available, will be used for notifications in GUI app.


## Building:
`bin/build`


## Packaging:
`bin/package`


## Syndir usage examples:
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


## OSX 64bit binary builds (for OSX 10.6.x - 10.9.x):
* [Synshot binary builds site](http://dmilith.verknowsys.com/Public/Synshot-releases)


## KNOWN ISSUES/ DRAWBACKS:
* Synshot/ Syndir are currently unable to gracefully handle "too many open files" error on deep/big source directories.
* Synshot might be CPU heavy (single thread) while uploading large files. This might be an issue for mobile machines using battery power. It's not that big when uploading small files (e.g. screenshot). It's caused very efficient sending algorithm used in PtSSH implementation.
* Sound notifications are currently disabled by default. It's causes app to leak memory, when using JackD Audio System as primary sound device.


## License:
* Released under the [BSD](http://opensource.org/licenses/BSD-2-Clause) license.


## Used 3rd party software and licenses info:
* PTssh implementation by Paul Thomas (BSD licensed)
* CuteLogger MT logger implementation by Boris Moiseev (LGPL licensed)
* QT4 4.8.x implementation (LGPL licensed)
