#ifndef _3D_SHELL_C2D_HELPER_H
#define _3D_SHELL_C2D_HELPER_H

#include <citro2d.h>

bool Draw_LoadImageFile(C2D_Image *texture, const char *path);
bool Draw_LoadImageFileGIF(C2D_Image *texture, const char *path);
bool Draw_LoadImageMemory(C2D_Image *texture, void *data, size_t size);

#endif
