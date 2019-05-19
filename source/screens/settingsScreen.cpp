#include "screens/screenCommon.hpp"
            
void drawSettingsScreen(void) {
	 // Theme Stuff.
	if (settings.universal.theme == 0) {
		drawBgTop();
		drawBarsTopLight();
	} else if (settings.universal.theme == 1) {
		drawBgTop();
		drawBarsTopDark();
	}
    volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Settings");

		if (settings.universal.theme == 0) {
		drawBgBot();
		drawBarsBotLight();
	} else if (settings.universal.theme == 1) {
		drawBgBot();
		drawBarsBotDark();
	}

    volt_draw_texture(MainMenuButton, 0, 40);
    volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");
    volt_draw_texture(BackIcon, 288, 208);
    volt_end_draw();
}
