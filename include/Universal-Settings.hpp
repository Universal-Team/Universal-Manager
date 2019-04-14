#ifndef UNIVERSAL_SETTINGS_HPP
#define UNIVERSAL_SETTINGS_HPP

#include <string>
#include "graphic.h"
#include "pp2d/pp2d.h"

typedef struct _Settings_t {
	struct {
		int theme;		// 0 = None (16:10), 1 = Left/Right black bars (4:3)
	} universal;
} Settings_t;
extern Settings_t settings;

/**
 * Load the Universal settings.
 */
void LoadUniversalSettings(void);

/**
 * Save the Universal settings.
 */
void SaveUniversalSettings(void);

#endif /* UNIVERSAL_SETTINGS_HPP */