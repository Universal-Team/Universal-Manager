/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#include "io.hpp"
#include <sys/stat.h>
#include <unistd.h>

bool io::exists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
