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

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

C3D_RenderTarget* Top;
C3D_RenderTarget* Bottom;

bool exiting = false;

touchPosition touch;

int main()
{
	// Init everything.
	gfxInitDefault();
	romfsInit();
	sdmcInit();
	cfguInit();

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// Loop as long as the status is not exit
	while (aptMainLoop() && !exiting)
	{
		hidScanInput();
		u32 hHeld = hidKeysHeld();
		u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 255));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 255));

		// Draw Blue Screen haha.
		C2D_SceneBegin(Top);
		C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, C2D_Color32(0, 0, 255, 255));
		C2D_SceneBegin(Bottom);
		C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, C2D_Color32(0, 0, 255, 255));

		C3D_FrameEnd(0);

		if (hDown & KEY_START) {
			exiting = true;
		}
	}
	// Exit everything.
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	cfguExit();
	romfsExit();
	sdmcExit();
	return 0;
}