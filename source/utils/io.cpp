

#include "io.hpp"
#include <sys/stat.h>
#include <unistd.h>

bool io::exists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
