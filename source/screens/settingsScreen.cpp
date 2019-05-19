#include "screens/screenCommon.hpp"
            
void drawSettingsScreen(void) {
	 // Theme Stuff.
	if (settings.universal.theme == 0) {
		drawBgTop();
		drawBarsTopLight();
	} else if (settings.universal.theme == 1) {
		drawBgTop();
		drawBarsTopDark();
	} else if (settings.universal.theme == 2) {
		drawBgTop();
		drawBarsTopRed();
	}
    volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Settings");

    volt_draw_text(50, 130, 0.72f, 0.72f, WHITE, "Press \uE002 to change the Theme.");

		if (settings.universal.theme == 0) {
		drawBgBot();
		drawBarsBotLight();
	} else if (settings.universal.theme == 1) {
		drawBgBot();
		drawBarsBotDark();
	} else if (settings.universal.theme == 2) {
		drawBgBot();
		drawBarsBotRed();
	}

    volt_draw_texture(MainMenuButton, 0, 40);
    volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");

    //volt_draw_texture(MainMenuButton, 170, 40);
	//volt_draw_text(210, 57, 0.7f, 0.7f, BLACK, "Theme"); // To do.

    volt_draw_texture(BackIcon, 288, 208);
    volt_end_draw();
}
