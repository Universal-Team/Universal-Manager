// This is the TWiLightMenu + NDS-Bootstrap Screen..
#include "screens/TWLScreen.hpp"


void loadTWLScreenGraphics(void)
{
    pp2d_load_texture_png(twltoptex, "romfs:/graphics/TWL/top_bg.png");
    pp2d_load_texture_png(twlbottomtex, "romfs:/graphics/TWL/bottom_bg.png");
}

void drawTWLScreenGraphics(void)
{
    pp2d_draw_on(GFX_TOP, GFX_LEFT);
	pp2d_draw_texture(twltoptex, 0, 0);
    pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
    pp2d_draw_texture(twlbottomtex, 0, 0);
}