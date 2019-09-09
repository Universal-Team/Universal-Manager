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