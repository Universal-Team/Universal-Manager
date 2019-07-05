#include "fileBrowse.h"
#include <3ds.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "gui.hpp"
#include "animation.hpp"

int file_count = 0;

extern bool continueFileScan;
extern uint selectedFile;
extern int keyRepeatDelay;
extern bool dirChanged;
extern std::vector<DirEntry> dirContents;
//extern std::string currentFile = "";
//extern std::string currentFiles;


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
		DisplayMsg("Unable to open the directory.");
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
			DisplayMsg(scanningMessage);
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
		DisplayMsg("Unable to open the directory.");
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

static void drawUnselectedBoxes(void) {
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 25, GRAY);
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 55, GRAY);
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 90, GRAY);
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 125, GRAY);
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 160, GRAY);
	Gui::Draw_ImageBlend(sprites_selected_idx, 0, 195, GRAY);
}

void drawFileBrowser(const char *text) {
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(120, 0, 0.68f, WHITE, text);
		if (dirChanged) {
            dirContents.clear();
            std::vector<DirEntry> dirContentsTemp;
            getDirectoryContents(dirContentsTemp);
            for(uint i=0;i<dirContentsTemp.size();i++) {
                    dirContents.push_back(dirContentsTemp[i]);
            }
		dirChanged = false;
	}
	std::string dirs;
	for (uint i=(selectedFile<5) ? 0 : selectedFile-5;i<dirContents.size()&&i<((selectedFile<5) ? 6 : selectedFile+1);i++) {
		(i == selectedFile);

		if (selectedFile == 0) {
			Gui::sprite(sprites_selected_idx, 0, 25);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 1) {
			Gui::sprite(sprites_selected_idx, 0, 55);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 2) {
			Gui::sprite(sprites_selected_idx, 0, 90);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 3) {
			Gui::sprite(sprites_selected_idx, 0, 125);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 4) {
			Gui::sprite(sprites_selected_idx, 0, 160);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 5) {
			Gui::sprite(sprites_selected_idx, 0, 195);
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			Gui::sprite(sprites_selected_idx, 0, 195);
			dirs += "  " + dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}
	Draw_Text(26, 27, 0.56f, WHITE, dirs.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}