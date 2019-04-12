// This is the Main Menu Screen part.
#include "screens/MainMenuScreen.hpp"


void loadMainMenuGraphics(void)
{
    pp2d_load_texture_png(mainmenutoptex, "romfs:/graphics/Main-Menu/top_bg.png");
    pp2d_load_texture_png(mainmenubottomtex, "romfs:/graphics/Main-Menu/bottom_bg.png");
}

void drawMainMenuGraphics(void)
{
    pp2d_draw_on(GFX_TOP, GFX_LEFT);
	pp2d_draw_texture(mainmenutoptex, 0, 0);
    pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
    pp2d_draw_texture(mainmenubottomtex, 0, 0);
}

