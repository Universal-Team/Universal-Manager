#include "gui.hpp"

#include <stack>


std::stack<std::unique_ptr<SCREEN>> screens;

void Gui::mainLoop(u16 hDown, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, touch);
}

void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
	screens.push(std::move(screen));
}

void Gui::screenBack()
{
	screens.pop();
}

// Basic GUI Stuff.

void Gui::DrawBGTop(void)
{
	drawRectangle(0, 20, 256, 152, Config::Bg, true);
}

void Gui::DrawBarsTop(void)
{
	drawRectangle(0, 0, 256, 20, Config::Barcolor, true);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, true);
}

void Gui::DrawBGBot(void)
{
	drawRectangle(0, 20, 256, 152, Config::Bg, false);
}

void Gui::DrawBarsBot(void)
{
	drawRectangle(0, 0, 256, 20, Config::Barcolor, false);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, false);
}