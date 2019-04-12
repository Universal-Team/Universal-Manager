// This is the PKSM Screen..
#include "screens/PKSMScreen.hpp"


void loadPKSMScreenGraphics(void)
{
    pp2d_load_texture_png(pksmtoptex, "romfs:/graphics/PKSM/top_bg.png");
    pp2d_load_texture_png(pksmbottomtex, "romfs:/graphics/PKSM/bottom_bg.png");
}

void drawPKSMScreenGraphics(void)
{
    pp2d_draw_on(GFX_TOP, GFX_LEFT);
	pp2d_draw_texture(pksmtoptex, 0, 0);
    pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
    pp2d_draw_texture(pksmbottomtex, 0, 0);
}