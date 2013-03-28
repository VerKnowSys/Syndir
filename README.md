# Syndir - SCP/SFTP file synchronization utility.


## Author:
* Daniel (dmilith) Dettlaff (dmilith [at] verknowsys.com). I'm also on #freebsd and #scala.pl @ freenode IRC.


## Contributors:
* Michał (tallica) Lipski


## Examples:
```sh
syndir /Volumes/Projects/MyProjectDir dmilith@myhost.com:/existing/destination/MyProjectDir
```
Will launch Syndir in watch mode, automatically uploading each modified file from MyProjectDir to destination MyProjectDir. It works recursively thoughout source directory structure.


## TODO:
* Fix remote mkdir issues. (for example it will currently fail for multiple/non/existant/path given).
* Idiot proof code.


## License:
* Released under the [BSD](http://opensource.org/licenses/BSD-2-Clause) license.
