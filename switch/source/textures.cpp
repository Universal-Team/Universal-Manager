#include "gui.hpp"
#include "textures.hpp"

SDL_Texture *MainMenuButton;

void Textures_Load(void) {
	Gui::LoadImage(&MainMenuButton, "romfs:/Buttons/MainMenuButton.png");
}

void Textures_Free(void) {
	SDL_DestroyTexture(MainMenuButton);
}