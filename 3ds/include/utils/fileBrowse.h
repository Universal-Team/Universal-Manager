#ifndef FILE_BROWSE_H
#define FILE_BROWSE_H

#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>

struct DirEntry {
	std::string name;
	std::string path;
	bool isDirectory;
	off_t size;
};

void getDirectoryContents(std::vector<DirEntry>& dirContents);

void drawFileBrowser();

#endif //FILE_BROWSE_H
