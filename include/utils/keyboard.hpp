#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <string>

namespace Input {
	std::string getLine();
	std::string getLine(uint maxLength);
	// -1 if invaild text entered
	int getUint(int max);
}

#endif
