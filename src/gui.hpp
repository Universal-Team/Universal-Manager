#ifndef __UNIVERSAL_UPDATER__
#define __UNIVERSAL_UPDATER__

//Add the Include Paths.
#include <citro3d.h>
#include <citro2d.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

typedef enum {
	TOP_BACKGROUND,
	DOT,
	LOADING_BACKGROUND,
	PAGE_BUTTON,
	PAGE_BUTTON_SMALL,
	TOP_BACKGROUND,
} Gui_Sprites;

class Gui
{
	public:
	Gui(void);
	
	~Gui(void);
	
	void Drawui(void);

    int InitUpdaterGFX(void);

    void CloseupdaterGFX(void);

    private:
    void textinit(void);

    void startframe(void);

	void endframe(void);
    };
#endif