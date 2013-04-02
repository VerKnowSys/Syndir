# Syndir - SCP/SFTP file synchronization utility.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] verknowsys.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


## Contributors:
* Michał (tallica) Lipski


## Features:
* Synchronize your files with 0 setup. Works with SFTP. Requires public key authentication.
* Live file monitoring. Syndir by default runs in foreground as blocking application, watching directories recursively. On each file change it will automatically upload changed file to remote host in destination folder.
* Any amount of remote hosts supported. Just give them as additional params.
* Automatic creation of remote non existent remote directories.
* Full support for File & Directory creation/ deletion.
* Asynchronous, parallel transfers for multiple hosts at once.
* Uses almost no memory (< 5MiB RSS for 64bit app, even when syncing huge files)


## Requirements:
* [Qt4 4.8.x](http://qt-project.org/downloads)
* [LibSSH2 1.4.x](http://www.libssh2.org)
* [Sofin 0.46.x](https://github.com/VerKnowSys/sofin)


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
