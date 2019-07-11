#ifndef _3D_SHELL_C2D_HELPER_H
#define _3D_SHELL_C2D_HELPER_H

#include <citro2d.h>

struct ImageSize {
	int height;
	int width;
};

struct ImageSize Draw_LoadImageFile(C2D_Image *texture, const char *path);
void GetImageSizeFile(const char *path, int *width, int *height);
bool Draw_LoadImageMemory(C2D_Image *texture, void *data, size_t size);

#endif
