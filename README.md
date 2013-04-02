# Syndir - SCP/SFTP file synchronization utility.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] verknowsys.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


## Contributors:
* Michał (tallica) Lipski


## Examples:
```sh
syndir /Volumes/Projects/MyProjectDir myhost.com:/existing/destination/MyProjectDir
```
or
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir
```
Will launch Syndir in watch mode, automatically uploading each modified file from MyProjectDir to destination MyProjectDir. It works recursively thoughout directory structure.


## TODO:
* Gracefully handle "too many open files" error on deep/big source directories.


## License:
* Released under the [BSD](http://opensource.org/licenses/BSD-2-Clause) license.
