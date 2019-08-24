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

#include "screens/screen.hpp"
#include "utils/fileBrowse.h"

#include <vector>

extern "C" {
#include "C2D_helper.h"
}

class Image : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int ImageMode = 0;

	void DrawBrowse(void) const;
	void DrawViewer(void) const;

	void BrowseLogic(u32 hDown, u32 hHeld);
	void ViewerLogic(u32 hDown, u32 hHeld);

	void FreeImage(C2D_Image *image);
	bool Draw_Image(void) const;

	
	std::string currentImage = "";
	uint selectedFile = 0;
	int keyRepeatDelay = 0;
	mutable bool dirChanged = true;
	std::vector<DirEntry> dirContents;
	double imageScale = 1.0f;
	int positionX = 0, positionY = 0;
	ImageSize imageSize;
	std::string filename;
	C2D_Image image;
};