#include "SDL_helper.h"
#include "textures.h"

SDL_Texture *MainMenuButton;

void Textures_Load(void) {
	SDL_LoadImage(&MainMenuButton, "romfs:/Buttons/MainMenuButton.png");
}

void Textures_Free(void) {
	SDL_DestroyTexture(MainMenuButton);
}