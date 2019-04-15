#include "Themes.hpp"
#include "Colors.hpp"
#include "Graphic.h"
#include "voltlib/volt.h"
#include "utils/datetime.h"

void drawSharkiveUI(void) {
    volt_set_screen_color(GFX_TOP, SHARKIVE_BG);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
	volt_draw_rectangle(0, 0, 400, 19, SHARKIVE_BARS);
	volt_draw_rectangle(0, 221, 400, 19, SHARKIVE_BARS);

	volt_draw_text(4, 3, 0.45f, 0.45f, GREYISH, getTime().c_str());
    volt_draw_text(350, 3, 0.45f, 0.45f, GREYISH, "v1.0.0");
    volt_draw_text(140, 3, 0.45f, 0.45f, WHITE, "Universal-Updater");
    volt_draw_text(280, 210, 0.45f, 0.45f, WHITE, "\uE004 / \uE005: switch page");
    volt_draw_text(150, 210, 0.45f, 0.45f, WHITE, "\uE000: Choose an Option");
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, SHARKIVE_BG);
    volt_draw_rectangle(0, 0, 320, 19, SHARKIVE_BARS);
	volt_draw_rectangle(0, 221, 320, 19, SHARKIVE_BARS);
    //volt_end_draw();
}
void drawCheckpointUI(void) {
    volt_set_screen_color(GFX_TOP, CHECKPOINT_BG);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
	volt_draw_rectangle(0, 0, 400, 19, CHECKPOINT_BARS);
	volt_draw_rectangle(0, 221, 400, 19, CHECKPOINT_BARS);

	volt_draw_text(4, 3, 0.45f, 0.45f, GREYISH, getTime().c_str());
    volt_draw_text(350, 3, 0.45f, 0.45f, GREYISH, "v1.0.0");
    volt_draw_text(140, 3, 0.45f, 0.45f, WHITE, "Universal-Updater");
    volt_draw_text(280, 210, 0.45f, 0.45f, WHITE, "\uE004 / \uE005: switch page");
    volt_draw_text(150, 210, 0.45f, 0.45f, WHITE, "\uE000: Choose an Option");
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, CHECKPOINT_BG);
    volt_draw_rectangle(0, 0, 320, 19, CHECKPOINT_BARS);
	volt_draw_rectangle(0, 221, 320, 19, CHECKPOINT_BARS);
    //volt_end_draw();
}