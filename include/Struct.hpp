#include "pp2d/pp2d.h"

//Main
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
	//{ 220, 128},
};

size_t button_tex2[] = {
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	classicbuttontex,
	redbuttontex,
	classicbuttontex,
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
	"-",
	"Release",
	//"Cheats",
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
/////////////////////////////////////////////////////////////

//Settings Struct

struct ButtonPos {
	int x;
	int y;
} buttons2[] = {
	{ 129, 48},
}