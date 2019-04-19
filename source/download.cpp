/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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

#include "download.hpp"
#include <sys/stat.h>
#include <vector>
#include <unistd.h>

#include "extract.hpp"
#include "inifile.h"
#include "graphic.h"
#include "fileBrowse.h"
#include "voltlib/volt.h"
#include "Universal-Settings.hpp"
#include "Colors.hpp"
extern "C" {
	#include "cia.h"
}

#define  USER_AGENT   APP_TITLE "-" VERSION_STRING

static char* result_buf = NULL;
static size_t result_sz = 0;
static size_t result_written = 0;
std::vector<std::string> _topText;
std::string jsonName;
CIniFile versionsFile("sdmc:/Universal-Updater/currentVersions.ini");
std::string latestMenuReleaseCache = "";
std::string latestMenuNightlyCache = "";
std::string latestBootstrapReleaseCache = "";
std::string latestBootstrapNightlyCache = "";
std::string latestPKSMReleaseCache = "";
std::string latestPKSMNightlyCache = "";
std::string latestLumaReleaseCache = "";
std::string latestLumaNightlyCache = "";
std::string latestCheckpointReleaseCache = "";
std::string latestCheckpointNightlyCache = "";
std::string latestUpdaterReleaseCache = "";
std::string latestUpdaterNightlyCache = "";
std::string godMode9Cache = "";
std::string godMode9NightlyCache = "";
extern void displayTopMsg(const char* text);
extern void draw_Dialogbox_Color(void);

extern bool downloadNightlies;
extern bool updateAvailable[];

// following function is from 
// https://github.com/angelsl/libctrfgh/blob/master/curl_test/src/main.c
static size_t handle_data(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    (void) userdata;
    const size_t bsz = size*nmemb;

    if (result_sz == 0 || !result_buf)
    {
        result_sz = 0x1000;
        result_buf = (char*)malloc(result_sz);
    }

    bool need_realloc = false;
    while (result_written + bsz > result_sz) 
    {
        result_sz <<= 1;
        need_realloc = true;
    }

    if (need_realloc)
    {
        char *new_buf = (char*)realloc(result_buf, result_sz);
        if (!new_buf)
        {
            return 0;
        }
        result_buf = new_buf;
    }

    if (!result_buf)
    {
        return 0;
    }

    memcpy(result_buf + result_written, ptr, bsz);
    result_written += bsz;
    return bsz;
}

static Result setupContext(CURL *hnd, const char * url)
{
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, url);
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, USER_AGENT);
    curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, handle_data);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(hnd, CURLOPT_STDERR, stdout);

    return 0;
}

Result downloadToFile(std::string url, std::string path)
{
	Result ret = 0;	
	printf("Downloading from:\n%s\nto:\n%s\n", url.c_str(), path.c_str());

    void *socubuf = memalign(0x1000, 0x100000);
    if (!socubuf)
    {
        return -1;
    }

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
    {
		free(socubuf);
        return ret;
    }

	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, url.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return ret;
	}

	Handle fileHandle;
	u64 offset = 0;
	u32 bytesWritten = 0;
	
	ret = openFile(&fileHandle, path.c_str(), true);
	if (R_FAILED(ret)) {
		printf("Error: couldn't open file to write.\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return DL_ERROR_WRITEFILE;
	}
	
	u64 startTime = osGetTime();

	CURLcode cres = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);

	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return -1;
	}
	
	FSFILE_Write(fileHandle, &bytesWritten, offset, result_buf, result_written, 0);

	u64 endTime = osGetTime();
	u64 totalTime = endTime - startTime;
	printf("Download took %llu milliseconds.\n", totalTime);

    socExit();
    free(result_buf);
    free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;
	FSFILE_Close(fileHandle);
	return 0;
}

Result downloadFromRelease(std::string url, std::string asset, std::string path)
{
	Result ret = 0;
    void *socubuf = memalign(0x1000, 0x100000);
    if (!socubuf)
    {
        return -1;
    }

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
    {
		free(socubuf);
        return ret;
    }

	std::regex parseUrl("github\\.com\\/(.+)\\/(.+)");
	std::smatch result;
	regex_search(url, result, parseUrl);
	
	std::string repoOwner = result[1].str(), repoName = result[2].str();
	
	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repoOwner << "/" << repoName << "/releases/latest";
	std::string apiurl = apiurlStream.str();
	
	printf("Downloading latest release from repo:\n%s\nby:\n%s\n", repoName.c_str(), repoOwner.c_str());
	printf("Crafted API url:\n%s\n", apiurl.c_str());
	
	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return ret;
	}

	CURLcode cres = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string
	
	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return -1;
	}
	
	printf("Looking for asset with matching name:\n%s\n", asset.c_str());
	std::string assetUrl;
	json parsedAPI = json::parse(result_buf);
	if (parsedAPI["assets"].is_array()) {
		for (auto jsonAsset : parsedAPI["assets"]) {
			if (jsonAsset.is_object() && jsonAsset["name"].is_string() && jsonAsset["browser_download_url"].is_string()) {
				std::string assetName = jsonAsset["name"];
				if (matchPattern(asset, assetName)) {
					assetUrl = jsonAsset["browser_download_url"];
					break;
				}
			}
		}
	}
    socExit();
    free(result_buf);
    free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;
	
	if (assetUrl.empty())
		ret = DL_ERROR_GIT;
	else
		ret = downloadToFile(assetUrl, path);
	
	return ret;
}

/**
 * Check Wi-Fi status.
 * @return True if Wi-Fi is connected; false if not.
 */
bool checkWifiStatus(void) {
	u32 wifiStatus;
	bool res = false;

	if (R_SUCCEEDED(ACU_GetWifiStatus(&wifiStatus)) && wifiStatus) {
		res = true;
	}
	
	return res;
}

void downloadFailed(void) {
	displayTopMsg("Download failed!\n");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void notImplemented(void) {
	displayTopMsg("Not implemented Yet.\n");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void doneMsg(void) {
	displayTopMsg("Done!");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

std::string getLatestRelease(std::string repo, std::string item)
{
	Result ret = 0;
    void *socubuf = memalign(0x1000, 0x100000);
    if (!socubuf)
    {
        return "";
    }

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
    {
		free(socubuf);
        return "";
    }
	
	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repo << "/releases/latest";
	std::string apiurl = apiurlStream.str();
	
	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}

	CURLcode cres = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string
	
	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}
	
	std::string jsonItem;
	json parsedAPI = json::parse(result_buf);
	if (parsedAPI[item].is_string()) {
		jsonItem = parsedAPI[item];
	}
    socExit();
    free(result_buf);
    free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return jsonItem;
}

std::string getLatestCommit(std::string repo, std::string item)
{
	Result ret = 0;
    void *socubuf = memalign(0x1000, 0x100000);
    if (!socubuf)
    {
        return "";
    }

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
    {
		free(socubuf);
        return "";
    }
	
	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repo << "/commits/master";
	std::string apiurl = apiurlStream.str();
	
	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}

	CURLcode cres = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string
	
	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}
	
	std::string jsonItem;
	json parsedAPI = json::parse(result_buf);
	if (parsedAPI[item].is_string()) {
		jsonItem = parsedAPI[item];
	}
    socExit();
    free(result_buf);
    free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return jsonItem;
}

std::string getLatestCommit(std::string repo, std::string array, std::string item)
{
	Result ret = 0;
    void *socubuf = memalign(0x1000, 0x100000);
    if (!socubuf)
    {
        return "";
    }

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
    {
		free(socubuf);
        return "";
    }
	
	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repo << "/commits/master";
	std::string apiurl = apiurlStream.str();
	
	CURL *hnd = curl_easy_init();
	ret = setupContext(hnd, apiurl.c_str());
	if (ret != 0) {
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}

	CURLcode cres = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
	char* newbuf = (char*)realloc(result_buf, result_written + 1);
	result_buf = newbuf;
	result_buf[result_written] = 0; //nullbyte to end it as a proper C style string
	
	if (cres != CURLE_OK) {
		printf("Error in:\ncurl\n");
		socExit();
		free(result_buf);
		free(socubuf);
		result_buf = NULL;
		result_sz = 0;
		result_written = 0;
		return "";
	}
	
	std::string jsonItem;
	json parsedAPI = json::parse(result_buf);
	if (parsedAPI[array][item].is_string()) {
		jsonItem = parsedAPI[array][item];
	}
    socExit();
    free(result_buf);
    free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return jsonItem;
}

bool showReleaseInfo(std::string repo, bool showExitText)
{
	jsonName = getLatestRelease(repo, "name");
	std::string jsonBody = getLatestRelease(repo, "body");
	
	setMessageText(jsonBody);
	int textPosition = 0;
	bool redrawText = true;

	while(1) {
		if(redrawText) {
			drawMessageText(textPosition, showExitText);
			redrawText = false;
		}

		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();

		if(hHeld & KEY_UP || hHeld & KEY_DOWN) {
			for(int i=0;i<9;i++)
				gspWaitForVBlank();
		}
		
		if (hDown & KEY_A || hDown & KEY_Y || hDown & KEY_TOUCH) {
			return true;
		} else if (hDown & KEY_B) {
			return false;
		} else if (hHeld & KEY_UP) {
			if(textPosition > 0) {
				textPosition--;
				redrawText = true;
			}
		} else if (hHeld & KEY_DOWN) {
			if(textPosition < (int)(_topText.size() - 10)) {
				textPosition++;
				redrawText = true;
			}
		}
	}
}

bool showCommitInfo(std::string repo, bool showExitText)
{
	jsonName = getLatestCommit(repo, "sha").substr(0,7);
	std::string jsonBody = getLatestCommit(repo, "commit", "message");
	setMessageText(jsonBody);
	int textPosition = 0;
	bool redrawText = true;

	while(1) {
		if(redrawText) {
			drawMessageText(textPosition, showExitText);
			redrawText = false;
		}

		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();

		if(hHeld & KEY_UP || hHeld & KEY_DOWN) {
			for(int i=0;i<9;i++)
				gspWaitForVBlank();
		}
		
		if (hDown & KEY_A || hDown & KEY_Y || hDown & KEY_TOUCH) {
			return true;
		} else if (hDown & KEY_B) {
			return false;
		} else if (hHeld & KEY_UP) {
			if(textPosition > 0) {
				textPosition--;
				redrawText = true;
			}
		} else if (hHeld & KEY_DOWN) {
			if(textPosition < (int)(_topText.size() - 10)) {
				textPosition++;
				redrawText = true;
			}
		}
	}
}

void setMessageText(const std::string &text)
{
	std::string _topTextStr(text);
	std::vector<std::string> words;
	std::size_t pos;
	// std::replace( _topTextStr.begin(), _topTextStr.end(), '\n', ' ');
	_topTextStr.erase(std::remove(_topTextStr.begin(), _topTextStr.end(), '\r'), _topTextStr.end());
	while((pos = _topTextStr.find(' ')) != std::string::npos) {
		words.push_back(_topTextStr.substr(0, pos));
		_topTextStr = _topTextStr.substr(pos + 1);
	}
	if(_topTextStr.size())
		words.push_back(_topTextStr);
	std::string temp;
	_topText.clear();
	for(auto word : words)
	{
		int width = volt_get_text_width((temp + " " + word).c_str(), 0.5f, 0.5f);
		if(word.find('\n') != -1u)
		{
			word.erase(std::remove(word.begin(), word.end(), '\n'), word.end());
			temp += " " + word;
			_topText.push_back(temp);
			temp = "";
		}
		else if(width > 256)
		{
			_topText.push_back(temp);
			temp = word;
		}
		else
		{
			temp += " " + word;
		}
	}
	if(temp.size())
	   _topText.push_back(temp);
}

void drawMessageText(int position, bool showExitText)
{
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	draw_Dialogbox_Color();
	volt_draw_text(18, 24, .7, .7, settings.universal.text, jsonName.c_str());
	for (int i = 0; i < (int)_topText.size() && i < (showExitText ? 9 : 10); i++) {
			volt_draw_text(24, ((i * 16) + 48), 0.5f, 0.5f, settings.universal.text, _topText[i+position].c_str());
	}
	if(showExitText)
		volt_draw_text(24, 200, 0.5f, 0.5f, settings.universal.text, "B: Cancel   A: Update");
	volt_end_draw();
}

std::string latestMenuRelease(void) {
	if (latestMenuReleaseCache == "")
		latestMenuReleaseCache = getLatestRelease("DS-Homebrew/TWiLightMenu", "tag_name");
	return latestMenuReleaseCache;
}

std::string latestMenuNightly(void) {
	if (latestMenuNightlyCache == "")
		latestMenuNightlyCache = getLatestCommit("DS-Homebrew/TWiLightMenu", "sha").substr(0,7);
	return latestMenuNightlyCache;
}

std::string latestBootstrapRelease(void) {
	if (latestBootstrapReleaseCache == "")
		latestBootstrapReleaseCache = getLatestRelease("ahezard/nds-bootstrap", "tag_name");
	return latestBootstrapReleaseCache;
}

std::string latestBootstrapNightly(void) {
	if (latestBootstrapNightlyCache == "")
		latestBootstrapNightlyCache = getLatestCommit("ahezard/nds-bootstrap", "sha").substr(0,7);
	return latestBootstrapNightlyCache;
}

std::string latestPKSMRelease(void) {
	if (latestPKSMReleaseCache == "")
		latestPKSMReleaseCache = getLatestRelease("FlagBrew/PKSM", "tag_name");
	return latestPKSMReleaseCache;
}

std::string latestPKSMNightly(void) {
	if (latestPKSMNightlyCache == "")
		latestPKSMNightlyCache = getLatestCommit("SuperSaiyajinVoltZ/PKSM-Nightlies", "sha").substr(0,7);
	return latestPKSMNightlyCache;
}

std::string latestLumaRelease(void) {
	if (latestLumaReleaseCache == "")
		latestLumaReleaseCache = getLatestRelease("Aurorawright/Luma3DS", "tag_name");
	return latestLumaReleaseCache;
}

std::string latestLumaNightly(void) {
	if (latestLumaNightlyCache == "")
		latestLumaNightlyCache = getLatestRelease("hax0kartik/luma-hourlies", "tag_name");
	return latestLumaNightlyCache;
}

std::string latestCheckpointRelease(void) {
	if (latestCheckpointReleaseCache == "")
		latestCheckpointReleaseCache = getLatestRelease("FlagBrew/Checkpoint", "tag_name");
	return latestCheckpointReleaseCache;
}


std::string latestCheckpointNightly(void) {
	if (latestCheckpointNightlyCache == "")
		latestCheckpointNightlyCache = getLatestCommit("FlagBrew/Checkpoint", "tag_name");
	return latestCheckpointNightlyCache;
}


std::string latestUpdaterRelease(void) {
	if (latestUpdaterReleaseCache == "")
		latestUpdaterReleaseCache = getLatestRelease("Universal-Team/Universal-Updater", "tag_name");
	return latestUpdaterReleaseCache;
}



std::string latestUpdaterNightly(void) {
	if (latestUpdaterNightlyCache == "")
		latestUpdaterNightlyCache = getLatestCommit("Universal-Team/Universal-Updater", "sha").substr(0,7);
	return latestUpdaterNightlyCache;
}

std::string latestGodMode9(void) {
	if (godMode9Cache == "")
		godMode9Cache = getLatestRelease("D0k3/GodMode9", "tag_name");
	return godMode9Cache;
}

std::string NightlyGodMode9(void) {
	if (godMode9NightlyCache == "")
		godMode9NightlyCache = getLatestCommit("D0k3/GodMode9", "tag_name");
	return godMode9NightlyCache;
}

void saveUpdateData(void) {
	versionsFile.SaveIniFile("sdmc:/Universal-Updater/currentVersions.ini");
}

std::string getInstalledVersion(std::string component) {
	return versionsFile.GetString(component, "VERSION", "");
}

std::string getInstalledChannel(std::string component) {
	return versionsFile.GetString(component, "CHANNEL", "");
}

void setInstalledVersion(std::string component, std::string version) {
	versionsFile.SetString(component, "VERSION", version);
}

void setInstalledChannel(std::string component, std::string channel) {
	versionsFile.SetString(component, "CHANNEL", channel);
}

void checkForUpdates() {

	std::string menuChannel = getInstalledChannel("TWILIGHTMENU");
	std::string menuVersion = getInstalledVersion("TWILIGHTMENU");
	std::string pksmChannel = getInstalledChannel("PKSM");
	std::string pksmVersion = getInstalledVersion("PKSM");
	std::string bootstrapRelease = getInstalledVersion("NDS-BOOTSTRAP-RELEASE");
	std::string boostrapNightly = getInstalledVersion("NDS-BOOTSTRAP-NIGHTLY");
	std::string lumaRelease = getInstalledVersion("LUMA3DS-RELEASE");
	std::string lumaNightly = getInstalledVersion("LUMA3DS-NIGHTLY");
	std::string checkpointRelease = getInstalledVersion("CHECKPOINT-RELEASE");
	std::string checkpointNightly = getInstalledChannel("CHECKPOINT-NIGHTLY");
	std::string updaterChannel = getInstalledChannel("UNIVERSAL-UPDATER");
	std::string updaterVersion = getInstalledVersion("UNIVERSAL-UPDATER");
	std::string godMode9Version = getInstalledVersion ("GODMODE9");
	std::string godMode9Nightly = getInstalledChannel ("GODMODE9-NIGHTLY");

	if (menuChannel == "release")
		updateAvailable[0] = menuVersion != latestMenuRelease();
	else if (menuChannel == "nightly")
		updateAvailable[1] = menuVersion != latestMenuNightly();
	else
		updateAvailable[0] = updateAvailable[1] = true;

	updateAvailable[2] = bootstrapRelease != latestBootstrapRelease();
	updateAvailable[3] = boostrapNightly != latestBootstrapNightly();

	if (pksmChannel == "release")
		updateAvailable[4] = pksmVersion != latestPKSMRelease();
	else if (pksmChannel == "nightly")
		updateAvailable[5] = pksmVersion != latestPKSMNightly();
	else
		updateAvailable[4] = updateAvailable[5] = true;

	updateAvailable[6] = lumaRelease != latestLumaRelease();
	updateAvailable[7] = lumaNightly != latestLumaNightly();
	updateAvailable[10] = checkpointRelease != latestCheckpointRelease();
	updateAvaliable[11] = checkpointNightly != latestCheckpointCommit();
	updateAvailable[14] = godMode9Version != latestGodMode9();
	updateAvaliable[16] = godMode9Nightly != NightlyGodMode9();
	
	if (updaterChannel == "release")
		updateAvailable[12] = updaterVersion != latestUpdaterRelease();
	else if (updaterChannel == "nightly")
		updateAvailable[13] = updaterVersion != latestUpdaterNightly();
	else
		updateAvailable[12] = updateAvailable[13] = true;
	

}

void updateBootstrap(bool nightly) {
	if(nightly) {
		displayTopMsg("Downloading NDS-Bootstrap\n"
						"(Nightly)");
		if (downloadToFile("https://github.com/TWLBot/Builds/blob/master/nds-bootstrap.7z?raw=true", "/nds-bootstrap-nightly.7z") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Now extracting.\n"
						"(Nightly)");
		extractArchive("/nds-bootstrap-nightly.7z", "nds-bootstrap/", "/_nds/");

		deleteFile("sdmc:/nds-bootstrap-nightly.7z");

		setInstalledVersion("NDS-BOOTSTRAP-NIGHTLY", latestBootstrapNightly());
		saveUpdateData();
		updateAvailable[3] = false;
	} else {	
		displayTopMsg("Downloading NDS-Bootstrap\n"
						"(Release)");
		if (downloadFromRelease("https://github.com/ahezard/nds-bootstrap", "nds-bootstrap\\.zip", "/nds-bootstrap-release.zip") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Now extracting.\n"
						"(Release)");
		extractArchive("/nds-bootstrap-release.zip", "/", "/_nds/");

		deleteFile("sdmc:/nds-bootstrap-release.zip");

		setInstalledVersion("NDS-BOOTSTRAP-RELEASE", latestBootstrapRelease());
		saveUpdateData();
		updateAvailable[2] = false;
	}
	doneMsg();
}

void updateTWiLight(bool nightly) {
	if(nightly) {
		displayTopMsg("Now Downloading TWiLightMenu++\n"
						"(Nightly)\n\nThis may take a while.");
		if (downloadToFile("https://github.com/TWLBot/Builds/blob/master/TWiLightMenu.7z?raw=true", "/TWiLightMenu-nightly.7z") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Now extracting.\n"
						"(Nightly)\n\nThis may take a while.");
		extractArchive("/TWiLightMenu-nightly.7z", "TWiLightMenu/_nds/", "/_nds/");
		extractArchive("/TWiLightMenu-nightly.7z", "3DS - CFW users/", "/");

		displayTopMsg("Now installing the CIAs.\n"
						"(Nightly)");
		// installCia("/TWiLight Menu.cia");
		installCia("/TWiLight Menu - Game booter.cia");

		deleteFile("sdmc:/TWiLightMenu-nightly.7z");
		deleteFile("sdmc:/TWiLight Menu.cia");
		deleteFile("sdmc:/TWiLight Menu - Game booter.cia");

		setInstalledChannel("TWILIGHTMENU", "nightly");
		setInstalledVersion("TWILIGHTMENU", latestMenuNightly());
		saveUpdateData();
		updateAvailable[1] = false;
	} else {
		displayTopMsg("Downloading TWiLightMenu++\n"
						"(Release)\n\nThis may take a while.");
		if (downloadFromRelease("https://github.com/DS-Homebrew/TWiLightMenu", "TWiLightMenu\\.7z", "/TWiLightMenu-release.7z") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Now extracting.\n"
						"(Release)\n\nThis may take a while.");
		extractArchive("/TWiLightMenu-release.7z", "_nds/", "/_nds/");
		extractArchive("/TWiLightMenu-release.7z", "3DS - CFW users/", "/");
		extractArchive("/TWiLightMenu-release.7z", "DSi&3DS - SD card users/", "/");

		displayTopMsg("Now installing the CIAs.\n"
						"(Release)");
		installCia("/TWiLight Menu.cia");
		installCia("/TWiLight Menu - Game booter.cia");

		deleteFile("sdmc:/TWiLightMenu-release.7z");
		deleteFile("sdmc:/TWiLight Menu.cia");
		deleteFile("sdmc:/TWiLight Menu - Game booter.cia");

		setInstalledChannel("TWILIGHTMENU", "release");
		setInstalledVersion("TWILIGHTMENU", latestMenuRelease());
		saveUpdateData();
		updateAvailable[0] = false;
	}
	doneMsg();
}

void updatePKSM(bool nightly) {
	if(nightly) {
		displayTopMsg("Downloading PKSM...\n"
						"(Nightly)\n\nThis may take a while.");
		if (downloadToFile("https://github.com/SuperSaiyajinVoltZ/PKSM-Nightlies/blob/master/PKSM.cia?raw=true", "/PKSM-Nightly.cia") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Installing PKSM CIA...\n"
						"(Nightly)\n\n\n\n\n\n\n\n\n\n"
						"This may take a while.");
		installCia("/PKSM-Nightly.cia");

		deleteFile("sdmc:/PKSM-Nightly.cia");

		setInstalledChannel("PKSM", "nightly");
		setInstalledVersion("PKSM", latestPKSMNightly());
		saveUpdateData();
		updateAvailable[5] = false;
	} else {
		displayTopMsg("Downloading PKSM\n"
						"(Release)\n\nThis may take a while.");
		if (downloadFromRelease("https://github.com/FlagBrew/PKSM", "PKSM\\.cia", "/PKSM-Release.cia") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Installing PKSM.cia\n"
						"(Release)\n\n\n\n\n\n\n\n\n\n"
						"This may take a while..");
		installCia("/PKSM-Release.cia");

		deleteFile("sdmc:/PKSM-Release.cia");

		setInstalledChannel("PKSM", "release");
		setInstalledVersion("PKSM", latestPKSMRelease());
		saveUpdateData();
		updateAvailable[4] = false;
	}
	doneMsg();
}

void updateCheats(void) {
	displayTopMsg("Downloading DSJ's usrcheat.dat\n");
	if (downloadToFile("https://github.com/TWLBot/Builds/raw/master/usrcheat.dat.7z", "/usrcheat.dat.7z") != 0) {
		downloadFailed();
		return;
	}

	displayTopMsg("Now extracting...\n"
					"\nThis may take a while.");
	extractArchive("/usrcheat.dat.7z", "usrcheat.dat", "/_nds/TWiLightMenu/extras/usrcheat.dat");

	deleteFile("sdmc:/usrcheat.dat.7z");

	doneMsg();
}

const char* getBoxartRegion(char tid_region) {
	// European boxart languages.
	static const char *const ba_langs_eur[] = {
		"EN",	// Japanese (English used in place)
		"EN",	// English
		"FR",	// French
		"DE",	// German
		"IT",	// Italian
		"ES",	// Spanish
		"ZHCN",	// Simplified Chinese
		"KO",	// Korean
	};
	CIniFile ini("sdmc:/_nds/TWiLightMenu/settings.ini");
	int language = ini.GetInt("SRLOADER", "LANGUAGE", -1);
	const char *ba_region;

	switch (tid_region) {
		case 'E':
		case 'T':
			ba_region = "US";	// USA
			break;
		case 'J':
			ba_region = "JA";	// Japanese
			break;
		case 'K':
			ba_region = "KO";	// Korean
			break;

		case 'O':			// USA/Europe
			// if (region == CFG_REGION_USA) {
				// System is USA region.
				// Get the USA boxart if it's available.
				ba_region = "EN";
				break;
			// }
			// fall-through
		case 'P':			// Europe
		default:
			// System is not USA region.
			// Get the European boxart that matches the system's language.
			// TODO: Check country code for Australia.
			// This requires parsing the Config savegame. (GetConfigInfoBlk2())
			// Reference: https://3dbrew.org/wiki/Config_Savegame
			if(language == -1)
				ba_region = "EN";	// TODO: make this actually set to the console's language
			else
				ba_region = ba_langs_eur[language];
			break;

		case 'U':
			// Alternate country code for Australia.
			ba_region = "AU";
			break;

		// European country-specific localizations.
		case 'D':
			ba_region = "DE";	// German
			break;
		case 'F':
			ba_region = "FR";	// French
			break;
		case 'H':
			ba_region = "NL";	// Dutch
			break;
		case 'I':
			ba_region = "IT";	// Italian
			break;
		case 'R':
			ba_region = "RU";	// Russian
			break;
		case 'S':
			ba_region = "ES";	// Spanish
			break;
		case '#':
			ba_region = "HB"; // Homebrew
			break;
	}
	return ba_region;
}

void downloadBoxart(void) {

	vector<DirEntry> dirContents;
	std::string scanDir;

	displayTopMsg("Would you like to choose a directory, or scan\nthe entire SD card?\n\n\n\n\n\n\n\n\n\nB: Cancel   A: Full SD   X: Choose Directory");

	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();

		if(hDown & KEY_A) {
			scanDir = "sdmc:/";
			break;
		} else if(hDown & KEY_X) {
			chdir("sdmc:/");
			bool dirChosen = false;
			uint selectedDir = 0;
			while(!dirChosen) {
				getDirectoryContents(dirContents);
				for(uint i=0;i<dirContents.size();i++) {
					if(!dirContents[i].isDirectory) {
						dirContents.erase(dirContents.begin()+i);
					}
				}
				while(1) {
					gspWaitForVBlank();
					hidScanInput();
					const u32 hDown = hidKeysDown();
					if(hDown & KEY_A) {
						chdir(dirContents[selectedDir].name.c_str());
						selectedDir = 0;
						break;
					} else if(hDown & KEY_B) {
						chdir("..");
						selectedDir = 0;
						break;
					}	else if(hDown & KEY_X) {
						chdir(dirContents[selectedDir].name.c_str());
						char path[1024];
						getcwd(path, sizeof(path));
						scanDir = path;
						dirChosen = true;
						break;
					} else if(hDown & KEY_UP) {
						if(selectedDir > 0) {
							selectedDir--;
						}
					} else if(hDown & KEY_DOWN) {
						if(selectedDir < dirContents.size()-1) {
							selectedDir++;
						}
					}
					std::string dirs;
					for(uint i=(selectedDir<10) ? 0 : selectedDir-10;i<dirContents.size()&&i<((selectedDir<10) ? 11 : selectedDir+1);i++) {
						if(i == selectedDir) {
							dirs += "> " + dirContents[i].name + "\n";
						} else {
							dirs += "  " + dirContents[i].name + "\n";
						}
					}
					for(uint i=0;i<((dirContents.size()<10) ? 11-dirContents.size() : 0);i++) {
						dirs += "\n";
					}
					dirs += "B: Back   A: Open   X: Choose";
					displayTopMsg(dirs.c_str());
				}
			}
			break;
		} else if(hDown & KEY_B) {
			return;
		}
	}

	displayTopMsg("Scanning SD card for DS roms...\n");

	chdir(scanDir.c_str());
	findNdsFiles(dirContents);

	for(int i=0;i<(int)dirContents.size();i++) {
		char downloadMessage[50];
		snprintf(downloadMessage, sizeof(downloadMessage), "Downloading \"%s.bmp\"...\n", dirContents[i].tid);
		displayTopMsg(downloadMessage);

		const char *ba_region = getBoxartRegion(dirContents[i].tid[3]);
		
		char boxartUrl[256];
		snprintf(boxartUrl, sizeof(boxartUrl), "https://art.gametdb.com/ds/coverDS/%s/%s.bmp", ba_region, dirContents[i].tid);
		char boxartPath[256];
		snprintf(boxartPath, sizeof(boxartPath), "/_nds/TWiLightMenu/boxart/%s.bmp", dirContents[i].tid);
		
		downloadToFile(boxartUrl, boxartPath);
	}

	chdir("sdmc:/_nds/TWiLightMenu/boxart/");
	getDirectoryContents(dirContents);

	displayTopMsg("Deleting blank files...");
	for(int i=0;i<(int)dirContents.size();i++) {
		if(dirContents[i].size == 0) {
			char path[256];
			snprintf(path, sizeof(path), "%s%s", "sdmc:/_nds/TWiLightMenu/boxart/", dirContents[i].name.c_str());
			deleteFile(path);
		}
	}
	doneMsg();
}

void updateLuma(bool nightly) {
	if(nightly) {
		displayTopMsg("Now Downloading Luma3DS\n" 
						"(Nightly)");
		if (downloadFromRelease("https://github.com/hax0kartik/luma-hourlies", "boot\\.firm", "/boot.firm") != 0) {
			downloadFailed();
			return;
		}

		setInstalledVersion("LUMA3DS-NIGHTLY", latestLumaNightly());
		saveUpdateData();
		updateAvailable[7] = false;
	} else {	
		displayTopMsg("Now Downloading Luma3DS\n"
						"(Release)");
		if (downloadFromRelease("https://github.com/AuroraWright/Luma3DS", "Luma3DS.*\\.7z", "/Luma3DS.7z") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("extracting Boot.firm\n"
						"(Release)");
		extractArchive("/Luma3DS.7z", "boot.firm", "/boot.firm");

		deleteFile("sdmc:/Luma3DS.7z");

		setInstalledVersion("LUMA3DS-RELEASE", latestLumaRelease());
		saveUpdateData();
		updateAvailable[6] = false;
	}
	doneMsg();
}

void updateCheckpoint(void) {
	displayTopMsg("Downloading Checkpoint.cia (Release)\n");
		if (downloadFromRelease("https://github.com/FlagBrew/Checkpoint", "Checkpoint\\.cia", "/Checkpoint-Release.cia") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Installing Checkpoint.cia\n"
						"(Release)");
		installCia("/Checkpoint-Release.cia");

		deleteFile("sdmc:/Checkpoint-Release.cia");

		setInstalledVersion("CHECKPOINT-RELEASE", latestCheckpointRelease());
		saveUpdateData();
		updateAvailable[10] = false;
	doneMsg();
}

void updateCheckpoint(void) {
	displayTopMsg("Downloading Checkpoint.cia (Nightly)\n");
		if (downloadToFile("https://github.com/FlagBrew/Checkpoint", "Checkpoint\\.cia", "/Checkpoint-Release.cia") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Installing Checkpoint.cia\n"
						"(Nighly)");
		installCia("/Checkpoint-Nightly.cia");

		deleteFile("sdmc:/Checkpoint-Nightly.cia");

		setInstalledVersion("CHECKPOINT-NIGHTLY", latestCheckpointNightly());
		saveUpdateData();
		updateAvailable[10] = false;
	doneMsg();
}

	void updateSelf(bool nightly) {
	if(nightly) {
		displayTopMsg("Downloading Universal-Updater\n" 
						"(Nightly)");
	if (downloadToFile("https://github.com/TWLBot/Builds/raw/master/extras/Universal/Universal-Updater.cia?raw=true", "/Universal-Updater-Nightly.cia") != 0) {
		downloadFailed();
		return;
	}

		displayTopMsg("Installing Universal-Updater CIA...\n"
						"\n\n\n\n\n\n\n\n\n"
						"The app will reboot when done.");

		installCia("/Universal-Updater-Nightly.cia");

		deleteFile("sdmc:/Universal-Updater-Nightly.cia");

		setInstalledChannel("UNIVERSAL-UPDATER", "nightly");
		setInstalledVersion("UNIVERSAL-UPDATER", latestUpdaterNightly());
		saveUpdateData();
		updateAvailable[13] = false;
	} else {
		displayTopMsg("Downloading Universal Updater...\n");

		if (downloadFromRelease("https://github.com/Universal-Team/Universal-Updater", "Universal-Updater\\.cia", "/Universal-Updater.cia") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Installing Universal-Updater CIA...\n"
						"\n\n\n\n\n\n\n\n\n"
						"The app will reboot when done.");

		installCia("/Universal-Updater.cia");

		deleteFile("sdmc:/Universal-Updater.cia");

		setInstalledChannel("UNIVERSAL-UPDATER", "release");
		setInstalledVersion("UNIVERSAL-UPDATER", latestUpdaterRelease());
		saveUpdateData();
		updateAvailable[12] = false;
	}
	doneMsg();
	}

void notImplementedYet(void) {
	notImplemented();
}

void downloadGodMode9(void) {
	displayTopMsg("Now Downloading GodMode9\n"
						"(Release)");
		if (downloadFromRelease("https://github.com/D0k3/GodMode9", "GodMode9.*\\.zip", "/GodMode9.zip") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Extracting GodMode9.firm\n"
						"(Release)");
		extractArchive("/GodMode9.zip", "GodMode9.firm", "/luma/payloads/GodMode9.firm");

		deleteFile("sdmc:/GodMode9.zip");

		setInstalledVersion("GODMODE9", latestGodMode9());
		saveUpdateData();
		updateAvailable[14] = false;
	doneMsg(); 
	}

void downloadGodMode9(void) {
	displayTopMsg("Now Downloading GodMode9\n"
						"(Nightly)");
		if (downloadToFile("https://github.com/D0k3/GodMode9", "GodMode9.*\\.zip", "/GodMode9.zip") != 0) {
			downloadFailed();
			return;
		}

		displayTopMsg("Extracting GodMode9.firm\n"
						"(Nightly)");
		extractArchive("/GodMode9.zip", "GodMode9.firm", "/luma/payloads/GodMode9.firm");

		deleteFile("sdmc:/GodMode9.zip");

		setInstalledVersion("GODMODE9", latestGodMode9());
		saveUpdateData();
		updateAvailable[14] = false;
	doneMsg(); 
	}
