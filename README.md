# AutoDownloadsOrganizer
A C++ script to automatically organize 'Downloads' directory by file types into categorized folders.

It sorts files in the downloads directory into JPGs, PNGs, ZIPs etc. It creates the folders necessary if they aren't available.

This script will not overwrite your files if they have the same name. It will add it as: file (1).txt etc.

This script runs in the background, checking for changes in the directory every minute.

You can change the target folder by changing this in the main function of the code:

```
// use custom folder path. Example:
// targetDirectory = L"D:\\Photos"; // Uncomment this line

targetDirectory = getDownloadsFolder(); \\ Comment or remove this line
```

## Building

### Terminal
- You can download the latest version of MinGW g++ from this link (https://www.msys2.org/)
```
g++ -std=c++17 -c main.cpp
g++ -std=c++17 -o AutoDownloadsOrganizer.exe main.o -mwindows -lole32 -lshell32 -luuid
```

### MS Build (Visual Studio)
- Make sure the specified C++ version is C++17
- Add the following libraries to the additional libraries: lole32, lshell32, luuid

### GNU Make

#### Prerequisites
- Install g++ on your device
- Install GNU Make on your device (https://www.gnu.org/software/make/)
- Clone the repository and run `make` in your terminal
- Run the file named 'AutoDownloadsOrganizer.exe'

## License

This project is licensed under the [MIT License](LICENSE).
