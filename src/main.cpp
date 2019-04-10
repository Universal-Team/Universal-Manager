// Add the Include Paths.
#include <algorithm>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "gui.hpp"
#include "download.hpp"
#include "dumpdsp.h"
#include "inifile.h"
#include "music.hpp"
#include "thread.hpp"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

static touchPosition touch;

bool dspfirmfound = false;

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
	{ 129, 208},
	{ 220, 208},
};

size_t button_tex2[] = {
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
	extrasmallbuttontex,
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

};

const int title_spacing[] = {
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
};

const char *row_titles2[] = {
	"TWL Menu++",
	"Bootstrap",
	"PKSM",
	"Luma",
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
};


//void displayBottomMsg(const char* text) {
//	C2D_SceneBegin(top);
//	drawImage(BUTTON, 20, 15);
//	guiprint(24, 32, 0.5f, 0.5f, BLACK, text);
//	C2D_End_Draw();
//}

int menuSelection = 0;

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
	mkdir("sdmc:/Universal-Updater/customTheme", 0777); //Custom Theme Path!

