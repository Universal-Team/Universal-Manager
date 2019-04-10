/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#include "stringutils.hpp"

bool matchPattern(std::string pattern, std::string tested)
{
	std::regex patternRegex(pattern);
	return regex_match(tested, patternRegex);
}
