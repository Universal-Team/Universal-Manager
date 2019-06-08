/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

// Language functions.
#ifndef UNIVERSAL_MANAGER_LANGUAGE_H
#define UNIVERSAL_MANAGER_LANGUAGE_H

#include <3ds/types.h>

// Active language ID.
extern u8 language;
// System language ID.
extern u8 sys_language;

/**
 * Initialize translations.
 * Uses the language ID specified in settings.ui.language.
 *
 * Check the language variable outside of settings to determine
 * the actual language in use.
 */
void langInit(void);

/**
 * Clear the translations cache.
 */
void langClear(void);

// String IDs.
typedef enum _StrID {
	
	/** Main Menu Stuff. **/
	STR_MAINMENU_TITLE = 0,										// "Universal-Manager"
	STR_MAINMENU_FILEMANAGER,									// "Filemanager"
	STR_MAINMENU_FTP,											// "FTP"
	STR_MAINMENU_UPDATER,										// "Updater"
	STR_MAINMENU_SETTINGS,										// "Settings"

	// File Manager Sub Menu Stuff.
	STR_FILEMANAGER_SUBMENU_MUSICPLAYER,						// "Music Player"
	STR_FILEMANAGER_SUBMENU_IMAGEVIEWER,						// "Image Viewer"
	STR_FILEMANAGER_SUBMENU_TITLE,								// "FileManager Sub Menu"

	// Music Stuff.
	STR_MUSICPLAYERMENU_SONGS,									// "Songs"
	STR_MUSICPLAYERMENU_NOWPLAYING,								// "Now Playing"
	STR_MUSICPLAYERMENU_PLAYLISTS, 								// "Playlists"
	STR_MUSICPLAYERMENU_THEMES,									// "Themes"
	STR_MUSICPLAYERMENU_TITLE,									// "Music Player Menu"

	STR_MUSICFILEBROWSE_OPEN_FOLDER,							// "\uE000 : Open Folder"
	STR_MUSICFILEBROWSE_BACK,									// "\uE001 : Back"
	STR_MUSICFILEBROWSE_EXIT,									// "\uE002 : Exit"
	STR_MUSICFILEBROWSE_ADDTOPL,								// "\uE003 : Add to Playlist"
	STR_MUSICFILEBROWSE_PLAY,									// "\uE000 : Play"

	STR_MUSICPLAYER_PLAY,										// "\uE000 : Play"
	STR_MUSICPLAYER_PAUSE,										// "\uE000 : Pause"
	STR_MUSICPLAYER_BACK,										// "\uE001 : Back"
	STR_MUSICPLAYER_STOPSONG,									// "\uE002 : Stop Song"
	STR_MUSICPLAYER_CURRENTSONG,								// "Current Song:"

	STR_PLAYLIST_TITLE,											// "Music Playlist Menu"
	STR_PLAYLIST_PLAY,											// "\uE000 : Play"
	STR_PLAYLIST_BACK,											// "\uE001 : Back"
	STR_PLAYLIST_DELETE,										// "\uE002 : Delete"
	STR_PLAYLIST_EDIT,											// "\uE003 : Edit"


	STR_PLAYLISTEDIT_SAVE,										// "\uE000 : Save"
	STR_PLAYLISTEDIT_BACK,										// "\uE001 : Back"
	STR_PLAYLISTEDIT_DELETE,									// "\uE002 : Delete"
	STR_PLAYLISTEDIT_MOVE,										// "\uE003 : Move"

	STR_THEMES_TITLE,											// "Theme Selector"

	// Image Stuff.
	STR_IMAGE_PNG_TITLE,										// "Image Viewer Menu [PNG]"
	STR_IMAGE_BMP_TITLE,										// "Image Viewer Menu [BMP]"
	STR_IMAGE_VIEWER_EXIT, 										// "Press B to Exit."

	// Download Stuff.
	STR_DOWNLOAD_DOWNLOADING,									// "Now Downloading"
	STR_DOWNLOAD_EXTRACTING,									// "Now Extracting"
	STR_DOWNLOAD_INSTALLING,									// "Now Installing"

	// FTP Stuff.
	STR_FTP_TITLE,												// "FTP Mode"

	// Settings Stuff.
	STR_SETTINGS_CREDITS,										// "Credits"
	STR_SETTINGS_UISETTINGS,									// "UI Settings"

	// UI Settings stuff.
	STR_UISETTINGS_SAVE,										// "Save"
	STR_UISETTINGS_TITLE,										// "UI Settings"
	STR_UISETTINGS_MUSICMODE,									// "Music Mode:"
	STR_UISETTINGS_BARS,										// "Bars"
	STR_UISETTINGS_BACKGROUND,									// "Background"
	
	STR_MAX
} StrID;

/**
 * Get a translation.
 *
 * NOTE: Call langInit() before using TR().
 *
 * @param strID String ID.
 * @return Translation, or error string if strID is invalid.
 */
const wchar_t *TR(StrID strID);

#endif /* UNIVERSAL_MANAGER_LANGUAGE_H */
