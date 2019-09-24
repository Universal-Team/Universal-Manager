#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
	extern int Barcolor, Bg;

	void loadConfig();
	void saveConfig();
}

#endif
