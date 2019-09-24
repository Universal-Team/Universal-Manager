#include "graphics/graphics.h"

#include "screens/screen.hpp"

#include "utils/config.h"

#include <random>
#include <stack>
#include <string.h>
#include <unordered_map>

namespace Gui {
	void mainLoop(u16 hDown, touchPosition touch);
	void setScreen(std::unique_ptr<SCREEN> screen);
	void screenBack(void);

	// GUI Stuff.
	void DrawBGTop(void);
	void DrawBarsTop(void);
	void DrawBGBot(void);
	void DrawBarsBot(void);
};