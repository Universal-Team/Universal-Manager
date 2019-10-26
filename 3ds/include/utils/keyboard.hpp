#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	void DrawNumpad();

	std::string Numpad(std::string Text);
	std::string Numpad(uint maxLength, std::string Text);

	std::string getLine(std::string Text);
	std::string getLine(uint maxLength, std::string Text);

	// -1 if invaild text entered
	int getUint(int max, std::string Text);
	char checkKeyboard(int hDown, int hHeld);
}

	void drawKeyboard();
#endif