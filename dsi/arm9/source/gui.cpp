#include <stack>
#include "gui.hpp"


std::stack<std::unique_ptr<SCREEN>> screens;

void Gui::mainLoop(void) {
	screens.top()->Draw();
	screens.top()->Logic();
}

void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
    screens.push(std::move(screen));
}

void Gui::screenBack()
{
    screens.pop();
}