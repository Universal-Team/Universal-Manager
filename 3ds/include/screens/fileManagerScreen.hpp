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
#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "screens/screen.hpp"
#include "utils/extract.hpp"
#include "utils/fileBrowse.h"
#include "utils/fileOperations.h"
#include "utils/keyboard.hpp"
#include "utils/settings.hpp"
#include "structs.hpp"

#include <array>
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>

extern "C" {
#include "C2D_helper.h"
#include "cia.h"
}

class FileManager : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	bool refresh = false;
	bool updatingSelf = false;
	uint selectedFile = 0;
	int keyRepeatDelay = 0;
	mutable bool dirChanged = true;
	std::vector<DirEntry> dirContents;
	std::string currentFile = "";
	std::string currentFiles;

	// Operations.
	DirEntry clipboard;
	void renameFile(void);
	void deleteFile(void);
	void copyPaste(void);
	void createFolder(void);
	void extractarchive(void);
	void install(void);
	bool displayActionBox(void);

	std::array<Structs::TextBtn, 6> functionPos = {{
		{59, 70, 93, 35, "Rename"},
		{165, 70, 93, 35, "Delete"},
		{59, 110, 93, 35, "Copy/Paste"},
		{165, 110, 93, 35, "Create"},
		{59, 150, 93, 35, "Extract"},
		{165, 150, 93, 35, "Install"}
	}
	};
};

#endif