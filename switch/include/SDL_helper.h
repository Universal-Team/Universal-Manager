/*
*   This file is part of Universal-Manager-NX
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "SDL_FontCache.h"
#include "colors.hpp"

SDL_Renderer *SDL_GetMainRenderer(void);
SDL_Window *SDL_GetMainWindow(void);
int SDL_Initialize(void);
void SDL_Terminate(void);
void SDL_ClearScreen(SDL_Color colour);
void SDL_DrawRect(int x, int y, int w, int h, SDL_Color colour);
void SDL_DrawText(int x, int y, int size, SDL_Color colour, const char *text);
void SDL_GetTextDimensions(int size, const char *text, u32 *width, u32 *height);
void SDL_LoadImage(SDL_Texture **texture, char *path);
void SDL_DrawImage(SDL_Texture *texture, int x, int y);
void SDL_RenderScreen(void);
