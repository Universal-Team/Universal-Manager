#include "screens/screenCommon.hpp"
						
void drawSettingsScreen(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Settings");

	volt_draw_text(50, 90, 0.72f, 0.72f, WHITE, "Press \uE003 to change the Green RGB.\nPress \uE002 to change the Red RGB.\nPress \uE000 to change the Blue RGB.\n and press R to save!");

	drawBgBot();
	drawBarsBot();

	volt_draw_texture(MainMenuButton, 0, 40);
	volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");

	//volt_draw_texture(MainMenuButton, 170, 40);
	//volt_draw_text(210, 57, 0.7f, 0.7f, BLACK, "Theme"); // To do.

	volt_draw_texture(BackIcon, 288, 208);
	volt_end_draw();
}
