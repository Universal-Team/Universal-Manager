/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "fileBrowse.h"
#include <3ds.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

int file_count = 0;

extern bool continueFileScan;

extern void displayMsg(const char* text);

/**
 * Get the title ID.
 * @param ndsFile DS ROM image.
 * @param buf Output buffer for title ID. (Must be at least 4 characters.)
 * @return 0 on success; non-zero on error.
 */
int grabTID(FILE *ndsFile, char *buf)
{
	fseek(ndsFile, offsetof(sNDSHeadertitlecodeonly, gameCode), SEEK_SET);
	size_t read = fread(buf, 1, 4, ndsFile);
	return !(read == 4);
}

void findFiles(std::vector<DirEntry>& dirContents, std::vector<std::string> fileTypes)
{	
	struct stat st;
	DIR *pdir = opendir(".");

	if (pdir == NULL)
	{
		displayMsg("Unable to open the directory.");
		for(int i=0;i<120;i++)
			gspWaitForVBlank();
	}
	else
	{
		while (continueFileScan)
		{
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if (pent == NULL) break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			char scanningMessage[512];
			snprintf(scanningMessage, sizeof(scanningMessage), "Scanning SD card for files...\n\n(Press B to cancel)\n\n\n\n\n\n\n\n\n%s", dirEntry.name.c_str());
			displayMsg(scanningMessage);
			dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;
				if(!(dirEntry.isDirectory) && dirEntry.name.length() >= 3) {
					for(uint i=0;i<fileTypes.size();i++) {
						if (strcasecmp(dirEntry.name.substr(dirEntry.name.length()-3, 3).c_str(), fileTypes[i].c_str()) == 0) {
							if(strcasecmp(fileTypes[i].c_str(), "nds") == 0) {
								// Get game's TID
								FILE *f_nds_file = fopen(dirEntry.name.c_str(), "rb");
								// char game_TID[5];
								grabTID(f_nds_file, dirEntry.tid);
								dirEntry.tid[4] = 0;
								fclose(f_nds_file);
							}

							dirContents.push_back(dirEntry);
							file_count++;
						}
					}
				} else if (dirEntry.isDirectory
				&& dirEntry.name.compare(".") != 0
				&& dirEntry.name.compare("_nds") != 0
				&& dirEntry.name.compare("3ds") != 0
				&& dirEntry.name.compare("DCIM") != 0
				&& dirEntry.name.compare("gm9") != 0
				&& dirEntry.name.compare("luma") != 0
				&& dirEntry.name.compare("Nintendo 3DS") != 0
				&& dirEntry.name.compare("private") != 0
				&& dirEntry.name.compare("retroarch") != 0) {
					chdir(dirEntry.name.c_str());
					findNdsFiles(dirContents);
					chdir("..");
				}
		}
		closedir(pdir);
	}
}

off_t getFileSize(const char *fileName)
{
	FILE* fp = fopen(fileName, "rb");
	off_t fsize = 0;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);			// Get source file's size
		fseek(fp, 0, SEEK_SET);
	}
	fclose(fp);

	return fsize;
}

void getDirectoryContents(std::vector<DirEntry>& dirContents) {
	struct stat st;

	dirContents.clear();

	DIR *pdir = opendir ("."); 
	
	if (pdir == NULL) {
		displayMsg("Unable to open the directory.");
		for(int i=0;i<120;i++)
			gspWaitForVBlank();
	} else {

		while(true) {
			DirEntry dirEntry;

			struct dirent* pent = readdir(pdir);
			if(pent == NULL) break;

			stat(pent->d_name, &st);
			if (strcmp(pent->d_name, "..") != 0) {
				dirEntry.name = pent->d_name;
				dirEntry.isDirectory = (st.st_mode & S_IFDIR) ? true : false;
				if (!dirEntry.isDirectory) {
					dirEntry.size = getFileSize(dirEntry.name.c_str());
				}

				if (dirEntry.name.compare(".") != 0) {
					dirContents.push_back (dirEntry);
				}
			}

		}
		
		closedir(pdir);
	}	
}
