#include "fs.h"

void openArchive(FS_ArchiveID id)
{
	FSUSER_OpenArchive(&fsArchive, id, fsMakePath(PATH_EMPTY, ""));
}

void closeArchive(void)
{
	FSUSER_CloseArchive(fsArchive);
}

bool fileExists(FS_Archive archive, const char * path)
{
	if((!path) || (!archive))
		return false;
	
	Handle handle;

	Result ret = FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0);
	
	if(ret != 0)
		return false;

	ret = FSFILE_Close(handle);
	
	if(ret != 0)
		return false;
	
	return true;
}

Result fsRemove(FS_Archive archive, const char * filename)
{
    Result ret = FSUSER_DeleteFile(archive, fsMakePath(PATH_ASCII, filename));

    return ret == 0 ? 0 : -1;
}

Result fsOpen(Handle * handle, const char * path, u32 flags)
{
	FS_ArchiveID id;
	
	id = ARCHIVE_SDMC;
	
	Result ret = FSUSER_OpenFileDirectly(handle, id, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, path), flags, 0);
	
	return ret == 0 ? 0 : -1;
}

Result fsClose(Handle handle)
{
	Result ret = FSFILE_Close(handle);
	
	return ret == 0 ? 0 : -1;
}

Result writeFile(const char * path, const char * buf)
{
	Handle handle;
	u32 len = strlen(buf);
	u64 size;
	u32 written;
	
	if (fileExists(fsArchive, path))
		fsRemove(fsArchive, path);
	
	Result ret = fsOpen(&handle, path, (FS_OPEN_WRITE | FS_OPEN_CREATE));
	ret = FSFILE_GetSize(handle, &size);
	ret = FSFILE_SetSize(handle, size + len);
	ret = FSFILE_Write(handle, &written, size, buf, len, FS_WRITE_FLUSH);
	ret = fsClose(handle);
	
	return ret == 0 ? 0 : -1;
}