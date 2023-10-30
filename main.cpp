#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <set>
#include <unordered_map>
#include <windows.h>
#include <shlobj.h>

// YOU HAVE TO COMPILE THIS PROGRAM WITH C++17 STANDARD: -std=c++17
// YOU HAVE TO INCLUDE THESE LIBRARIES WHEN COMPILING: -lole32 -lshell32 -luuid

namespace fs = std::filesystem;

// The use of wide strings is necessary for the program to work with non-ASCII characters

fs::path targetDirectory;

std::set<fs::path> previousFiles;
std::set<fs::path> currentFiles;
std::set<std::wstring> myExtensions;
std::unordered_map<std::wstring, std::wstring> myMap;

bool detectChange(const fs::path &path)
{
    currentFiles.clear();

    for (const auto &entry : fs::directory_iterator(path))
        if (entry.is_regular_file())
            currentFiles.insert(entry.path());

    if (previousFiles.empty())
    {
        previousFiles = currentFiles;
        return true;
    }

    if (currentFiles == previousFiles)
        return false;

    myExtensions.clear();
    previousFiles = currentFiles;
    return true;
}

bool createFolder(const std::wstring &path)
{
    if (fs::create_directory(path))
        return true;

    return false;
}

void toUpperCase(std::wstring &s)
{
    for (int i = 0; i < s.size(); i++)
        s[i] = std::towupper(s[i]);
}

void collectingExtensions()
{
    try
    {
        const fs::path p{targetDirectory};
        for (auto const &dir_entry : fs::directory_iterator{p})
        {
            if (!dir_entry.path().has_extension())
                continue;

            if (!dir_entry.is_regular_file())
                continue;

            try
            {
                myExtensions.insert(dir_entry.path().extension().wstring()); // Use wide strings for extensions
            }
            catch (const std::exception &e)
            {
                std::wcout << L"Error collecting extensions: " << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::wcout << L"Error in collectingExtensions: " << e.what() << std::endl;
    }
}

void createFolderNames()
{
    for (std::set<std::wstring>::iterator iter = myExtensions.begin(); iter != myExtensions.end(); iter++)
    {
        std::wstring extension = *iter;
        extension = extension.substr(1);
        toUpperCase(extension);

        std::wstring folderName = extension + L"s";

        createFolder(targetDirectory / fs::path(folderName));
        myMap.insert({*iter, folderName});
    }
}

bool moveToFile(const fs::path &oldpath, const fs::path &newpath)
{
    try
    {
        // If the file already exists in the destination folder, rename it
        // It is renamed as file (1) or file (2) etc.

        int i = 0;
        fs::path path = newpath;
        while (fs::exists(path))
        {
            i++;
            std::wstring newFilename = newpath.filename().wstring();
            std::wstring extension = newFilename.substr(newFilename.find_last_of(L"."), newFilename.size());

            newFilename = newFilename.substr(0, newFilename.find_last_of(L"."));
            newFilename = newFilename + L" (" + std::to_wstring(i) + L")" + extension;
            path = newpath.parent_path() / fs::path(newFilename);
        }

        fs::copy(oldpath, path);
        fs::remove(oldpath);
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::wcerr << L"Error moving the file: " << e.what() << std::endl;
        return false;
    }
}

void moveFiles()
{
    try
    {
        const fs::path p{targetDirectory};
        for (auto const &currentFile : fs::directory_iterator{p})
        {
            if (!currentFile.path().has_extension())
                continue;

            if (!currentFile.is_regular_file())
                continue;

            try
            {
                std::wstring extension = currentFile.path().extension().wstring(); // Use wide strings for extensions
                std::wstring filename = currentFile.path().filename().wstring();   // Use wide strings for filenames
                std::wstring folderName = myMap.at(extension);
                std::wstring movePath = targetDirectory / fs::path(folderName) / fs::path(filename); // Use wide strings for paths
                std::wstring oldPath = targetDirectory / fs::path(filename);

                if (!moveToFile(oldPath, movePath))
                    std::wcout << L"\nFailed to move file: " << filename << std::endl;
            }
            catch (const std::exception &e)
            {
                std::wcout << L"Error moving a file: " << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::wcout << L"Error in moveFiles: " << e.what() << std::endl;
    }
}

std::wstring getDownloadsFolder()
{
    PWSTR downloadsPath;
    if (SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &downloadsPath) == S_OK)
    {
        std::wstring downloadsFolder(downloadsPath);
        CoTaskMemFree(downloadsPath);
        return downloadsFolder;
        // std::wcout << L"Downloads folder path: " << downloadsFolder << std::endl;
    }
    else
    {
        std::cerr << "Failed to retrieve Downloads folder path." << std::endl;
        return NULL;
    }
}

int main()
{
    // use custom folder path. Example:
    // targetDirectory = L"D:\\OneDrive\\Downloads";

    // Detect the Downloads folder path
    // You have to include these libraries when compiling
    // -lole32 -lshell32 -luuid
    targetDirectory = getDownloadsFolder();

    int sleepTime = 1;

    while (true)
    {
        if (!detectChange(targetDirectory))
        {
            std::this_thread::sleep_for(std::chrono::minutes(sleepTime)); // No change detected, sleep
            continue;
        }

        collectingExtensions();
        createFolderNames();
        moveFiles();

        std::this_thread::sleep_for(std::chrono::minutes(sleepTime));
    }
}
