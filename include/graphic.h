

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <3ds.h>
#include "voltlib/volt.h"
#include "Colors.hpp"
#include "Textures.hpp"
#include "Universal-Settings.hpp"


void volt_draw_texture_scale_blend(size_t id, int x, int y, float scaleX, float scaleY, u32 color);
void volt_draw_texture_part_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, u32 color);
void volt_draw_texture_part_scale(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY);
void volt_draw_texture_part_scale_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY, u32 color);
void volt_draw_texture_rotate_flip_blend(size_t id, int x, int y, float angle, flipType fliptype, u32 color);
void graphicsInit(void);
//void volt_free_texture(size_t id);
#endif // GRAPHIC_H