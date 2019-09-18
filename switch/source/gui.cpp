#include "gui.hpp"
#include "screens/screenCommon.hpp"

#include <assert.h>
#include <stack>
#include <stdarg.h>
#include <unistd.h>

std::stack<std::unique_ptr<SCREEN>> screens;
SDL_Window *Window;
SDL_Renderer *Rendering;
FC_Font *Roboto, *Roboto_large, *Roboto_small, *Roboto_title;

SDL_Renderer *SDL_GetMainRenderer(void) {
	return Rendering;
}

SDL_Window *SDL_GetMainWindow(void) {
	return Window;
}

static FC_Font *SDL_GetFont(int size) {
	if (size == 20)
		return Roboto_small;
	else if (size == 25)
		return Roboto;
	else if (size == 30)
		return Roboto_large;
	else if (size == 72)
		return Roboto_title;
	
	return Roboto;
}

Result Gui::Init(void) {
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
		return -1;

	Window = SDL_CreateWindow("Universal-Manager-NX", 0, 0, 1280, 720, SDL_WINDOW_FULLSCREEN);
	if (Window == NULL)
		return -1;

	Rendering = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
		return -1;

	Roboto_small = FC_CreateFont();
	FC_LoadFont(Roboto_small, Rendering, "romfs:/Font/Roboto-Regular.ttf", 20, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	Roboto = FC_CreateFont();
	FC_LoadFont(Roboto, Rendering, "romfs:/Font/Roboto-Regular.ttf", 25, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	Roboto_large = FC_CreateFont();
	FC_LoadFont(Roboto_large, Rendering, "romfs:/Font/Roboto-Regular.ttf", 30, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

	Roboto_title = FC_CreateFont();
	FC_LoadFont(Roboto_title, Rendering, "romfs:/Font/Roboto-Regular.ttf", 72, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
	return 0;
}


void Gui::Exit(void) {
	FC_FreeFont(Roboto_large);
	FC_FreeFont(Roboto);
	FC_FreeFont(Roboto_title);
	//FC_FreeFont(Roboto_small);
	TTF_Quit();

	IMG_Quit();

	SDL_DestroyRenderer(Rendering);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Gui::ClearScreen(SDL_Color colour) {
	SDL_SetRenderDrawColor(Rendering, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderClear(Rendering);
}

void Gui::DrawRect(int x, int y, int w, int h, SDL_Color colour) {
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_SetRenderDrawBlendMode(Rendering, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Rendering, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(Rendering, &rect);
}

void Gui::DrawText(int x, int y, int size, SDL_Color colour, const char *text) {
	FC_DrawColor(SDL_GetFont(size), Rendering, x, y, colour, text);
}

void Gui::GetTextDimensions(int size, const char *text, u32 *width, u32 *height) {
	FC_Font *font = SDL_GetFont(size);

	if (width != NULL) 
		*width = FC_GetWidth(font, text);
	if (height != NULL) 
		*height = FC_GetHeight(font, text);
}

void Gui::LoadImage(SDL_Texture **texture, char *path) {
	SDL_Surface *loaded_surface = NULL;
	loaded_surface = IMG_Load(path);

	if (loaded_surface) {
		SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0);
		*texture = SDL_CreateTextureFromSurface(Rendering, loaded_surface);
	}

	SDL_FreeSurface(loaded_surface);
}

void Gui::DrawImage(SDL_Texture *texture, int x, int y) {
	SDL_Rect position;
	position.x = x; position.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(Rendering, texture, NULL, &position);
}

void Gui::RenderScreen(void) {
	SDL_RenderPresent(Rendering);
}

// Screen Stuff.

void Gui::mainLoop(u64 hDown) {
	screens.top()->Draw();
	screens.top()->Logic(hDown);
}

void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
    screens.push(std::move(screen));
}

void Gui::screenBack()
{
    screens.pop();
}


// Basic GUI Stuff.

void Gui::DrawBG(void) 
{
	Gui::DrawRect(0, 0, 1280, 720, GRAY);
}

void Gui::DrawBars(void) 
{
	Gui::DrawRect(0, 0, 1280, 100, BARCOLOR);
	Gui::DrawRect(0, 620, 1280, 100, BARCOLOR);
}