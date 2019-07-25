#ifndef _3D_SHELL_C2D_HELPER_H
#define _3D_SHELL_C2D_HELPER_H

#include <citro2d.h>

struct ImageSize {
	int height;
	int width;
};
extern C3D_RenderTarget *RENDER_TOP, *RENDER_BOTTOM;
extern C2D_TextBuf c2d_static_buf, c2d_dynamic_buf, c2d_size_buf;
extern C2D_Font font;
typedef u32 Colour;

struct ImageSize Draw_LoadImageFile(C2D_Image *texture, const char *path);
void GetImageSizeFile(const char *path, int *width, int *height);
bool Draw_LoadImageMemory(C2D_Image *texture, void *data, size_t size);

#endif

// bool Draw_Image(C2D_Image image, float x, float y);
// bool Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY);
// bool Draw_LoadImageFile(C2D_Image *texture, const char *path);
// bool Draw_LoadImageFileGIF(C2D_Image *texture, const char *path);
// bool Draw_LoadImageFilePCX(C2D_Image *texture, const char *path);
// bool Draw_LoadImageMemory(C2D_Image *texture, void *data, size_t size);