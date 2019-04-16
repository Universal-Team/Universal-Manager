#include "Themes.hpp"
#include "Colors.hpp"
#include "graphic.h"
#include "voltlib/volt.h"
#include "utils/datetime.h"

void drawSharkiveUI(void) {
    volt_set_screen_color(GFX_TOP, SHARKIVE_BG);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
	volt_draw_rectangle(0, 0, 400, 19, SHARKIVE_BARS);
	volt_draw_rectangle(0, 221, 400, 19, SHARKIVE_BARS);

	volt_draw_text(4, 3, 0.5f, 0.5f, GREYISH, getTime().c_str());
    volt_draw_text(350, 3, 0.5f, 0.5f, GREYISH, "v1.0.0");
    volt_draw_text(140, 3, 0.5f, 0.5f, WHITE, "Universal-Updater");
    volt_draw_text(280, 225, 0.5f, 0.5f, WHITE, "\uE004 / \uE005: switch page");
    volt_draw_text(1, 225, 0.5f, 0.5f, WHITE, "\uE000: Select an Option");

    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, SHARKIVE_BG);
    volt_draw_rectangle(0, 0, 320, 19, SHARKIVE_BARS);
	volt_draw_rectangle(0, 221, 320, 19, SHARKIVE_BARS);

    volt_draw_text(170, 5, 0.50, 0.50, WHITE, "Current Page :");
	volt_draw_text(280, 5, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
	volt_draw_text(300, 5, 0.50, 0.50, WHITE, "2"); // Draw Second Page Number.
	volt_draw_texture(settingsIcon, 292, 212); // Draw the settings icon
}
void drawCheckpointUI(void) {
    volt_set_screen_color(GFX_TOP, CHECKPOINT_BG);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
	volt_draw_rectangle(0, 0, 400, 19, CHECKPOINT_BARS);
	volt_draw_rectangle(0, 221, 400, 19, CHECKPOINT_BARS);

	volt_draw_text(4, 3, 0.45f, 0.45f, GREYISH, getTime().c_str());
    volt_draw_text(350, 3, 0.45f, 0.45f, GREYISH, "v1.0.0");
    volt_draw_text(140, 3, 0.45f, 0.45f, WHITE, "Universal-Updater");
    volt_draw_text(280, 225, 0.5f, 0.5f, BLACK, "\uE004 / \uE005: switch page");
    volt_draw_text(1, 225, 0.5f, 0.5f, BLACK, "\uE000: Select an Option");

    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, CHECKPOINT_BG);
    volt_draw_rectangle(0, 0, 320, 19, CHECKPOINT_BARS);
	volt_draw_rectangle(0, 221, 320, 19, CHECKPOINT_BARS);

    volt_draw_text(170, 5, 0.50, 0.50, BLACK, "Current Page :");
	volt_draw_text(280, 5, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	volt_draw_text(300, 5, 0.50, 0.50, BLACK, "2"); // Draw Second Page Number.
	volt_draw_texture(settingsIcon, 292, 212); // Draw the settings icon
}

void drawLasagnaUI(void) {
    volt_set_screen_color(GFX_TOP, LASAGNA_BG);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
	volt_draw_rectangle(0, 0, 400, 19, LASAGNA_BARS);
	volt_draw_rectangle(0, 221, 400, 19, LASAGNA_BARS);

	volt_draw_text(4, 3, 0.45f, 0.45f, GREYISH, getTime().c_str());
    volt_draw_text(350, 3, 0.45f, 0.45f, GREYISH, "v1.0.0");
    volt_draw_text(140, 3, 0.45f, 0.45f, WHITE, "Universal-Updater");
    volt_draw_text(280, 225, 0.5f, 0.5f, WHITE, "\uE004 / \uE005: switch page");
    volt_draw_text(1, 225, 0.5f, 0.5f, WHITE, "\uE000: Select an Option");

    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, LASAGNA_BG);
    volt_draw_rectangle(0, 0, 320, 19, LASAGNA_BARS);
	volt_draw_rectangle(0, 221, 320, 19, LASAGNA_BARS);

    volt_draw_text(170, 5, 0.50, 0.50, BLACK, "Current Page :");
	volt_draw_text(280, 5, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	volt_draw_text(300, 5, 0.50, 0.50, BLACK, "2"); // Draw Second Page Number.
	volt_draw_texture(settingsIcon, 292, 212); // Draw the settings icon
}
