

#ifndef STDIRECTORY_HPP
#define STDIRECTORY_HPP

#include <dirent.h>
#include "types.h"
#include <errno.h>
#include <string>
#include <vector>

struct STDirectoryEntry {
    std::string name;
    bool        directory;
};

class STDirectory
{
public:
    STDirectory(const std::string& root);
    ~STDirectory(void) { };

    Result      error(void);
    std::string item(size_t index);
    bool        folder(size_t index);
    bool        good(void);
    size_t      count(void);

private:
    std::vector<STDirectoryEntry> mList;
    Result                  mError;
    bool                    mGood;
};

#endif
