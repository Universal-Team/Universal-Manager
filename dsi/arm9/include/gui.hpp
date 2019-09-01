#include "screen.hpp"
#include <random>
#include <stack>
#include <string.h>
#include <unordered_map>

namespace Gui {
	void mainLoop(u16 hDown, touchPosition touch);
	void setScreen(std::unique_ptr<SCREEN> screen);
	void screenBack(void);
};