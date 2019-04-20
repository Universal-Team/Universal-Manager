/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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
#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "datetime.h"
#include "download.hpp"
#include "dumpdsp.h"
#include "graphic.h"
#include "inifile.h"
#include "keyboard.h"
#include "music.hpp"
#include "settings.h"
#include "Textures.hpp"
#include "thread.hpp"
#include "Universal-Settings.hpp"
#include "voltlib/volt.h"


#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

static touchPosition touch;

bool dspfirmfound = false;
bool updatingSelf = false;
bool showSettings = false;

std::string musicNames[] = {"Chill", "Settings", "SD", "OFF"};

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {0.0f, 0.0f};	
struct {
	int x;
	int y;
} buttons2[] = {
	{ 129, 48},
	{ 220, 48},
	{ 129, 88},
	{ 220, 88},
	{ 129, 128},
	{ 220, 128},
	{ 129, 168},
	{ 220, 168},
	{ 129, 48},
	{ 220, 48},
	{ 129, 88},
	{ 220, 88},
	{ 129, 128},
	{ 220, 128},
	{ 129, 168},
	{ 220, 168},
	{ 129, 48},
	{ 220, 48},
	{ 129, 88},
	{ 220, 88},
};

size_t button_tex2[] = {
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
	button,
};

const char *button_titles2[] = {
	"Release",
	"Nightly",
	"Release",
	"Nightly",
	"Release",
	"Nightly",	
	"Release",
	"Nightly",
	"Boxart",
	"Cheats",
	"Release",
	"Nightly",
	"Release",
	"Nightly",
	"Release",
	"Nightly",
	"  FBI",
	"     - ",
	"     - ",
	"     - ",
};

const int title_spacing2[] = {
	6,
	10,
	6,
	10,
	6,
	10,
	6,
	10,
	10,
	10,
	6,
	6,
	10,
	10,
	6,
	10,
	10,
	10,
	6,
	10,
};

const char *row_titles2[] = {
	"TWL Menu++",
	"nds-bootstrap",
	"PKSM",
	"Luma3DS",
	"Extras",
	"Checkpoint",
	"Updater",
	"GodMode9",
	"FBI",
	"/",
	"/",
	"/",
};	

bool updateAvailable[] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};

struct {
	int x;
	int y;
	int w;
	int h;
} buttons_settings[] = {
	{ 130, 32, 48, 32},
	{ 190, 32, 48, 32},
	{ 250, 32, 48, 32},
	{ 130, 80, 48, 32},
	{ 190, 80, 48, 32},
	{ 250, 80, 48, 32},
	{ 130, 128, 48, 32},
	{ 190, 128, 48, 32},
	{ 250, 128, 48, 32},
	{ 147, 176, 134, 32},
};

size_t buttons_settings_tex[] = {
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	buttonRGB,
	settingsButton,
};

int button_settings_blend[] = {
	RED,
	GREEN,
	BLUE,
	RED,
	GREEN,
	BLUE,
	RED,
	GREEN,
	BLUE,
	WHITE,
};

const char *button_titles_settings[] = {
};

const int title_spacing_settings[] = {
	6,
	10,
};

void screenoff()
{
    gspLcdInit();\
    GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}

void screenon()
{
    gspLcdInit();\
    GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}

void currentMusic(void) {
	if (settings.universal.music == 0) {
		Music_Chill();
	} else if (settings.universal.music == 1) {
		Music_Settings();
	} else if (settings.universal.music == 2) {
		Music_SD();
	} else if (settings.universal.music == 3) {

	}
}

void draw_Bars_Bottom(void) {
	volt_draw_rectangle(0, 0, 320, 19, settings.universal.bars);
	volt_draw_rectangle(0, 221, 320, 19, settings.universal.bars);
}

void draw_Bars_Top(void) {
	volt_draw_rectangle(0, 0, 400, 19, settings.universal.bars);
	volt_draw_rectangle(0, 221, 400, 19, settings.universal.bars);
}

void draw_Background_Bottom(void) {
	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
    volt_draw_rectangle(0, 0, 320, 240, settings.universal.bg);
}

void draw_Background_Top(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
    volt_draw_rectangle(0, 0, 400, 240, settings.universal.bg);
}

void draw_Dialogbox_Color(void) {
	volt_draw_rectangle(0, 19, 400, 202, settings.universal.bg);
}

void displayTopMsg(const char* text) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	draw_Dialogbox_Color();
		volt_draw_text(24, 32, 0.5f, 0.5f, settings.universal.text, text);
	volt_end_draw();
}

int getColorValue(int color, int bgr) {
	char colorName[10];
	int i;
	std::stringstream ss;

	itoa(color, colorName, 16);
	std::string colorNamePart(colorName, 2*bgr+2, 2);
	ss << std::hex << colorNamePart.c_str();
	ss >> i;

	return i;
}

std::string getColorName(int color, int bgr) {
	char colorName[10];
	int i = getColorValue(color, bgr);
	itoa(i, colorName, 10);
	return colorName;
}

int menuSelection = 0;
int menuPage = 0;

int main()
{
	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();
	acInit();
	SDLH_Init_SD();
	SDLH_Init_Chill();
	SDLH_Init_Settings();

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Updater", 0777); //main Path.
	mkdir("sdmc:/Universal-Updater/Music", 0777); //MP3 Path.

	volt_init();

	volt_set_3D(1);
	LoadUniversalSettings();

	volt_load_texture_png(button, "romfs:/graphics/Misc/Button.png");
	volt_load_texture_png(dot, "romfs:/graphics/Misc/Dot.png");
	volt_load_texture_png(settingsButton, "romfs:/graphics/Misc/SettingsButton.png");
	volt_load_texture_png(buttonRGB, "romfs:/graphics/Grayscaled/buttonRGB.png");
	volt_load_texture_png(pageframe, "romfs:/graphics/Misc/Page_Number_Frame.png");
	volt_load_texture_png(settingsIcon, "romfs:/graphics/Misc/Settings_Icon.png");
		
 	if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
		ndspInit();
		dspfirmfound = true;
	} else {
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		volt_end_draw();
		dumpDsp();
		if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
			ndspInit();
			dspfirmfound = true;
		} else {
			volt_begin_draw(GFX_TOP, GFX_LEFT);
			volt_draw_rectangle(0, 0, 400, 240, BLACK);
			volt_draw_on(GFX_BOTTOM, GFX_LEFT);
			volt_draw_rectangle(0, 0, 320, 240, BLACK);
			volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "DSP firm dumping failed.\n"
					"Running without sound.");
			volt_end_draw();
		}
	}

	bool buttonShading = false;
	bool setOption = false;
	bool showMessage = false;

	int fadealpha = 255;
	bool fadein = true;

	checkForUpdates();
	// Loop as long as the status is not exit
	Threads::create((ThreadFunc)currentMusic);
	while(aptMainLoop()) {
		offset3D[0].topbg = CONFIG_3D_SLIDERSTATE * -7.0f;
		offset3D[1].topbg = CONFIG_3D_SLIDERSTATE * 7.0f;

		// Scan hid shared memory for input events
		hidScanInput();
		
		const u32 hDown = hidKeysDown();

		hidTouchRead(&touch);

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) volt_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else volt_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			//volt_draw_texture(topbgtex, offset3D[topfb].topbg, 0);
			if (fadealpha > 0) volt_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		}

		draw_Background_Top();
		draw_Bars_Top();
		volt_draw_text(4, 3, 0.5f, 0.5f, settings.universal.text, getTime().c_str());
    	volt_draw_text(350, 3, 0.5f, 0.5f, settings.universal.text, "v2.0.0");
    	volt_draw_text(140, 3, 0.5f, 0.5f, settings.universal.text, "Universal-Updater");
    	volt_draw_text(260, 225, 0.5f, 0.5f, settings.universal.text, "\uE004 / \uE005: Switch Page");
    	volt_draw_text(1, 225, 0.5f, 0.5f, settings.universal.text, "\uE000: Select an Option");
		draw_Background_Bottom();
		draw_Bars_Bottom();
		volt_draw_texture(settingsIcon, 301, 221); // Draw the settings icon

		if (showSettings) {
			// Draw buttons
			for (int i = (int)((sizeof(buttons_settings)/sizeof(buttons_settings[0])))-1; i >= 0; i--) {
				if (menuSelection == i) {
					// Button is highlighted.
					volt_draw_texture_blend(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y, button_settings_blend[i]);
				} else {
					// Button is not highlighted. Darken the texture.
					if (buttonShading) {
						volt_draw_texture_blend(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y, GRAY & button_settings_blend[i]);
					} else {
						volt_draw_texture_blend(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y, button_settings_blend[i]);
					}
				}

				// Draw the Settings Options!
				volt_draw_text(20, 42, 0.65, 0.65, settings.universal.text, "Background");
				volt_draw_text(81, 90, 0.65, 0.65, settings.universal.text, "Bars");
				volt_draw_text(78, 138, 0.65, 0.65, settings.universal.text, "Text");
				volt_draw_text(68, 186, 0.65, 0.65, settings.universal.text, "Music");
			}

			volt_draw_text(138, 40, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bg, 2).c_str());
			volt_draw_text(198, 40, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bg, 1).c_str());
			volt_draw_text(258, 40, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bg, 0).c_str());
			volt_draw_text(138, 88, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bars, 2).c_str());
			volt_draw_text(198, 88, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bars, 1).c_str());
			volt_draw_text(258, 88, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.bars, 0).c_str());
			volt_draw_text(167, 184, 0.65, 0.65, settings.universal.text, musicNames[settings.universal.music].c_str());
			volt_draw_text(138, 136, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.text, 2).c_str());
			volt_draw_text(198, 136, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.text, 1).c_str());
			volt_draw_text(258, 136, 0.65, 0.65, settings.universal.text, getColorName(settings.universal.text, 0).c_str());

			volt_end_draw();
		} else {
			volt_draw_texture(pageframe, 256+(menuPage*20), 3); //Draw the Page Frame Texture.
    		volt_draw_text(170, 4, 0.50, 0.50, settings.universal.text, "Current Page:");
			volt_draw_text(260, 4, 0.50, 0.50, settings.universal.text, "1"); //Draw First Page Number.
			volt_draw_text(280, 4, 0.50, 0.50, settings.universal.text, "2"); // Draw Second Page Number.
			volt_draw_text(300, 4, 0.50, 0.50, settings.universal.text, "3"); // Draw Third Page Number.
			// Draw buttons
			for (int i = (int)((sizeof(buttons2)/sizeof(buttons2[0])))-1; i >= 0; i--) {
				if (i <= ((ceil(((double)menuSelection+1)/8)*8)-1) && i >= ((ceil(((double)menuSelection+1)/8)*8)-8)) {
					if (menuSelection == i) {
						// Button is highlighted.
						volt_draw_texture(button_tex2[i], buttons2[i].x, buttons2[i].y);
					} else {
						// Button is not highlighted. Darken the texture.
						if (buttonShading) {
							volt_draw_texture_blend(button_tex2[i], buttons2[i].x, buttons2[i].y, GRAY);
						} else {
							volt_draw_texture(button_tex2[i], buttons2[i].x, buttons2[i].y);
						}
					}
					// Draw a dot if an update is availible
					if (updateAvailable[i]) {
						volt_draw_texture(dot, buttons2[i].x+75, buttons2[i].y-6);
					}

					// Determine the text height.
					// NOTE: Button texture size is 132x34.
					const int h = 30;

					// Draw the title.
					int y = buttons2[i].y + ((40 - h) / 2);
					int x_from_width = buttons2[i].x + title_spacing2[i];
					volt_draw_text(x_from_width, y, 0.75, 0.75, settings.universal.text, button_titles2[i]);

					if (!(i%2)) {
						volt_draw_text(5, y, 0.7, 0.7, settings.universal.text, row_titles2[i/2]);
					}
				}
			}
			volt_end_draw();
		}
		
		if (fadein == true) {
			fadealpha -= 15;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}

		if (hDown & KEY_R && !showSettings) {
			if (menuPage<(ceil(((double)(sizeof(buttons2)/sizeof(buttons2[0])+1.0)/8))-1)) {
				menuPage++;
				menuSelection += 8;
				if (menuSelection > (int)sizeof(buttons2))	menuSelection = (int)sizeof(buttons2);
			}
		} else if (hDown & KEY_L) {
			if (menuPage>0) {
				menuPage--;
				menuSelection -= 8;
				if (menuSelection < 0)	menuSelection = 0;
			}
		}

		if (hDown & KEY_UP) {
			if (buttonShading) menuSelection -= (showSettings ? 3 : 2);
		} else if (hDown & KEY_DOWN) {
			if (buttonShading) menuSelection += (showSettings ? 3 : 2);
		} else if (hDown & KEY_LEFT) {
			if (buttonShading && (showSettings ? menuSelection%3 : menuSelection%2)) menuSelection--;
		} else if (hDown & KEY_RIGHT) {
			if (buttonShading && (showSettings ? (menuSelection+1)%3 : !(menuSelection%2))) menuSelection++;
		}
		if (hDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) {
			buttonShading = true;
			if (dspfirmfound) {
			}
		}

		if (hDown & KEY_TOUCH) {
			buttonShading = false;
		}
		if (showSettings) {
			if (menuSelection > 11) menuSelection = 0;
			else if (menuSelection > 9) menuSelection = 9;
			else if (menuSelection < 0) menuSelection = 9;
		} else {
			if ((menuSelection > (menuPage*8)+8) || (menuSelection > (int)(sizeof(buttons2)/sizeof(buttons2[0])))) {
				menuSelection = (menuPage*8)+1; 
			} else if ((menuSelection > (menuPage*8)+7) || (menuSelection > (int)((sizeof(buttons2)/sizeof(buttons2[0]))-1))) {
				menuSelection = (menuPage*8); 
			} else if (menuSelection < (menuPage*8)-1) {
				menuSelection = ((menuPage*8)+6 < (int)(sizeof(buttons2)/sizeof(buttons2[0]))-2) ? (menuPage*8)+6 : sizeof(buttons2)/sizeof(buttons2[0])-2;
			} else if (menuSelection < (menuPage*8)) {
				menuSelection = ((menuPage*8)+7 < (int)(sizeof(buttons2)/sizeof(buttons2[0]))-1) ? (menuPage*8)+7 : sizeof(buttons2)/sizeof(buttons2[0])-1;
			}
		}

		if (hDown & KEY_A) {
			setOption = true;
		} else if (hDown & KEY_SELECT) {
			menuSelection = 0;
			showSettings = !showSettings;
		}

		if (hDown & KEY_TOUCH) {
			if (touch.py >= 221 && touch.py <= 240 && touch.px >= 301 && touch.px <= 320) {
				menuSelection = 0;
				showSettings = !showSettings;
			} else if (showSettings) {
				if (touch.py >= 48 && touch.py <= 81 && touch.px >= 100 && touch.px <= 235) {
					menuSelection = 0;
					setOption = true;
				} else if (touch.py >= 98 && touch.py <= 130 && touch.px >= 100 && touch.px <= 235) {
					menuSelection = 1;
					setOption = true;
				} else if (touch.py >= 148 && touch.py <= 181 && touch.px >= 100 && touch.px <= 235) {
					menuSelection = 2;
					setOption = true;
				} else if (touch.py >= 198 && touch.py <= 231 && touch.px >= 100 && touch.px <= 235) {
					menuSelection = 3;
					setOption = true;
				}

				for (int i = (int)(sizeof(buttons_settings)/sizeof(buttons_settings[0]))-1; i >= 0; i--) {
					if (touch.py >= buttons_settings[i].y && touch.py <= (buttons_settings[i].y+buttons_settings[i].h) && touch.px >= buttons_settings[i].x && touch.px <= (buttons_settings[i].x+buttons_settings[i].w)) {
						menuSelection = i;
						setOption = true;
					}
				}

			} else {
				for (int i = ((int)(sizeof(buttons2)/sizeof(buttons2[0]))-1 < (menuPage*8+8)) ? (sizeof(buttons2)/sizeof(buttons2[0]))-1 : (menuPage*8+8); i >= menuPage*8; i--) {
					if (updateAvailable[i]){
						if (touch.py >= (buttons2[i].y-6) && touch.py <= (buttons2[i].y+10) && touch.px >= (buttons2[i].x+75) && touch.px <= (buttons2[i].x+91)) {
							menuSelection = i;
							showMessage = true;
						}
					}
				}

				if (!showMessage) {
				for (int i = ((int)(sizeof(buttons2)/sizeof(buttons2[0]))-1 < (menuPage*8+8)) ? (sizeof(buttons2)/sizeof(buttons2[0]))-1 : (menuPage*8+8); i >= menuPage*8; i--) {
					if (touch.py >= buttons2[i].y && touch.py <= (buttons2[i].y+33) && touch.px >= buttons2[i].x && touch.px <= (buttons2[i].x+87)) {
						menuSelection = i;
						setOption = true;
					}
				}
			}
			}
		}

		if ((hDown & KEY_Y || showMessage) && !showSettings) {
			switch (menuSelection)
			{
				case 0:
					if (dspfirmfound) {
					}
					showReleaseInfo("DS-Homebrew/TWiLightMenu", false);
					break;
				case 1:
					if (dspfirmfound) {
					}
					showCommitInfo("DS-Homebrew/TWiLightMenu", false);
					break;
				case 2:
					if (dspfirmfound) {
					}
					showReleaseInfo("ahezard/nds-bootstrap", false);
					break;
				case 3:
					if (dspfirmfound) {
					}
					showCommitInfo("ahezard/nds-bootstrap", false);
					break;
				case 4:
					if (dspfirmfound) {
					}
					showReleaseInfo("FlagBrew/PKSM", false);
					break;
				case 5:
					if (dspfirmfound) {
					}
					showCommitInfo("SuperSaiyajinVoltZ/PKSM-Nightlies", false);
					break;
				case 6:
					if (dspfirmfound) {
					}
					showReleaseInfo("AuroraWright/Luma3DS", false);
					break;
				case 7:
					if (dspfirmfound) {
					}
					showReleaseInfo("hax0kartik/luma-hourlies", false);
					break;
				case 10:
					if (dspfirmfound) {
					}
					showReleaseInfo("FlagBrew/Checkpoint", false);
					break;
				case 11:
					if (dspfirmfound) {
					}
					showCommitInfo("FlagBrew/Checkpoint", false); //The Repo for the Nightlys.
					break;
				case 12:
					if (dspfirmfound) {
					}
					showReleaseInfo("Universal-Team/Universal-Updater", false);
					break;
				case 13:
					if (dspfirmfound) {
					}
					showCommitInfo("Universal-Team/Universal-Updater", false);
					break;
				case 14:
					if (dspfirmfound) {
					}
					showReleaseInfo("D0k3/GodMode9", false);
					break;
				
				case 15:
					if (dspfirmfound) {
					}
					showCommitInfo("D0k3/GodMode9", false); // The Commit for the Nightlys.
					break;
				case 16:
					if (dspfirmfound) {
					}
					showReleaseInfo("Steveice10/FBI", false); 
					break;
				default:
					if (dspfirmfound) {
					}
					break;
			}
			showMessage = false;
		}

		if (setOption) {
			if (showSettings) {
				int red;
				int green;
				int blue;
				switch (menuSelection) {
				case 0:
				default:
					red = keyboardInputInt("Red");
					settings.universal.bg = RGBA8(red, getColorValue(settings.universal.bg, 1), getColorValue(settings.universal.bg, 0), 255);
					break;
				case 1:
					green = keyboardInputInt("Green (0-255)");
					settings.universal.bg = RGBA8(getColorValue(settings.universal.bg, 2), green, getColorValue(settings.universal.bg, 0), 255);
					break;
				case 2:
					blue = keyboardInputInt("Blue (0-255)");
					settings.universal.bg = RGBA8(getColorValue(settings.universal.bg, 2), getColorValue(settings.universal.bg, 1), blue, 255);
					break;
				case 3:
					red = keyboardInputInt("Red");
					settings.universal.bars = RGBA8(red, getColorValue(settings.universal.bars, 1), getColorValue(settings.universal.bars, 0), 255);
					break;
				case 4:
					green = keyboardInputInt("Green (0-255)");
					settings.universal.bars = RGBA8(getColorValue(settings.universal.bars, 2), green, getColorValue(settings.universal.bars, 0), 255);
					break;
				case 5:
					blue = keyboardInputInt("Blue (0-255)");
					settings.universal.bars = RGBA8(getColorValue(settings.universal.bars, 2), getColorValue(settings.universal.bars, 1), blue, 255);
					break;
				case 6:
					red = keyboardInputInt("Red");
					settings.universal.text = RGBA8(red, getColorValue(settings.universal.text, 1), getColorValue(settings.universal.text, 0), 255);
					break;
				case 7:
					green = keyboardInputInt("Green (0-255)");
					settings.universal.text = RGBA8(getColorValue(settings.universal.text, 2), green, getColorValue(settings.universal.text, 0), 255);
					break;
				case 8:
					blue = keyboardInputInt("Blue (0-255)");
					settings.universal.text = RGBA8(getColorValue(settings.universal.text, 2), getColorValue(settings.universal.text, 1), blue, 255);
					break;
				case 9:
					settings.universal.music++;
					if (settings.universal.music > 3) settings.universal.music = 0;
					break;
				}
			} else {
			char str[16];
			itoa(menuSelection,str,10);
			displayTopMsg(str);
			switch (menuSelection) {
					case 0:	// TWiLight release
						if (checkWifiStatus()){ 
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("DS-Homebrew/TWiLightMenu", true))
							updateTWiLight(false);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 1:	// TWiLight nightly
						if (checkWifiStatus()){ 
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showCommitInfo("DS-Homebrew/TWiLightMenu", true))
							updateTWiLight(true);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 2:	// nds-bootstrap release
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("Ahezard/NDS-Bootstrap", true))
							updateBootstrap(false);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 3:	// nds-bootstrap nightly
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showCommitInfo("ahezard/NDS-Bootstrap", true))
							updateBootstrap(true);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 4:	// PKSM Release
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("FlagBrew/PKSM", true))
							updatePKSM(false);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 5:	// PKSM nightly
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showCommitInfo("SuperSaiyajinVoltZ/PKSM-Nightlies", true))
							updatePKSM(true);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 6:	// Luma Release
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("AuroraWright/Luma3DS", true))
							updateLuma(false);
						break;
					case 7:	// LumaNightly
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showReleaseInfo("hax0kartik/luma-hourlies", true))
							updateLuma(true);
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 8:	// Boxarts
						//if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							downloadBoxart();
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 9:	// usrcheat.dat
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							updateCheats();
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 10: // Checkpoint Release
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("FlagBrew/Checkpoint", true))
							updateCheckpoint();
						} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 11: //Checkpoint Nightly
							if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showReleaseInfo("Flagbrew/Checkpoint", true))
								nightlyCheckpoint();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 12: //Updater Update
							if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("Universal-Team/Universal-Updater", true)) {
								updatingSelf = true;
								updateSelf(false);
								updatingSelf = false;
							} else if (dspfirmfound) {
							}
						}
						break;
					case 13: //Updater Update
							if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showCommitInfo("Universal-Team/Universal-Updater", true)) {
								updatingSelf = true;
								updateSelf(true);
								updatingSelf = false;
							} else if (dspfirmfound) {
							}
						}
						break;
					case 14: //GodMode9 Release
							if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if (showReleaseInfo("D0k3/GodMode9", true))
								downloadGodMode9();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 15: //GodMode9 Nightly
							if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							displayTopMsg("Loading commit notes...");
							if (showCommitInfo("D0k3/GodMode9", true))
								godMode9Nightly();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 16: //NotImplemented!
							if (checkWifiStatus()){
								if (dspfirmfound) {
								}
								displayTopMsg("Loading release notes...");
								if (showReleaseInfo("Steveice10/FBI", true))
								updateFBIRelease();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 17: //NotImplemented!
							if (checkWifiStatus()){
								if (dspfirmfound) {
								}
								notImplementedYet();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 18: //NotImplemented!
							if (checkWifiStatus()){
								if (dspfirmfound) {
								}
								notImplementedYet();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					case 19: //NotImplemented!
							if (checkWifiStatus()){
								if (dspfirmfound) {
								}
								notImplementedYet();
								} else {
							if (dspfirmfound) {
							}
						}
						break;
					default:
						if (dspfirmfound) {
						}
						break;
				}
			}
			setOption = false;
		}

	}
		SaveUniversalSettings();

	if (dspfirmfound) {
		ndspExit();
	}
	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	SDLH_Exit();
	Threads::destroy();
	aptExit();

    return 0;
}
