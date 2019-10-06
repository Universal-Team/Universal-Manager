/*-----------------------------------------------------------------
 Copyright(C) 2005 - 2017
	Michael "Chishm" Chisholm
	Dave "WinterMute" Murphy
	Claudio "sverx"

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or(at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------------------------------------------------------------------*/

#include "colors.h"
#include "fileBrowse.h"

#include "screenCommon.hpp"

#include <algorithm>
#include <dirent.h>
#include <strings.h>
#include <unistd.h>

#include "graphics.h"

#define ENTRIES_PER_SCREEN 11
#define ENTRY_PAGE_LENGTH 10
#define copyBufSize 0x8000

char path[PATH_MAX];
char fatLabel[12];
char sdLabel[12];
u32 copyBuf[copyBufSize];

bool nameEndsWith(const std::string& name, const std::vector<std::string> extensionList) {
	if(name.substr(0, 2) == "._") return false;

	if(name.size() == 0) return false;

	if(extensionList.size() == 0) return true;

	for(int i = 0; i <(int)extensionList.size(); i++) {
		const std::string ext = extensionList.at(i);
		if(strcasecmp(name.c_str() + name.size() - ext.size(), ext.c_str()) == 0) return true;
	}
	return false;
}

bool dirEntryPredicate(const DirEntry& lhs, const DirEntry& rhs) {
	if(!lhs.isDirectory && rhs.isDirectory) {
		return false;
	}
	if(lhs.isDirectory && !rhs.isDirectory) {
		return true;
	}
	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void getDirectoryContents(std::vector<DirEntry>& dirContents, const std::vector<std::string> extensionList) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir(".");

	if(pdir == NULL) {
		printText("Unable to open the directory.", 0, 0, false);
	} else {
		while(true) {
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if(pent == NULL)	break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;

			if(dirEntry.name.compare(".") != 0 && (dirEntry.isDirectory || nameEndsWith(dirEntry.name, extensionList))) {
				dirContents.push_back(dirEntry);
			}
		}
		closedir(pdir);
	}
	sort(dirContents.begin(), dirContents.end(), dirEntryPredicate);
}

void getDirectoryContents(std::vector<DirEntry>& dirContents) {
	getDirectoryContents(dirContents, {});
}

void showDirectoryContents(const std::vector<DirEntry>& dirContents, int startRow) {
	getcwd(path, PATH_MAX);

	// Print path
	drawImageFromSheet(5, 0, 250, 16, fileBrowse, fileBrowseData.width, 5, 0, false);
	printTextMaxW(path, 250, 1, 5, 0, false);

	// Print directory listing
	for(int i=0;i < ENTRIES_PER_SCREEN; i++) {
		// Clear row
		drawImageFromSheet(10, i*16+16, 246, 16, fileBrowse, fileBrowseData.width, 10, i*16+16, false);

		if(i < ((int)dirContents.size() - startRow)) {
			std::u16string name = StringUtils::UTF8toUTF16(dirContents[i + startRow].name);

			// Trim to fit on screen
			bool addEllipsis = false;
			while(getTextWidth(name) > 227) {
				name = name.substr(0, name.length()-1);
				addEllipsis = true;
			}
			if(addEllipsis)	name += StringUtils::UTF8toUTF16("...");

			printTextTinted(name, GRAY, 10, i*16+16, false, true);
		}
	}
}

bool showTopMenuOnExit = true, noCardMessageSet = false;
int tmCurPos = 0, tmScreenOffset = 0, tmSlot1Offset = 0;

std::string browseForFile(const std::vector<std::string>& extensionList, bool directoryNavigation) {
	int pressed = 0, held = 0, screenOffset = 0, fileOffset = 0;
	touchPosition touch;
	bool bigJump = false;
	std::vector<DirEntry> dirContents;

	getDirectoryContents(dirContents, extensionList);
	showDirectoryContents(dirContents, screenOffset);

	while(1) {
		// Clear old cursors
		drawImageFromSheet(0, 17, 10, 175, fileBrowse, fileBrowseData.width, 0, 17, false);

		// Draw cursor
		drawRectangle(3, (fileOffset-screenOffset)*16+24, 4, 3, DARK_GRAY, false);


		// Power saving loop. Only poll the keys once per frame and sleep the CPU if there is nothing else to do
		do {
			swiWaitForVBlank();
			scanKeys();
			pressed = keysDown();
			held = keysDownRepeat();
		} while(!held);

		if(held & KEY_UP)	fileOffset -= 1;
		else if(held & KEY_DOWN)	fileOffset += 1;
		else if(held & KEY_LEFT) {
			fileOffset -= ENTRY_PAGE_LENGTH;
			bigJump = true;
		} else if(held & KEY_RIGHT) {
			fileOffset += ENTRY_PAGE_LENGTH;
			bigJump = true;
		}

		if(fileOffset < 0) {
			// Wrap around to bottom of list unless left was pressed
			fileOffset = bigJump ? 0 : dirContents.size()-1;
			bigJump = true;
		} else if(fileOffset > ((int)dirContents.size()-1)) {
			// Wrap around to top of list unless right was pressed
			fileOffset = bigJump ? dirContents.size()-1 : 0;
			bigJump = true;
		} else if(pressed & KEY_A) {
			selection:
			DirEntry* entry = &dirContents.at(fileOffset);
			if(entry->isDirectory && directoryNavigation) {
				// Enter selected directory
				chdir(entry->name.c_str());
				getDirectoryContents(dirContents, extensionList);
				screenOffset = 0;
				fileOffset = 0;
				showDirectoryContents(dirContents, screenOffset);
			} else if(!entry->isDirectory) {
				// Return the chosen file
				return entry->name;
			}
		} else if(pressed & KEY_B && directoryNavigation) {
			// Go up a directory
			if((strcmp (path, "sd:/") == 0) || (strcmp (path, "fat:/") == 0)) {
			} else {
				chdir("..");
			}
			getDirectoryContents(dirContents, extensionList);
			screenOffset = 0;
			fileOffset = 0;
			showDirectoryContents(dirContents, screenOffset);
		} else if(pressed & KEY_B && !directoryNavigation) {
			return "";
		} else if(pressed & KEY_Y && !dirContents[fileOffset].isDirectory && directoryNavigation) {
		} else if(pressed & KEY_TOUCH) {
			touchRead(&touch);
			for(int i=0;i<std::min(ENTRIES_PER_SCREEN, (int)dirContents.size());i++) {
				if(touch.py > (i+1)*16 && touch.py < (i+2)*16) {
					fileOffset = i;
					goto selection;
				}
			}
		}

		// Scroll screen if needed
		if(fileOffset < screenOffset) {
			screenOffset = fileOffset;
			showDirectoryContents(dirContents, screenOffset);
		} else if(fileOffset > screenOffset + ENTRIES_PER_SCREEN - 1) {
			screenOffset = fileOffset - ENTRIES_PER_SCREEN + 1;
			showDirectoryContents(dirContents, screenOffset);
		}
		bigJump = false;
	}
}

std::string browseForSave(void) {
	// Clear screens
	drawImage(0, 0, fileBrowseData.width, fileBrowseData.height, fileBrowse, false);

	std::vector<std::string> extensionList;
	extensionList.push_back("sav");
	char sav[6];
	for(int i=1;i<10;i++) {
		snprintf(sav, sizeof(sav), "sav%i", i);
		extensionList.push_back(sav);
	}
	return browseForFile(extensionList, true);
}

int fcopy(const char *sourcePath, const char *destinationPath) {
	DIR *isDir = opendir(sourcePath);

	if(isDir == NULL) {
		closedir(isDir);

		// Source path is a file
		FILE* sourceFile = fopen(sourcePath, "rb");
		off_t fsize = 0;
		if(sourceFile) {
			fseek(sourceFile, 0, SEEK_END);
			fsize = ftell(sourceFile);			// Get source file's size
			fseek(sourceFile, 0, SEEK_SET);
		} else {
			fclose(sourceFile);
			return -1;
		}

		FILE* destinationFile = fopen(destinationPath, "wb");
			fseek(destinationFile, 0, SEEK_SET);

		off_t offset = 0;
		int numr;
		while(1) {
			drawRectangle(((offset < fsize ? (double)offset/fsize : 1)*227)+5, 33, 19, 16, DARK_GRAY, false);
			// Copy file to destination path
			numr = fread(copyBuf, 2, copyBufSize, sourceFile);
			fwrite(copyBuf, 2, numr, destinationFile);
			offset += copyBufSize;

			if(offset > fsize) {
				fclose(sourceFile);
				fclose(destinationFile);

				return 1;
				break;
			}
		}

		return -1;
	} else {
		closedir(isDir);
		return -2;
	}
}
