
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
#include "Themes.hpp"


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
	{ 129, 118},
	{ 220, 118},
	{ 129, 168},
	//{ 220, 128},
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
	//greenbuttontex,
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
	//"Cheats",
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
	//10,
};

const char *row_titles2[] = {
	"TWL Menu++",
	"Bootstrap",
	"PKSM",
	"Luma",
	"Downloads",
	"Checkpoint",
	"Updater",
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
	//false,
};

struct {
	int x;
	int y;
} buttons_settings[] = {
	{ 129, 48},
	{ 220, 48},
	{ 129, 88},
	{ 220, 88},
	{ 129, 128},
	{ 220, 128},
	{ 129, 168},
	{ 220, 168},
};

const char *button_titles_settings[] = {
	"Release",
	"Nightly",
	"Release",
	"Nightly",
	"Release",
	"Nightly",
	"Nightly",	
	"Release",
};

const int title_spacing_settings[] = {
	6,
	10,
	6,
	10,
	6,
	10,
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

void displayTopMsg(const char* text) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	//volt_draw_texture(loadingbgtex, 0, 0);
	volt_draw_text(24, 32, 0.5f, 0.5f, BLACK, text);
	volt_end_draw();
}

// Version numbers.

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
	SDLH_Init();

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Updater", 0777); //main Path.
	mkdir("sdmc:/Universal-Updater/Music", 0777); //MP3 Path.

	volt_init();

	volt_set_3D(1);

				volt_load_texture_png(button, "romfs:/graphics/Misc/Button.png");

				volt_load_texture_png(dot, "romfs:/graphics/Misc/Dot.png");

                //volt_load_texture_png(loadingbgtex, "romfs:/graphics/standard-Graphics/BS_loading_background.png"); Not used anymore. Maybe Later again?

			   // volt_load_texture_png(topbgtex, "romfs:/graphics/standard-Graphics/top_bg.png"); // Not used. Maybe later?

				volt_load_texture_png(pageframe, "romfs:/graphics/Misc/Page_Number_Frame.png");

				volt_load_texture_png(settingsIcon, "romfs:/graphics/Misc/Settings_Icon.png");
	
	//Result res = 0;
		
 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	}else{
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		volt_end_draw();
		dumpDsp();
		if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
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
	LoadUniversalSettings();
	// Loop as long as the status is not exit
	Threads::create((ThreadFunc)SDLH_Play);
	while(aptMainLoop()) {
		offset3D[0].topbg = CONFIG_3D_SLIDERSTATE * -7.0f;
		offset3D[1].topbg = CONFIG_3D_SLIDERSTATE * 7.0f;

		// Scan hid shared memory for input events
		hidScanInput();
		
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();

		hidTouchRead(&touch);

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) volt_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else volt_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			//volt_draw_texture(topbgtex, offset3D[topfb].topbg, 0);
			if (fadealpha > 0) volt_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		}
		volt_draw_on(GFX_TOP, GFX_LEFT);
		if (settings.universal.theme == 1) {									// Load the current Top Border from the Settings.ini!
				drawSharkiveUI();
			} else if (settings.universal.theme == 2) {
				drawCheckpointUI();
			}
		//drawSharkiveUI();
		volt_draw_text(170, 5, 0.50, 0.50, WHITE, "current Page :");
		volt_draw_text(280, 5, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
		volt_draw_text(300, 5, 0.50, 0.50, WHITE, "2"); // Draw Second Page Number.
		volt_draw_texture(pageframe, 276+(menuPage*20), 3); //Draw the Page Frame Texture.
		volt_draw_texture(settingsIcon, 292, 212); // Draw the settings icon
		if (menuPage == 1) {
			volt_draw_text(1, 78, 0.50, 0.50, WHITE, "For TWiLightMenu++"); // Draw Second Page Number.
		}

		if(showSettings) {
			// Draw buttons
			for (int i = (int)((sizeof(buttons_settings)/sizeof(buttons_settings[0])))-1; i >= 0; i--) {
				if (menuSelection == i) {
					// Button is highlighted.
					volt_draw_texture(button_tex2[i], buttons_settings[i].x, buttons_settings[i].y);
				} else {
					// Button is not highlighted. Darken the texture.
					if (buttonShading) {
						volt_draw_texture_blend(button_tex2[i], buttons_settings[i].x, buttons_settings[i].y, GRAY);
					} else {
						volt_draw_texture(button_tex2[i], buttons_settings[i].x, buttons_settings[i].y);
					}
				}

				// Determine the text height.
				// NOTE: Button texture size is 132x34.
				const int h = 30;

				// Draw the title.
				int y = buttons_settings[i].y + ((40 - h) / 2);
				int x_from_width = buttons_settings[i].x + title_spacing_settings[i];
				volt_draw_text(x_from_width, y, 0.75, 0.75, BLACK, button_titles_settings[i]);
			}
			volt_end_draw();
		} else {
			// Draw buttons
			for (int i = (int)((sizeof(buttons2)/sizeof(buttons2[0])))-1; i >= 0; i--) {
				if(i <= ((ceil(((double)menuSelection+1)/8)*8)-1) && i >= ((ceil(((double)menuSelection+1)/8)*8)-8)) {
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
					if(updateAvailable[i]) {
						volt_draw_texture(dot, buttons2[i].x+75, buttons2[i].y-6);
					}

					// Determine the text height.
					// NOTE: Button texture size is 132x34.
					const int h = 30;

					// Draw the title.
					int y = buttons2[i].y + ((40 - h) / 2);
					int x_from_width = buttons2[i].x + title_spacing2[i];
					volt_draw_text(x_from_width, y, 0.75, 0.75, BLACK, button_titles2[i]);

					if(!(i%2)) {
						volt_draw_text(5, y, 0.7, 0.7, BLACK, row_titles2[i/2]);
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

		if (hDown & KEY_R) {
			if(menuPage<(ceil(((double)(sizeof(buttons2)/sizeof(buttons2[0])+1.0)/8))-1)) {
				menuPage++;
				menuSelection += 8;
				if(menuSelection > (int)sizeof(buttons2))	menuSelection = (int)sizeof(buttons2);
			}
		} else if (hDown & KEY_L) {
			if(menuPage>0) {
				menuPage--;
				menuSelection -= 8;
				if(menuSelection < 0)	menuSelection = 0;
			}
		}

		if (hDown & KEY_UP) {
			if (buttonShading) menuSelection -= 2;
		} else if (hDown & KEY_DOWN) {
			if (buttonShading) menuSelection += 2;
		} else if (hDown & KEY_LEFT) {
			if (buttonShading && menuSelection%2) menuSelection--;
		} else if (hDown & KEY_RIGHT) {
			if (buttonShading && !(menuSelection%2)) menuSelection++;
		}
		if (hDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) {
			buttonShading = true;
			if(dspfirmfound) {
			}
		}

		if (hDown & KEY_TOUCH) {
			buttonShading = false;
		}

		if ((menuSelection > (menuPage*8)+8) || (menuSelection > (int)(sizeof(buttons2)/sizeof(buttons2[0])))) {
			menuSelection = (menuPage*8)+1; 
		} else if ((menuSelection > (menuPage*8)+7) || (menuSelection > (int)((sizeof(buttons2)/sizeof(buttons2[0]))-1))) {
			menuSelection = (menuPage*8); 
		} else if (menuSelection < (menuPage*8)-1) {
			menuSelection = ((menuPage*8)+6 < (int)(sizeof(buttons2)/sizeof(buttons2[0]))-2) ? (menuPage*8)+6 : sizeof(buttons2)/sizeof(buttons2[0])-2;
		} else if (menuSelection < (menuPage*8)) {
			menuSelection = ((menuPage*8)+7 < (int)(sizeof(buttons2)/sizeof(buttons2[0]))-1) ? (menuPage*8)+7 : sizeof(buttons2)/sizeof(buttons2[0])-1;
		}

		if (hDown & KEY_A) {
			setOption = true;
		}

		if (hDown & KEY_TOUCH) {
			if(touch.py >= 212 && touch.py <= 240 && touch.px >= 292 && touch.px <= 320) {
				showSettings = !showSettings;
			} else {
				for (int i = (int)(sizeof(buttons2)/sizeof(buttons2[0]))-1; i >= 0; i--) {
					if(updateAvailable[i]){
						if (touch.py >= (buttons2[i].y-6) && touch.py <= (buttons2[i].y+10) && touch.px >= (buttons2[i].x+75) && touch.px <= (buttons2[i].x+91)) {
							menuSelection = i;
							showMessage = true;
						}
					}
				}
			}
			if(!showMessage) {
				for (int i = (int)(sizeof(buttons2)/sizeof(buttons2[0]))-1; i >= 0; i--) {
					if (touch.py >= buttons2[i].y && touch.py <= (buttons2[i].y+33) && touch.px >= buttons2[i].x && touch.px <= (buttons2[i].x+87)) {
						menuSelection = i;
						setOption = true;
					}
				}
			}
		}

		if (hDown & KEY_Y || showMessage) {
			switch (menuSelection)
			{
				case 0:
					if(dspfirmfound) {
					}
					showReleaseInfo("RocketRobz/TWiLightMenu", false);
					break;
				case 1:
					if(dspfirmfound) {
					}
					showCommitInfo("RocketRobz/TWiLightMenu", false);
					break;
				case 2:
					if(dspfirmfound) {
					}
					showReleaseInfo("ahezard/nds-bootstrap", false);
					break;
				case 3:
					if(dspfirmfound) {
					}
					showCommitInfo("ahezard/nds-bootstrap", false);
					break;
				case 4:
					if(dspfirmfound) {
					}
					showReleaseInfo("FlagBrew/PKSM", false);
					break;
				case 5:
					if(dspfirmfound) {
					}
					showCommitInfo("SuperSaiyajinVoltZ/PKSM-Nightlies", false);
					break;
					case 6:
					if(dspfirmfound) {
					}
					showReleaseInfo("AuroraWright/Luma3DS", false);
					break;
					case 7:
					if(dspfirmfound) {
					}
					showReleaseInfo("hax0kartik/luma-hourlies", false);
					break;
					case 10:
					if(dspfirmfound) {
					}
					showReleaseInfo("FlagBrew/Checkpoint", false);
					break;
					case 12:
					if(dspfirmfound) {
					}
					showReleaseInfo("SuperSaiyajinVoltZ/Universal-Updater", false);
					break;
				default:
					if(dspfirmfound) {
					}
					break;
			}
			showMessage = false;
		}

		if (setOption) {
			if(showSettings) {
				switch (menuSelection) {
					
				}
			} else {
			switch ((menuPage*8)+menuSelection) {
					case 0:	// TWiLight release
						if(checkWifiStatus()){ 
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("RocketRobz/TWiLightMenu", true))
							updateTWiLight(false);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 1:	// TWiLight nightly
						if(checkWifiStatus()){ 
							if(dspfirmfound) {
							}
							displayTopMsg("Loading commit...");
							if(showCommitInfo("RocketRobz/TWiLightMenu", true))
							updateTWiLight(true);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 2:	// nds-bootstrap release
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("Ahezard/NDS-Bootstrap", true))
							updateBootstrap(false);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 3:	// nds-bootstrap nightly
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading commit...");
							if(showCommitInfo("ahezard/NDS-Bootstrap", true))
							updateBootstrap(true);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 4:	// PKSM Release
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("FlagBrew/PKSM", true))
							updatePKSM(false);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 5:	// PKSM nightly
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading commit...");
							if(showCommitInfo("SuperSaiyajinVoltZ/PKSM-Nightlies", true))
							updatePKSM(true);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
					case 6:	// Luma Release
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("AuroraWright/Luma3DS", true))
							updateLuma(false);
						break;
					case 7:	// LumaNightly
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("hax0kartik/luma-hourlies", true))
							updateLuma(true);
						} else {
							if(dspfirmfound) {
							}
						}
						break;
						case 8:	// Boxarts
						//if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							downloadBoxart();
						} else {
							if(dspfirmfound) {
							}
						}
						break;
						case 9:	// usrcheat.dat
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							updateCheats();
						} else {
							if(dspfirmfound) {
							}
						}
						break;
						case 10:	// Checkpoint Release
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("FlagBrew/Checkpoint", true))
							updateCheckpoint();
						} else {
							if(dspfirmfound) {
							}
						}
						break;
						case 11:	// Checkpoint Release
						if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							notImplementedYet();
						} else {
							if(dspfirmfound) {
							}
						}
						break;
						case 12: //Updater Update
							if(checkWifiStatus()){
							if(dspfirmfound) {
							}
							displayTopMsg("Loading release notes...");
							if(showReleaseInfo("SuperSaiyajinVoltZ/Universal-Updater", true))
								updatingSelf = true;
								updateSelf();
								updatingSelf = false;
								} else {
							if(dspfirmfound) {
							}
						}
						break;
					default:
						if(dspfirmfound) {
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