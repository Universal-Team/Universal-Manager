#include "screens/screenCommon.hpp"
            
void drawSettingsScreen(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT);
    volt_draw_rectangle(0, 0, 400, 240, GRAY);
    volt_draw_rectangle(0, 0, 400, 25, BLACK);
    volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Settings");

    volt_draw_on(GFX_BOTTOM, GFX_LEFT);
    volt_draw_rectangle(0, 0, 320, 240, GRAY);

    volt_draw_texture(MainMenuButton, 0, 40);
    volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");
    volt_draw_texture(BackIcon, 288, 208);
    volt_end_draw();
}
