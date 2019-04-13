

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <3ds.h>
#include "pp2d/pp2d.h"
#include "Colors.hpp"
#include "Textures.hpp"


void pp2d_draw_texture_scale_blend(size_t id, int x, int y, float scaleX, float scaleY, u32 color);
void pp2d_draw_texture_part_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, u32 color);
void pp2d_draw_texture_part_scale(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY);
void pp2d_draw_texture_part_scale_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY, u32 color);
void pp2d_draw_texture_rotate_flip_blend(size_t id, int x, int y, float angle, flipType fliptype, u32 color);
//void pp2d_free_texture(size_t id);
#endif // GRAPHIC_H