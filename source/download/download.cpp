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

#include "download/download.hpp"
#include <sys/stat.h>
#include <vector>
#include <unistd.h>

#include "download/extract.hpp"
#include "inifile.h"
#include "gui.hpp"
#include "fileBrowse.h"
#include "utils/settings.hpp"
#include "colors.hpp"
#include "download/thread.hpp"
#include "screens/screenCommon.hpp"

extern "C" {
	#include "download/cia.h"
}

#define  USER_AGENT   APP_TITLE "-" VERSION_STRING

static char* result_buf = NULL;
static size_t result_sz = 0;
static size_t result_written = 0;
std::vector<std::string> _topText;
std::string jsonName;
CIniFile versionsFile("sdmc:/Universal-Manager/currentVersions.ini");
std::string latestMenuReleaseCache = "";
std::string latestMenuNightlyCache = "";
std::string latestBootstrapReleaseCache = "";
std::string latestBootstrapNightlyCache = "";
std::string latestLumaReleaseCache = "";
std::string latestLumaNightlyCache = "";
std::string godMode9Cache = "";
std::string latestUniversalReleaseCache = "";
std::string latestUniversalNightlyCache = "";
std::string latestpkmnchestReleaseCache = "";
std::string latestpkmnchestNightlyCache = "";
std::string latestRelaunchReleaseCache = "";
std::string latestRelaunchNightlyCache = "";

extern bool downloadNightlies;
extern int filesExtracted;
extern std::string extractingFile;

char progressBarMsg[128] = "";
bool showProgressBar = false;
bool progressBarType = 0; // 0 = Download | 1 = Extract

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
	DisplayMsg("Download Failed!");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void notImplemented(void) {
	DisplayMsg("Not Implemented Yet.");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void doneMsg(void) {
	DisplayMsg("Done!");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void notConnectedMsg(void) {
	DisplayMsg("Please Connect to WiFi.");
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

std::vector<ThemeEntry> getThemeList(std::string repo, std::string path)
{
	Result ret = 0;
	void *socubuf = memalign(0x1000, 0x100000);
	std::vector<ThemeEntry> emptyVector;
	if (!socubuf)
	{
		return emptyVector;
	}

	ret = socInit((u32*)socubuf, 0x100000);
	if (R_FAILED(ret))
	{
		free(socubuf);
		return emptyVector;
	}
	
	std::stringstream apiurlStream;
	apiurlStream << "https://api.github.com/repos/" << repo << "/contents/" << path;
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
		return emptyVector;
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
		
		return emptyVector;
	}
	
	std::vector<ThemeEntry> jsonItems;
	json parsedAPI = json::parse(result_buf);
	for(uint i=0;i<parsedAPI.size();i++) {
		ThemeEntry themeEntry;
		if (parsedAPI[i]["name"].is_string()) {
			themeEntry.name = parsedAPI[i]["name"];
		}
		if (parsedAPI[i]["download_url"].is_string()) {
			themeEntry.downloadUrl = parsedAPI[i]["download_url"];
		}
		if (parsedAPI[i]["path"].is_string()) {
			themeEntry.sdPath = "sdmc:/";
			themeEntry.sdPath += parsedAPI[i]["path"];
			themeEntry.path = parsedAPI[i]["path"];

			size_t pos;
			while ((pos = themeEntry.path.find(" ")) != std::string::npos) {
				themeEntry.path.replace(pos, 1, "%20");
			}
		}
		jsonItems.push_back(themeEntry);
	}

	socExit();
	free(result_buf);
	free(socubuf);
	result_buf = NULL;
	result_sz = 0;
	result_written = 0;

	return jsonItems;
}

void downloadTheme(std::string path) {
	std::vector<ThemeEntry> themeContents = getThemeList("Universal-Team/extras", path);
	for(uint i=0;i<themeContents.size();i++) {
		if(themeContents[i].downloadUrl != "") {
			DisplayMsg(("Downloading: "+themeContents[i].name).c_str());
			downloadToFile(themeContents[i].downloadUrl, themeContents[i].sdPath);
		} else {
			DisplayMsg(("Downloading: "+themeContents[i].name).c_str());
			mkdir((themeContents[i].sdPath).c_str(), 0777);
			downloadTheme(themeContents[i].path);
		}
	}
}

void displayProgressBar() {
	char str[256];
	while(showProgressBar) {
		snprintf(str, sizeof(str), "%s\n%s%s\n%i %s", progressBarMsg, (!progressBarType ? "" : "\nCurrently extracting:\n"), (!progressBarType ? "" : extractingFile.c_str()), (!progressBarType ? (int)round(result_written/1000) : filesExtracted), (!progressBarType ? "KB downloaded." : (filesExtracted == 1 ? "file extracted." :"files extracted.")));
		DisplayMsg(str);
		gspWaitForVBlank();
	}
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

std::string latestGodMode9(void) {
	if (godMode9Cache == "")
		godMode9Cache = getLatestRelease("D0k3/GodMode9", "tag_name");
	return godMode9Cache;
}

std::string latestUniversalRelease(void) {
	if (latestUniversalReleaseCache == "")
		latestUniversalReleaseCache = getLatestRelease("Universal-Team/Universal-Manager", "tag_name");
	return latestUniversalReleaseCache;
}

std::string latestUniversalNightly(void) {
	if (latestUniversalNightlyCache == "")
		latestUniversalNightlyCache = getLatestCommit("Universal-Team/Universal-Manager", "sha").substr(0,7);
	return latestUniversalNightlyCache;
}

std::string latestpkmnchestRelease(void) {
	if (latestpkmnchestReleaseCache == "")
		latestpkmnchestReleaseCache = getLatestRelease("Universal-Team/pkmn-chest", "tag_name");
	return latestpkmnchestReleaseCache;
}

std::string latestpkmnchestNightly(void) {
	if (latestpkmnchestNightlyCache == "")
		latestpkmnchestNightlyCache = getLatestCommit("Universal-Team/pkmn-chest", "sha").substr(0,7);
	return latestpkmnchestNightlyCache;
}

std::string latestRelaunchRelease(void) {
	if (latestRelaunchReleaseCache == "")
		latestRelaunchReleaseCache = getLatestRelease("Universal-Team/Relaunch", "tag_name");
	return latestRelaunchReleaseCache;
}

std::string latestRelaunchNightly(void) {
	if (latestRelaunchNightlyCache == "")
		latestRelaunchNightlyCache = getLatestCommit("Universal-Team/Relaunch", "sha").substr(0,7);
	return latestRelaunchNightlyCache;
}

void saveUpdateData(void) {
	versionsFile.SaveIniFile("sdmc:/Universal-Manager/currentVersions.ini");
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
	std::string bootstrapRelease = getInstalledVersion("NDS-BOOTSTRAP-RELEASE");
	std::string boostrapNightly = getInstalledVersion("NDS-BOOTSTRAP-NIGHTLY");
	std::string lumaRelease = getInstalledVersion("LUMA3DS-RELEASE");
	std::string lumaNightly = getInstalledVersion("LUMA3DS-NIGHTLY");
	std::string godMode9Version = getInstalledVersion ("GODMODE9");
	std::string universalRelease = getInstalledVersion("UNIVERSAL-MANAGER-RELEASE");
	std::string universalNightly = getInstalledVersion("UNIVERSAL-MANAGER-NIGHTLY");
	std::string pkmnchestRelease = getInstalledVersion("PKMN-CHEST-RELEASE");
	std::string pkmnchestNightly = getInstalledVersion("PKMN-CHEST-NIGHTLY");
	std::string relaunchRelease = getInstalledVersion("RELAUNCH-RELEASE");
	std::string relaunchNightly = getInstalledVersion("RELAUNCH-NIGHTLY");

	if (menuChannel == "release")
		updateAvailable[0] = menuVersion != latestMenuRelease();
	else if (menuChannel == "nightly")
		updateAvailable[1] = menuVersion != latestMenuNightly();
	else
		updateAvailable[0] = updateAvailable[1] = true;

	updateAvailable[2] = bootstrapRelease != latestBootstrapRelease();
	updateAvailable[3] = boostrapNightly != latestBootstrapNightly();
	updateAvailable[5] = lumaRelease != latestLumaRelease();
	updateAvailable[6] = lumaNightly != latestLumaNightly();
	updateAvailable[7] = godMode9Version != latestGodMode9();
	updateAvailable[8] = universalRelease != latestUniversalRelease();
	updateAvailable[9] = universalNightly != latestUniversalNightly();
	updateAvailable[10] = pkmnchestRelease != latestpkmnchestRelease();
	updateAvailable[11] = pkmnchestNightly != latestpkmnchestNightly();
	updateAvailable[12] = relaunchRelease != latestRelaunchRelease();
	updateAvailable[13] = relaunchNightly != latestRelaunchNightly();
}


void updateBootstrap(bool nightly) {
	if(nightly) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading nds-bootstrap...\n(Nightly)");
		showProgressBar = true;
		progressBarType = 0;
		 Threads::create((ThreadFunc)displayProgressBar);
		if (downloadToFile("https://github.com/TWLBot/Builds/blob/master/nds-bootstrap.7z?raw=true", "/nds-bootstrap-nightly.7z") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Extracting nds-bootstrap...\n(Nightly)");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/nds-bootstrap-nightly.7z", "nds-bootstrap/", "/_nds/");
		showProgressBar = false;

		deleteFile("sdmc:/nds-bootstrap-nightly.7z");

		setInstalledVersion("NDS-BOOTSTRAP-NIGHTLY", latestBootstrapNightly());
		saveUpdateData();
		updateAvailable[3] = false; // For Later.
	} else {	
		DisplayMsg("Downloading nds-bootstrap...\n(Release)");
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading nds-bootstrap...\n(Release)");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/ahezard/nds-bootstrap", "nds-bootstrap\\.zip", "/nds-bootstrap-release.zip") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Extracting nds-bootstrap...\n(Release)");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/nds-bootstrap-release.zip", "/", "/_nds/");
		showProgressBar = false;

		deleteFile("sdmc:/nds-bootstrap-release.zip");

		setInstalledVersion("NDS-BOOTSTRAP-RELEASE", latestBootstrapRelease());
		saveUpdateData();
		updateAvailable[2] = false; // For Later.
	}
	doneMsg();
}


void updateTWiLight(bool nightly) {
	if(nightly) {
		snprintf(progressBarMsg, sizeof(progressBarMsg),"Now Downloading TWiLightMenu++\n"
						"(Nightly)\n\nThis may take a while.");
		showProgressBar = true;
		progressBarType = 0;
		 Threads::create((ThreadFunc)displayProgressBar);
		if (downloadToFile("https://github.com/TWLBot/Builds/blob/master/TWiLightMenu.7z?raw=true", "/TWiLightMenu-nightly.7z") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now extracting.\n"
						"(Nightly)\n\nThis may take a while.");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/TWiLightMenu-nightly.7z", "TWiLightMenu/_nds/", "/_nds/");
		extractArchive("/TWiLightMenu-nightly.7z", "3DS - CFW users/", "/");
		showProgressBar = false;

		DisplayMsg("Now Installing the CIAs..");
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
		DisplayMsg("Downloading TWiLightMenu++\n"
						"(Release)\n\nThis may take a while.");
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading TWiLightMenu++\n"
						"(Release)\n\nThis may take a while.");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/DS-Homebrew/TWiLightMenu", "TWiLightMenu\\.7z", "/TWiLightMenu-release.7z") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now extracting.\n"
						"(Release)\n\nThis may take a while.");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/TWiLightMenu-release.7z", "_nds/", "/_nds/");
		extractArchive("/TWiLightMenu-release.7z", "3DS - CFW users/", "/");
		extractArchive("/TWiLightMenu-release.7z", "DSi&3DS - SD card users/", "/");
		showProgressBar = false;

		DisplayMsg("Now Installing the CIAs..");
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

void updateUniversalManager(bool nightly) {
	if(nightly) {
		DisplayMsg("Downloading Universal-Manager...\nNightly");
		if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Universal-Manager/Universal-Manager.cia?raw=true", "/Universal-Manager-Nightly.cia") != 0) {
		downloadFailed();
		return;
	}

		DisplayMsg("Now Installing the CIAs..");
		installCia("/Universal-Manager-Nightly.cia");

		deleteFile("sdmc:/Universal-Manager-Nightly.cia");

		setInstalledVersion("UNIVERSAL-MANAGER-NIGHTLY", latestUniversalNightly());
		saveUpdateData();
		updateAvailable[9] = false;
	} else {
		DisplayMsg("Downloading Universal-Manager...\nRelease");
		if (downloadFromRelease("https://github.com/Universal-Team/Universal-Manager", "Universal-Manager\\.cia", "/Universal-Manager-Release.cia") != 0) {
		downloadFailed();
		return;
	}

		DisplayMsg("Now Installing the CIAs..");
		installCia("/Universal-Manager-Release.cia");

		deleteFile("sdmc:/Universal-Manager-Release.cia");

		setInstalledVersion("UNIVERSAL-MANAGER-RELEASE", latestUniversalRelease());
		saveUpdateData();
		updateAvailable[8] = false; 
}
doneMsg();
}

void updateLuma(bool nightly) {
	if(nightly) {
		DisplayMsg("Downloading Luma 3DS...\nNightly");
		if (downloadFromRelease("https://github.com/hax0kartik/luma-hourlies", "boot\\.firm", "/boot.firm") != 0) {
			downloadFailed();
			return;
		}

		setInstalledVersion("LUMA3DS-NIGHTLY", latestLumaNightly());
		saveUpdateData();
		updateAvailable[6] = false;
	} else {	
		DisplayMsg("Downloading Luma 3DS...\nRelease");
		if (downloadFromRelease("https://github.com/AuroraWright/Luma3DS", "Luma3DS.*\\.zip", "/Luma3DS.zip") != 0) {
			downloadFailed();
			return;
		}

		DisplayMsg("Extracting Luma3DS...");
		extractArchive("/Luma3DS.zip", "boot.firm", "/boot.firm");

		deleteFile("sdmc:/Luma3DS.zip");

		setInstalledVersion("LUMA3DS-RELEASE", latestLumaRelease());
		saveUpdateData();
		updateAvailable[5] = false;
	}
	doneMsg();
}

void downloadGodMode9(void) {
	DisplayMsg("Downloading GodMode9...\nRelease");
		if (downloadFromRelease("https://github.com/D0k3/GodMode9", "GodMode9.*\\.zip", "/GodMode9.zip") != 0) {
			downloadFailed();
			return;
		}

		DisplayMsg("Extracting GodMode9...");
		extractArchive("/GodMode9.zip", "GodMode9.firm", "/luma/payloads/GodMode9.firm");
		extractArchive("/GodMode9.zip", "gm9/", "/gm9/");

		deleteFile("sdmc:/GodMode9.zip");

		setInstalledVersion("GODMODE9", latestGodMode9());
		saveUpdateData();
		updateAvailable[7] = false;
	doneMsg(); 
}
void downloadThemes(void) {
	int keyRepeatDelay = 0;

	DisplayMsg("Getting theme list...");

	std::vector<ThemeEntry> themeList;
	themeList = getThemeList("Universal-Team/extras", "Themes");
	mkdir("sdmc:/Universal-Manager/Themes", 0777);

	int selectedTheme = 0;
	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();
		if(keyRepeatDelay)	keyRepeatDelay--;
		if(hDown & KEY_A) {
			mkdir((themeList[selectedTheme].sdPath).c_str(), 0777);
			DisplayMsg(("Downloading: "+themeList[selectedTheme].name).c_str());
			downloadToFile(themeList[selectedTheme].downloadUrl, "sdmc:/Universal-Manager/Themes/"+themeList[selectedTheme].name);
		} else if(hDown & KEY_B) {
			selectedTheme = 0;
			return;
		} else if(hHeld & KEY_UP && !keyRepeatDelay) {
			if(selectedTheme > 0) {
				selectedTheme--;
				keyRepeatDelay = 3;
			}
		} else if(hHeld & KEY_DOWN && !keyRepeatDelay) {
			if(selectedTheme < (int)themeList.size()-1) {
				selectedTheme++;
				keyRepeatDelay = 3;
			}
		} else if(hHeld & KEY_LEFT && !keyRepeatDelay) {
			selectedTheme -= 10;
			if(selectedTheme < 0) {
				selectedTheme = 0;
			}
			keyRepeatDelay = 3;
		} else if(hHeld & KEY_RIGHT && !keyRepeatDelay) {
			selectedTheme += 10;
			if(selectedTheme > (int)themeList.size()) {
				selectedTheme = themeList.size()-1;
			}
			keyRepeatDelay = 3;
		}
		std::string themeText;
		for(int i=(selectedTheme<10) ? 0 : selectedTheme-10;i<(int)themeList.size()&&i<((selectedTheme<10) ? 11 : selectedTheme+1);i++) {
			if(i == selectedTheme) {
				themeText += "> " + themeList[i].name + "\n";
			} else {
				themeText += "  " + themeList[i].name + "\n";
			}
		}
		for(uint i=0;i<((themeList.size()<10) ? 11-themeList.size() : 0);i++) {
			themeText += "\n";
		}
		themeText += "				B: Back   A: Choose";
		DisplayMsg(themeText.c_str());
	}
}

void downloadScripts(void) {
DisplayMsg("Downloading latest Scripts Package...");
		if (downloadToFile("https://cdn.discordapp.com/attachments/596873160083636274/596874729277882381/scripts.zip?raw=true", "/Scripts.zip") != 0) {
		downloadFailed();
		return;
	}
	extractArchive("/Scripts.zip", "/", "/Universal-Manager/Scripts/");
	deleteFile("sdmc:/Scripts.zip");
	doneMsg();
}

void updateCheats(void) {
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading DSJ's cheat database...");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
	if (downloadToFile("https://github.com/TWLBot/Builds/raw/master/usrcheat.dat.7z?raw=true", "/usrcheat.dat.7z") != 0) {
		downloadFailed();
		return;
	}

	snprintf(progressBarMsg, sizeof(progressBarMsg), "Extracting DSJ's cheat database...");
	filesExtracted = 0;
	progressBarType = 1;
	extractArchive("/usrcheat.dat.7z", "usrcheat.dat", "/_nds/TWiLightMenu/extras/usrcheat.dat");
	showProgressBar = false;

	deleteFile("sdmc:/usrcheat.dat.7z");

	doneMsg();
}

void updatePKMNChestRelease(void) {
	DisplayMsg("Downloading PKMN-Chest...\nRelease");
		if (downloadFromRelease("https://github.com/Universal-Team/pkmn-chest", "pkmn-chest\\.cia", "/PKMN-Chest-Release.cia") != 0) {
		downloadFailed();
		return;
	}

	if (downloadFromRelease("https://github.com/Universal-Team/pkmn-chest", "pkmn-chest\\.nds", "/_nds/pkmn-chest/pkmn-chest.nds") != 0) {
		downloadFailed();
		return;
	}

		DisplayMsg("Now Installing the CIAs..");
		installCia("/PKMN-Chest-Release.cia");

		deleteFile("sdmc:/PKMN-Chest-Release.cia");

		setInstalledVersion("PKMN-CHEST-RELEASE", latestpkmnchestRelease());
		saveUpdateData();
		updateAvailable[10] = false;
		doneMsg();
}

void updatePKMNChestNightly(void) {
	DisplayMsg("Downloading PKMN-Chest...\nNightly");
		if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/pkmn-chest/pkmn-chest.cia?raw=true", "/PKMN-Chest-Nightly.cia") != 0) {
		downloadFailed();
		return;
	}

	if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/pkmn-chest/pkmn-chest.nds?raw=true", "/_nds/pkmn-chest/pkmn-chest.nds") != 0) {
		downloadFailed();
		return;
	}

		DisplayMsg("Now Installing the CIAs..");
		installCia("/PKMN-Chest-Nightly.cia");

		deleteFile("sdmc:/PKMN-Chest-Nightly.cia");

		setInstalledVersion("PKMN-CHEST-NIGHTLY", latestpkmnchestNightly());
		saveUpdateData();
		updateAvailable[11] = false;
		doneMsg();
}

void updateRelaunchNightly(void) {
	DisplayMsg("Downloading Relaunch...\nNightly");
	if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Relaunch.7z?raw=true", "/Relaunch-Nightly.7z") != 0) {
		downloadFailed();
		return;
	}

		DisplayMsg("Extracting Relaunch...");
		extractArchive("/Relaunch-Nightly.7z", "Relaunch/3DS/", "/");
		installCia("/Relaunch.cia");

		deleteFile("sdmc:/Relaunch-Nightly.7z");
		deleteFile("sdmc:/Relaunch.cia");
		setInstalledVersion("RELAUNCH-NIGHTLY", latestRelaunchNightly());
		saveUpdateData();
		updateAvailable[13] = false;
		doneMsg();
}

void updateRelaunchRelease(void) {
	DisplayMsg("Downloading Relaunch...\nRelease");
	if (downloadFromRelease("https://github.com/Universal-Team/Relaunch", "Relaunch\\.7z", "/Relaunch-Release.7z") != 0) {
		downloadFailed();
		return;
}

		DisplayMsg("Extracting Relaunch...");
		extractArchive("/Relaunch-Release.7z", "Relaunch/3DS/", "/");
		installCia("/Relaunch.cia");

		deleteFile("sdmc:/Relaunch-Release.7z");
		deleteFile("sdmc:/Relaunch.cia");
		setInstalledVersion("RELAUNCH-RELEASE", latestRelaunchRelease());
		saveUpdateData();
		updateAvailable[12] = false;
		doneMsg();
}
