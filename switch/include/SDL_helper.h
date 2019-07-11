#ifndef UNIVERSAL_MANAGER_SDL_HELPER_H
#define UNIVERSAL_MANAGER_SDL_HELPER_H

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

#endif