# FileSpam
Utility for spamming programs that watch for file system changes

Built originally for testing how Bitwig scans VST directories, this application can be used to test any software that actively listens for file system chnages and IO sctivity.

## Building
Requires CMake and a C++ 14 compatible compiler.

I personally use [CLion](https://www.jetbrains.com/clion/) as I find it easy to setup and use on windows.

## Usage/Commands
 - help: Prints the help menu.
 - make: Creates a file in the same directory of the executable.
 - del:  Deletes a file int eh same directory of the executable.
 - time: This does the spam test. There are three components:
   - filename: The name of the file to create and delete.
   - interval: The number of times to create and delete the file.
   - delay time: The delay in milliseconds between file creation and deletion.
 - open: creates an open file stream to the file without closing it and registers it with the program
 - close: closes an already opened and registered file stream
 - list: lists all currently registered open file streams

## Author
Chris Soderquist - Computer Science Major and Audio Enthusiast

## License
See [LICENSE](./LICENSE)