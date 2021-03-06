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

#ifndef FILEMANAGERSCREEN_HPP
#define FILEMANAGERSCREEN_HPP

#include "screens/screen.hpp"

class FileManager : public SCREEN
{
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;

private:
	mutable bool screenDrawn;
	int mode = 0;
	std::string selectedFile = "";
	int selectedOption = 0;

	void DrawBox(void) const;
	void boxLogic(u16 hDown);

	// Filemanager Operations.
	void CreateFolder(std::string FolderName);

	void CreateFile(std::string pathToFile);

	void RenameFile(std::string oldFileName, std::string newFileName);

	void DeleteFile(std::string pathToFile);
};

#endif