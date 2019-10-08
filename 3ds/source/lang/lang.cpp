#include "utils/settings.hpp"
#include "utils/inifile.h"

#include "lang/lang.h"
#include "lang/langStrings.h"

#include <fstream>



std::string langs[] = {"de", "en", "es", "fr", "it", "jp", "lt", "pt"};

void Lang::loadLangStrings(int lang) {
	// Declare variables
	char path[32];
	std::string line;

	// Load app strings
		snprintf(path, sizeof(path), "romfs:/lang/%s/app.ini", langs[lang].c_str());

	CIniFile ini(path);

	// [ButtonTester]
	Lang::ButtonTester[0] = ini.GetString("ButtonTester", "Title", Lang::ButtonTester[0]);
	Lang::ButtonTester[1] = ini.GetString("ButtonTester", "pressedBtn", Lang::ButtonTester[1]);

	// [Calendar]
	Lang::Calendar[0] = ini.GetString("Calendar", "January", Lang::Calendar[0]);
	Lang::Calendar[1] = ini.GetString("Calendar", "February", Lang::Calendar[1]);
	Lang::Calendar[2] = ini.GetString("Calendar", "March", Lang::Calendar[2]);
	Lang::Calendar[3] = ini.GetString("Calendar", "April", Lang::Calendar[3]);
	Lang::Calendar[4] = ini.GetString("Calendar", "May", Lang::Calendar[4]);
	Lang::Calendar[5] = ini.GetString("Calendar", "June", Lang::Calendar[5]);
	Lang::Calendar[6] = ini.GetString("Calendar", "July", Lang::Calendar[6]);
	Lang::Calendar[7] = ini.GetString("Calendar", "August", Lang::Calendar[7]);
	Lang::Calendar[8] = ini.GetString("Calendar", "September", Lang::Calendar[8]);
	Lang::Calendar[9] = ini.GetString("Calendar", "October", Lang::Calendar[9]);
	Lang::Calendar[10] = ini.GetString("Calendar", "November", Lang::Calendar[10]);
	Lang::Calendar[11] = ini.GetString("Calendar", "December", Lang::Calendar[11]);

	Lang::Calendar[12] = ini.GetString("Calendar", "Sunday", Lang::Calendar[12]);
	Lang::Calendar[13] = ini.GetString("Calendar", "Monday", Lang::Calendar[13]);
	Lang::Calendar[14] = ini.GetString("Calendar", "Tuesday", Lang::Calendar[14]);
	Lang::Calendar[15] = ini.GetString("Calendar", "Wednesday", Lang::Calendar[15]);
	Lang::Calendar[16] = ini.GetString("Calendar", "Thursday", Lang::Calendar[16]);
	Lang::Calendar[17] = ini.GetString("Calendar", "Friday", Lang::Calendar[17]);
	Lang::Calendar[18] = ini.GetString("Calendar", "Saturday", Lang::Calendar[18]);
	Lang::Calendar[19] = ini.GetString("Calendar", "currentDay", Lang::Calendar[19]);

	// [CurrentPage]
	Lang::CurrentPage = ini.GetString("CurrentPage", "CurrentPage", Lang::CurrentPage);

	// [MainMenu]
	Lang::MainMenu[0] = ini.GetString("MainMenu", "FileManager", Lang::MainMenu[0]);
	Lang::MainMenu[1] = ini.GetString("MainMenu", "FTP", Lang::MainMenu[1]);
	Lang::MainMenu[2] = ini.GetString("MainMenu", "Scripts", Lang::MainMenu[2]);
	Lang::MainMenu[3] = ini.GetString("MainMenu", "Music", Lang::MainMenu[3]);
	Lang::MainMenu[4] = ini.GetString("MainMenu", "Updater", Lang::MainMenu[4]);
	Lang::MainMenu[5] = ini.GetString("MainMenu", "Settings", Lang::MainMenu[5]);

	Lang::MainMenu[6] = ini.GetString("MainMenu", "ImageViewer", Lang::MainMenu[6]);
	Lang::MainMenu[7] = ini.GetString("MainMenu", "Credits", Lang::MainMenu[7]);
	Lang::MainMenu[8] = ini.GetString("MainMenu", "TextEditor", Lang::MainMenu[8]);
	Lang::MainMenu[9] = ini.GetString("MainMenu", "Utils", Lang::MainMenu[9]);
	Lang::MainMenu[10] = ini.GetString("MainMenu", "Games", Lang::MainMenu[10]);

	// [Misc]
	Lang::Misc[0] = ini.GetString("Misc", "running", Lang::Misc[0]);

	// [Title]
	Lang::Title = ini.GetString("Title", "Name", Lang::Title);
}

void Lang::refreshLanguage(int lang) {
	return loadLangStrings(lang);
}