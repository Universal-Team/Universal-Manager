#ifndef FILE_BROWSE_H
#define FILE_BROWSE_H

// #include "download.hpp"
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <vector>

struct DirEntry {
	std::string name;
	std::string path;
	bool isDirectory;
	char tid[5];
	off_t size;
};

typedef struct {
	char gameTitle[12];			//!< 12 characters for the game title.
	char gameCode[4];			//!< 4 characters for the game code.
} sNDSHeadertitlecodeonly;

void findNdsFiles(std::vector<DirEntry>& dirContents);

void getDirectoryContents(std::vector<DirEntry>& dirContents);

#endif //FILE_BROWSE_H
