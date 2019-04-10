/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#include "STDirectory.hpp"

STDirectory::STDirectory(const std::string& root)
{
    mGood = false;
    mError = 0;
    mList.clear();
    
    DIR* dir = opendir(root.c_str());
    struct dirent* ent;

    if (dir == NULL)
    {
        mError = (Result)errno;
        closedir(dir);
        return;
    }
    else
    {
        while ((ent = readdir(dir)))
        {
            std::string name = std::string(ent->d_name);
            bool directory = ent->d_type == DT_DIR;
            struct STDirectoryEntry de = { name, directory };
            mList.push_back(de);
        }
    }
    
    closedir(dir);
    mGood = true;
}

Result STDirectory::error(void)
{
    return mError;
}

bool STDirectory::good(void)
{
    return mGood;
}

std::string STDirectory::item(size_t index)
{
    return index < mList.size() ? mList.at(index).name : "";
}

bool STDirectory::folder(size_t index)
{
    return index < mList.size() ? mList.at(index).directory : false; 
}

size_t STDirectory::count(void)
{
    return mList.size();
}