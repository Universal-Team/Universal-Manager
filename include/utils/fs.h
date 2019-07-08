#ifndef FS_H
#define FS_H

#include <3ds.h>

#include <string.h>
#include <time.h>

FS_Archive fsArchive;


typedef struct 
{
    Handle handle;
    u64 offset;
	u64 size;
	unsigned int error;
} FSFILE;

void openArchive(FS_ArchiveID id);
void closeArchive(void);
bool fileExists(FS_Archive archive, const char * path);
Result fsRemove(FS_Archive archive, const char * filename);
Result fsOpen(Handle * handle, const char * path, u32 flags);
Result fsClose(Handle filehandle);
Result writeFile(const char * path, const char * buf);

#endif