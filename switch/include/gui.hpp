#ifndef GUI_HPP
#define GUI_HPP

#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "SDL_FontCache.h"
#include "colors.hpp"

#define FONT_SIZE_18 0.72f
#define FONT_SIZE_17 0.7f
#define FONT_SIZE_15 0.6f
#define FONT_SIZE_14 0.56f
#define FONT_SIZE_12 0.50f
#define FONT_SIZE_11 0.46f
#define FONT_SIZE_9 0.37f

namespace Gui
{
    Result Init(void);
    void Exit(void);
    
    void ClearScreen(SDL_Color colour);
    void DrawRect(int x, int y, int w, int h, SDL_Color colour);
    void DrawText(int x, int y, int size, SDL_Color colour, const char *text);
    void GetTextDimensions(int size, const char *text, u32 *width, u32 *height);
    void LoadImage(SDL_Texture **texture, char *path);
    void DrawImage(SDL_Texture *texture, int x, int y);
    void RenderScreen(void);
}

    SDL_Window *SDL_GetMainWindow(void);
    SDL_Renderer *SDL_GetMainRenderer(void);

#endif
