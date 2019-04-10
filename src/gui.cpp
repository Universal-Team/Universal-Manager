// Add the Include Paths.
#include <stdio.h>
#include <time.h>

#include "gui.hpp"

C3D_RenderTarget* top;
C3D_RenderTarget* bot;
C2D_SpriteSheet spriteSheet;
C2D_Sprite* sprite;
C2D_TextBuf g_staticBuf, g_dynamicBuf;

Gui::Gui(void) {
	InitUpdaterGFX();
	textinit();
}

Gui::~Gui(void) {
	CloseupdaterGFX();
}

void Gui::Drawui(void)
{
	startframe();
		C2D_SceneBegin(bot);
		C2D_DrawRectSolid(0, 0, 0.5f, SCREEN_WIDTH, 15, C2D_Color32(119, 131, 147, 255));
	}
	endframe();
}

int Gui::InitUpdaterGFX(void)
{
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);
	
	if (!top && !bot && !spriteSheet)
		return 1;
		
	else 
		return 0;

}

void Gui::CloseupdaterGFX(void)
{
	// Delete spritesheet
	C2D_SpriteSheetFree(spriteSheet);
	
	C2D_TextBufDelete(g_dynamicBuf);
	C2D_TextBufDelete(g_staticBuf);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void Gui::startframe(void)
{
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32(0, 0, 255, 255));
	C2D_TargetClear(bot, C2D_Color32(0, 255, 255, 255));
}

void Gui::endframe(void) 
{
	C3D_FrameEnd(0);
	C2D_TextBufClear(g_dynamicBuf);
}

void Gui::textinit(void)
{
	// Create two text buffers: one for static text, and another one for
	// dynamic text - the latter will be cleared at each frame.
	g_staticBuf = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	g_dynamicBuf = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer		
}

void Gui::guiprintColor(const char* text, float xloc, float yloc, float scaleX, float scaleY, u32 color)
{
	// Clear the dynamic text buffer
	C2D_TextBufClear(g_dynamicBuf);

	// Generate and draw dynamic text
	C2D_Text dynText;
	C2D_TextParse(&dynText, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, xloc, yloc, 0.5f, scaleX, scaleY, color);
}

void Gui::guiprint(const char* text, float xloc, float yloc, float scaleX, float scaleY)
{
	guiprintColor(text, xloc, yloc, scaleX, scaleY, 0xFFFFFFFF);
}

/*static void guiprintStatic(Text_id id, float xloc, float yloc, float scaleX, float scaleY)
{
	C2D_DrawText(&staticText[id], C2D_WithColor, xloc, yloc, 0.5f, scaleX, scaleY, 0xFFFFFFFF);
}*/

static void drawImage(int image_id, float x, float y)
{
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, image_id), x, y, 0.6f, NULL, 1.0f, 1.0f);
}
static void drawImageLayered(int image_id, float x, float y, float layer)
{
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, image_id), x, y, layer, NULL, 1.0f, 1.0f);
}