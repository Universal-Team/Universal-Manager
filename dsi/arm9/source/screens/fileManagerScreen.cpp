/*
*   This file is part of Universal-Manager-DSi
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

#include "screens/fileManagerScreen.hpp"
#include "screens/screenCommon.hpp"

#include "utils/config.h"
#include "utils/scripts.hpp"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <unistd.h>

void FileManager::Draw(void) const
{
	if (screenDrawn) return;

	Gui::DrawBGTop();
	Gui::DrawBarsTop();

	Gui::DrawBGBot();
	Gui::DrawBarsBot();

	printTextCenteredScaled("FileManager Sub Menu", 1.4, 1.4, 0, 30, true);

	// Battery Icon.
	drawImage(217, 0, batteryChargeData.width, batteryChargeData.height, batteryCharge, true);

	screenDrawn = true;
}

void FileManager::DrawBrowse(void) const {

}


void FileManager::Logic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenDrawn = false;
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_Y) {
		runScript("/_nds/Universal-Manager/scripts/Test.scpt");
	}
}

// Filemanager operations.

// Creates a Folder. (The Path must begin with 'sd:/' or 'fat:/'.)

void FileManager::CreateFolder(std::string FolderName) {
    mkdir(FolderName.c_str(), 0777);
}

// Creates a File. (The Path must begin with 'sd:/' or 'fat:/' and it must contain the FileName.)

void FileManager::CreateFile(std::string pathToFile) {
    std::ofstream file { pathToFile.c_str() };
}

// Renames a File. (The Path must begin with 'sd:/' or 'fat:/' and it must contain the FileNames.)

void FileManager::RenameFile(std::string oldFileName, std::string newFileName) {
    rename(oldFileName.c_str(), newFileName.c_str());
}

void FileManager::DeleteFile(std::string pathToFile) {
	remove(pathToFile.c_str());
}