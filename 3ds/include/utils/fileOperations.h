#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "fileBrowse.h"

void dirCopy(DirEntry* entry, int i, const char *destinationPath, const char *sourcePath);
int fcopy(const char *sourcePath, const char *destinationPath);

#endif //FILE_OPERATIONS_H
