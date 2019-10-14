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

#include "gui.hpp"

#include "screens/screenCommon.hpp"

#include "utils/download.hpp"
#include "utils/extract.hpp"
#include "utils/fileBrowse.h"
#include "utils/thread.hpp"

#include <3ds.h>
#include <algorithm>
#include <citro2d.h>
#include <fstream>
#include <unistd.h>
#include <vector>

extern "C" {
#include "cia.h"
}

// Progressbar stuff.
extern char progressBarMsg[128];
extern bool showProgressBar;
extern bool progressBarType; // 0 = Download | 1 = Extract

extern int filesExtracted;
extern std::string extractingFile;

struct Scpt {
	std::string function;
	std::string param1;
	std::string param2;
	std::string param3;
	std::string param4;
};

Scpt getScptFromLine(std::string line) {
	Scpt scpt;
	scpt.function = line.substr(0, line.find("	"));
	line = line.substr(line.find("	")+1);

	scpt.param1 = line.substr(0, std::min(line.find("	"), line.length()-1));
	line = line.substr(line.find("	")+1);

	scpt.param2 = line.substr(0, std::min(line.find("	"), line.length()-1));
	line = line.substr(line.find("	")+1);

	scpt.param3 = line.substr(0, std::min(line.find("	"), line.length()-1));
	line = line.substr(line.find("	")+1);

	scpt.param4 = line.substr(0, std::min(line.find("	"), line.length()-1));
	line = line.substr(line.find("	")+1);
	return scpt;
}

void runScript(std::string path) {
	std::ifstream in(path);
	if(in.good()) {
		std::string line;
		while(std::getline(in, line)) {
			Scpt scpt = getScptFromLine(line);

			if(scpt.function == "downloadRelease") {
				if (downloadFromRelease(scpt.param1, scpt.param2, scpt.param3) != 0) {
					downloadFailed();
					return;
				}
			}

			if(scpt.function == "downloadFile") {
				if (downloadToFile(scpt.param1, scpt.param2) != 0) {
					downloadFailed();
					return;
				}
			}

			if(scpt.function == "extract") {
				extractArchive(scpt.param1, scpt.param2, scpt.param3);
			}

			if(scpt.function == "mkdir") {
				mkdir(scpt.param1.c_str(), 0777);
			}

			if(scpt.function == "install") {
				installCia(scpt.param1.c_str());
			}

			if(scpt.function == "delete") {
				deleteFile(scpt.param1.c_str());
			}

			if(scpt.function == "msg") {
				DisplayMsg(scpt.param1.c_str());
			}

			// Download Functions with Progressbar!
			if(scpt.function == "progressDownloadRelease") {
				snprintf(progressBarMsg, sizeof(progressBarMsg), scpt.param4.c_str());
				showProgressBar = true;
				progressBarType = 0;
				Threads::create((ThreadFunc)displayProgressBar);
				if (downloadFromRelease(scpt.param1, scpt.param2, scpt.param3) != 0) {
					showProgressBar = false;
					downloadFailed();
					return;
				}
					showProgressBar = false;
			}

			if(scpt.function == "progressDownloadFile") {
				snprintf(progressBarMsg, sizeof(progressBarMsg), scpt.param3.c_str());
				showProgressBar = true;
				progressBarType = 0;
				Threads::create((ThreadFunc)displayProgressBar);
				if (downloadToFile(scpt.param1, scpt.param2) != 0) {
					showProgressBar = false;
					downloadFailed();
					return;
				}
					showProgressBar = false;
			}

			// The extract Function with a progressbar!
			if(scpt.function == "progressExtract") {
				snprintf(progressBarMsg, sizeof(progressBarMsg), scpt.param4.c_str());
				showProgressBar = true;
				filesExtracted = 0;
				progressBarType = 1;
				Threads::create((ThreadFunc)displayProgressBar);
				extractArchive(scpt.param1, scpt.param2, scpt.param3);
				showProgressBar = false;
			}
			// Displays a Message until A is pressed.
			//Param 1 -> Message.
			if(scpt.function == "waitMsg") {
				DisplayWaitMsg(scpt.param1.c_str());
			}

			// Displays a Message for 3 seconds.
			//Param 1 -> Message.
			if(scpt.function == "timeMsg") {
				int param2 = (int)(std::stoi(scpt.param2.c_str()));
				DisplayTimeMessage(scpt.param1.c_str(), param2);
			}

			// Launch a Title from the NAND or SD.
			if(scpt.function == "launch") {
				static bool isFound = false;
				FS_MediaType Media;
				u64 TitleID = std::stoull (scpt.param1, 0, 0);

				if (scpt.param2 == "NAND") {
					Media = MEDIATYPE_NAND;
				} else if (scpt.param2 == "SD") {
					Media = MEDIATYPE_SD;
				} else {
					Media = MEDIATYPE_SD;
				}

				// We will check, if this ID even exist, so it will not boot into it, if it does not exist.

				Result res = 0;
				u32 titleCount;

				res = AM_GetTitleCount(Media, &titleCount);
				if (R_FAILED(res))
				{
					return;
				}

				// get the Title List and check, if the ID is found.
				std::vector<u64> IDs;
				IDs.resize(titleCount);
				res	= AM_GetTitleList(nullptr, Media, titleCount, &IDs[0]);
				if (R_FAILED(res))
				{
					return;
				}

				DisplayTimeMessage("Checking, if Title Exists...", 2);

				if (std::find(IDs.begin(), IDs.end(), TitleID) != IDs.end())
				{
					DisplayTimeMessage("Title exist!", 2);
					isFound = true;
				} else {
					DisplayTimeMessage("Title does not exist!", 2);
				}

				if (isFound == true) {
					DisplayMsg("Booting Title... Please wait.");
					u8 param[0x300];
					u8 hmac[0x20];
					memset(param, 0, sizeof(param));
					memset(hmac, 0, sizeof(hmac));
					APT_PrepareToDoApplicationJump(0, TitleID, Media);
					APT_DoApplicationJump(param, sizeof(param), hmac);
				}
				isFound = false;
			}
		}
	}
}