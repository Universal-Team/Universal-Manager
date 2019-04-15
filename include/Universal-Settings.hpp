#ifndef UNIVERSAL_SETTINGS_HPP
#define UNIVERSAL_SETTINGS_HPP

#include <string>
#include "graphic.h"
#include "voltlib/volt.h"

typedef struct _Settings_t {
	struct {
		int bordertop;		// 
		int borderbottom;	//
		int dot;			//
		int button;			//
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