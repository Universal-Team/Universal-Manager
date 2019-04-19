
#include <algorithm>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "graphic.h"
#include "voltlib/volt.h"
#include "music.hpp"
#include "thread.hpp"
#include "dumpdsp.h"
#include "settings.h"
#include "download.hpp"
#include "inifile.h"
#include "datetime.h"
#include "Universal-Settings.hpp"
#include "Textures.hpp"
#include "Designs.hpp"


#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

static touchPosition touch;

bool dspfirmfound = false;
bool updatingSelf = false;
bool showSettings = false;

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
	settingsButton,
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
	"     -",
	"Release",
	"Nightly",
	"GodMode9"
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
	8,
};

const char *row_titles2[] = {
	"TWL Menu++",
	"Bootstrap",
	"PKSM",
	"Luma",
	"Downloads",
	"Checkpoint",
	"Updater",
	"Downloads",
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
};

struct {
	int x;
	int y;
} buttons_settings[] = {
	{ 100, 48},
	{ 100, 98},
	{ 100, 148},
	{ 100, 198},
};

size_t buttons_settings_tex[] = {
	settingsButton,
	settingsButton,
	settingsButton,
	settingsButton,
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
	if (settings.universal.bars == 0) {
		draw_Bars_Bottom_Black();
	} else if (settings.universal.bars == 1) {
		draw_Bars_Bottom_White();
	} else if (settings.universal.bars == 2) {
		draw_Bars_Bottom_Blue();
	} else if (settings.universal.bars == 3) {
		draw_Bars_Bottom_Red();
	} else if (settings.universal.bars == 4) {
		draw_Bars_Bottom_Yellow();
	} else if (settings.universal.bars == 5) {
		draw_Bars_Bottom_Gray();
	} else if (settings.universal.bars == 6) {
		draw_Bars_Bottom_Light_Brown();
	} else if (settings.universal.bars == 7) {
		draw_Bars_Bottom_LIGHT_GREEN();
	}
}

void draw_Bars_Top(void) {
	if (settings.universal.bars == 0) {
		draw_Bars_Top_Black();
	} else if (settings.universal.bars == 1) {
		draw_Bars_Top_White();
	} else if (settings.universal.bars == 2) {
		draw_Bars_Top_Blue();
	} else if (settings.universal.bars == 3) {
		draw_Bars_Top_Red();
	} else if (settings.universal.bars == 4) {
		draw_Bars_Top_Yellow();
	} else if (settings.universal.bars == 5) {
		draw_Bars_Top_Gray();
	} else if (settings.universal.bars == 6) {
		draw_Bars_Top_Light_Brown();
	} else if (settings.universal.bars == 7) {
		draw_Bars_Top_LIGHT_GREEN();
	}
}

void draw_Background_Bottom(void) {
	if (settings.universal.bg == 0) {
		draw_Background_Bottom_Black();
	} else if (settings.universal.bg == 1) {
		draw_Background_Bottom_White();
	} else if (settings.universal.bg == 2) {
		draw_Background_Bottom_Blue();
	} else if (settings.universal.bg == 3) {
		draw_Background_Bottom_Red();
	} else if (settings.universal.bg == 4) {
		draw_Background_Bottom_Yellow();
	} else if (settings.universal.bg == 5) {
		draw_Background_Bottom_Gray();
	} else if (settings.universal.bg == 6) {
		draw_Background_Bottom_Light_Brown();
	} else if (settings.universal.bg == 7) {
		draw_Background_Bottom_LIGHT_GREEN();
	}
}

void draw_Background_Top(void) {
	if (settings.universal.bg == 0) {
		draw_Background_Top_Black();
	} else if (settings.universal.bg == 1) {
		draw_Background_Top_White();
	} else if (settings.universal.bg == 2) {
		draw_Background_Top_Blue();
	} else if (settings.universal.bg == 3) {
		draw_Background_Top_Red();
	} else if (settings.universal.bg == 4) {
		draw_Background_Top_Yellow();
	} else if (settings.universal.bg == 5) {
		draw_Background_Top_Gray();
	} else if (settings.universal.bg == 6) {
		draw_Background_Top_Light_Brown();
	} else if (settings.universal.bg == 7) {
		draw_Background_Top_LIGHT_GREEN();
	}
}

void draw_Dialogbox_Color(void) {
	if (settings.universal.bg == 0) {
		volt_draw_rectangle(0, 17, 400, 208, BLACK);
	} else if (settings.universal.bg == 1) {
		volt_draw_rectangle(0, 17, 400, 208, WHITE);
	} else if (settings.universal.bg == 2) {
		volt_draw_rectangle(0, 17, 400, 208, BLUE);
	} else if (settings.universal.bg == 3) {
		volt_draw_rectangle(0, 17, 400, 208, RED);
	} else if (settings.universal.bg == 4) {
		volt_draw_rectangle(0, 17, 400, 208, YELLOW);
	} else if (settings.universal.bg == 5) {
		volt_draw_rectangle(0, 17, 400, 208, GRAY);
	} else if (settings.universal.bg == 6) {
		volt_draw_rectangle(0, 17, 400, 208, LIGHT_BROWN);
	} else if (settings.universal.bg == 7) {
		volt_draw_rectangle(0, 17, 400, 208, LIGHT_GREEN);
	}
}

void select_Text_Color(void) {
	if (settings.universal.txtcolor == 0) {
		settings.universal.text = BLACK;
	} else if (settings.universal.txtcolor == 1) {
		settings.universal.text = WHITE;
	} else if (settings.universal.txtcolor == 2) {
		settings.universal.text = BLUE;
	} else if (settings.universal.txtcolor == 3) {
		settings.universal.text = RED;
	} else if (settings.universal.txtcolor == 4) {
		settings.universal.text = YELLOW;
	} else if (settings.universal.txtcolor == 5) {
		settings.universal.text = GRAY;
	} else if (settings.universal.txtcolor == 6) {
		settings.universal.text = LIGHT_BROWN;
	} else if (settings.universal.txtcolor == 7) {
		settings.universal.text = LIGHT_GREEN;
	}
}

void displayTopMsg(const char* text) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	draw_Dialogbox_Color();
		volt_draw_text(24, 32, 0.5f, 0.5f, settings.universal.text, text);
	volt_end_draw();
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

				volt_load_texture_png(pageframe, "romfs:/graphics/Misc/Page_Number_Frame.png");

				volt_load_texture_png(settingsIcon, "romfs:/graphics/Misc/Settings_Icon.png");
	
	//Result res = 0;
		
 	if ( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	}else{
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		volt_end_draw();
		dumpDsp();
		if ( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
			ndspInit();
			dspfirmfound = true;
		} else {
			for (int i = 0; i < 90; i++) {
				volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
				volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "DSP firm dumping failed.\n"
						"Running without sound.");
				volt_end_draw();
			}	
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
	//	volt_draw_on(GFX_TOP, GFX_LEFT);
	//	if (settings.universal.theme == 0) {									// Load the current Top Border from the Settings.ini!
	//			drawSharkiveUI();
	//		} else if (settings.universal.theme == 1) {
	//			drawCheckpointUI();
	//		} else if (settings.universal.theme == 2) {
	//		drawLasagnaUI();
	//	} else if (settings.universal.theme == 3) {
	//	drawTheme4UI();
	//}
		draw_Background_Top();
		draw_Bars_Top();
		volt_draw_text(4, 3, 0.5f, 0.5f, settings.universal.text, getTime().c_str());
    	volt_draw_text(350, 3, 0.5f, 0.5f, settings.universal.text, "v2.0.0");
    	volt_draw_text(140, 3, 0.5f, 0.5f, settings.universal.text, "Universal-Updater");
    	volt_draw_text(280, 225, 0.5f, 0.5f, settings.universal.text, "\uE004 / \uE005: switch page");
    	volt_draw_text(1, 225, 0.5f, 0.5f, settings.universal.text, "\uE000: Select an Option");
		draw_Background_Bottom();
		draw_Bars_Bottom();
		volt_draw_texture(settingsIcon, 301, 221); // Draw the settings icon

		if (showSettings) {
			// Draw buttons
			for (int i = (int)((sizeof(buttons_settings)/sizeof(buttons_settings[0])))-1; i >= 0; i--) {
				if (menuSelection == i) {
					// Button is highlighted.
					volt_draw_texture(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y);
				} else {
					// Button is not highlighted. Darken the texture.
					if (buttonShading) {
						volt_draw_texture_blend(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y, GRAY);
					} else {
						volt_draw_texture(buttons_settings_tex[i], buttons_settings[i].x, buttons_settings[i].y);
					}
				}

				// Determine the text height.
				// NOTE: Button texture size is 132x34.

				// Draw the Settings Options!
				volt_draw_text(2, 58, 0.65, 0.65, settings.universal.text, "Background");

				volt_draw_text(2, 108, 0.65, 0.65, settings.universal.text, "Bars");

				volt_draw_text(2, 158, 0.65, 0.65, settings.universal.text, "Music");

				volt_draw_text(2, 208, 0.65, 0.65, settings.universal.text, "Text");
			}
			if (settings.universal.bg == 0) {
				volt_draw_text(125, 58, 0.65, 0.65, settings.universal.text, "BLACK");
			} else if (settings.universal.bg == 1) {
				volt_draw_text(125, 58, 0.65, 0.65, settings.universal.text, "WHITE");
			} else if (settings.universal.bg == 2) {
				volt_draw_text(130, 58, 0.65, 0.65, settings.universal.text, "BLUE");
			} else if (settings.universal.bg == 3) {
				volt_draw_text(135, 58, 0.65, 0.65, settings.universal.text, "RED");
			} else if (settings.universal.bg == 4) {
				volt_draw_text(125, 58, 0.65, 0.65, settings.universal.text, "YELLOW");
			} else if (settings.universal.bg == 5) {
				volt_draw_text(130, 58, 0.65, 0.65, settings.universal.text, "GRAY");
			} else if (settings.universal.bg == 6) {
				volt_draw_text(110, 58, 0.65, 0.65, settings.universal.text, "LIGHT BROWN");
			} else if (settings.universal.bg == 7) {
				volt_draw_text(110, 58, 0.65, 0.65, settings.universal.text, "LIGHT GREEN");
			}

			if (settings.universal.bars == 0) {
				volt_draw_text(125, 108, 0.65, 0.65, settings.universal.text, "BLACK");
			} else if (settings.universal.bars == 1) {
				volt_draw_text(125, 108, 0.65, 0.65, settings.universal.text, "WHITE");
			} else if (settings.universal.bars == 2) {
				volt_draw_text(130, 108, 0.65, 0.65, settings.universal.text, "BLUE");
			} else if (settings.universal.bars == 3) {
				volt_draw_text(135, 108, 0.65, 0.65, settings.universal.text, "RED");
			} else if (settings.universal.bars == 4) {
				volt_draw_text(120, 108, 0.65, 0.65, settings.universal.text, "YELLOW");
			} else if (settings.universal.bars == 5) {
				volt_draw_text(130, 108, 0.65, 0.65, settings.universal.text, "GRAY");
			} else if (settings.universal.bars == 6) {
				volt_draw_text(110, 108, 0.65, 0.65, settings.universal.text, "LIGHT BROWN");
			} else if (settings.universal.bars == 7) {
				volt_draw_text(110, 108, 0.65, 0.65, settings.universal.text, "LIGHT GREEN");
			}

			if (settings.universal.music == 0) {
				volt_draw_text(125, 158, 0.65, 0.65, settings.universal.text, "Chill");
			} else if (settings.universal.music == 1) {
				volt_draw_text(120, 158, 0.65, 0.65, settings.universal.text, "Settings");
			} else if (settings.universal.music == 2) {
				volt_draw_text(140, 158, 0.65, 0.65, settings.universal.text, "SD");
			} else if (settings.universal.music == 3) {
				volt_draw_text(135, 158, 0.65f, 0.65f, settings.universal.text, "OFF");
			}

				if (settings.universal.txtcolor == 0) {
				volt_draw_text(125, 208, 0.65, 0.65, settings.universal.text, "BLACK");
			} else if (settings.universal.txtcolor == 1) {
				volt_draw_text(125, 208, 0.65, 0.65, settings.universal.text, "WHITE");
			} else if (settings.universal.txtcolor == 2) {
				volt_draw_text(130, 208, 0.65, 0.65, settings.universal.text, "BLUE");
			} else if (settings.universal.txtcolor == 3) {
				volt_draw_text(135, 208, 0.65, 0.65, settings.universal.text, "RED");
			} else if (settings.universal.txtcolor == 4) {
				volt_draw_text(120, 208, 0.65, 0.65, settings.universal.text, "YELLOW");
			} else if (settings.universal.txtcolor == 5) {
				volt_draw_text(130, 208, 0.65, 0.65, settings.universal.text, "GRAY");
			} else if (settings.universal.txtcolor == 6) {
				volt_draw_text(110, 208, 0.65, 0.65, settings.universal.text, "LIGHT BROWN");
			} else if (settings.universal.txtcolor == 7) {
				volt_draw_text(110, 208, 0.65, 0.65, settings.universal.text, "LIGHT GREEN");
			}

			volt_end_draw();
		} else {
			volt_draw_texture(pageframe, 276+(menuPage*20), 3); //Draw the Page Frame Texture.

    		volt_draw_text(170, 4, 0.50, 0.50, settings.universal.text, "Current Page:");

			volt_draw_text(280, 4, 0.50, 0.50, settings.universal.text, "1"); //Draw First Page Number.

			volt_draw_text(300, 4, 0.50, 0.50, settings.universal.text, "2"); // Draw Second Page Number.
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
			if (buttonShading) menuSelection -= (showSettings ? 1 : 2);
		} else if (hDown & KEY_DOWN) {
			if (buttonShading) menuSelection += (showSettings ? 1 : 2);
		} else if (hDown & KEY_LEFT && !showSettings) {
			if (buttonShading && menuSelection%2) menuSelection--;
		} else if (hDown & KEY_RIGHT && !showSettings) {
			if (buttonShading && !(menuSelection%2)) menuSelection++;
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
			if (menuSelection > 3) menuSelection = 0;
			else if (menuSelection < 0) menuSelection = 3;
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
			showSettings = !showSettings;
		}

		if (hDown & KEY_TOUCH) {
			if (touch.py >= 221 && touch.py <= 240 && touch.px >= 301 && touch.px <= 320) {
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
				default:
					if (dspfirmfound) {
					}
					break;
			}
			showMessage = false;
		}

		if (setOption) {
			if (showSettings) {
				switch (menuSelection) {
				case 0:
				default:
					settings.universal.bg++;
					if (settings.universal.bg > 7) settings.universal.bg = 0;
					break;
				case 1:
					settings.universal.bars++;
					if (settings.universal.bars > 7) settings.universal.bars = 0;
					break;
				case 2:
					settings.universal.music++;
					if (settings.universal.music > 3) settings.universal.music = 0;
					break;
				case 3:
					settings.universal.txtcolor++;
					if (settings.universal.txtcolor > 7) settings.universal.txtcolor = 0;
					select_Text_Color();
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
							displayTopMsg("Loading commit...");
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
							displayTopMsg("Loading commit...");
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
							displayTopMsg("Loading commit...");
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
							displayTopMsg("Loading release notes...");
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
						case 10:	// Checkpoint Release
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
						case 11:	// Checkpoint Release
						if (checkWifiStatus()){
							if (dspfirmfound) {
							}
							notImplementedYet();
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
							displayTopMsg("Loading Commit...");
							if (showCommitInfo("Universal-Team/Universal-Updater", true)) {
								updatingSelf = true;
								updateSelf(true);
								updatingSelf = false;
							} else if (dspfirmfound) {
							}
						}
						break;
							case 14: //Updater Update
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
