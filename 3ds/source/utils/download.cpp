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

#include "colors.hpp"
#include "gui.hpp"

#include "screens/screenCommon.hpp"

#include "utils/download.hpp"
#include "utils/extract.hpp"
#include "utils/fileBrowse.h"
#include "utils/inifile.h"
#include "utils/settings.hpp"
#include "utils/thread.hpp"

#include <sys/stat.h>
#include <vector>
#include <unistd.h>

extern "C" {
	#include "utils/cia.h"
}

#define  USER_AGENT   APP_TITLE "-" V_STRING

static char* result_buf = NULL;
static size_t result_sz = 0;
static size_t result_written = 0;
std::vector<std::string> _topText;
std::string jsonName;
extern bool is3dsx;
extern bool Is3dsxUpdated;
extern std::string path3dsx;

extern bool downloadNightlies;
extern int filesExtracted;
extern std::string extractingFile;

char progressBarMsg[128] = "";
bool showProgressBar = false;
bool progressBarType = 0; // 0 = Download | 1 = Extract
bool continueNdsScan = true;

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
		extractArchive("/TWiLightMenu-nightly.7z", "TWiLightMenu/3DS - CFW users/", "/");
		extractArchive("/TWiLightMenu-nightly.7z", "TWiLightMenu/DSi&3DS - SD card users/", "/");
		showProgressBar = false;

		DisplayMsg("Now Installing the CIAs..");
		installCia("/TWiLight Menu.cia");
		installCia("/TWiLight Menu - Game booter.cia");

		deleteFile("sdmc:/TWiLightMenu-nightly.7z");
		deleteFile("sdmc:/TWiLight Menu.cia");
		deleteFile("sdmc:/TWiLight Menu - Game booter.cia");
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
	}
	doneMsg();
}

void updateUniversalManager(bool nightly) {
	static bool success = false;
	if(nightly) {
		if (is3dsx == false) {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Universal-Manager CIA...\nNightly");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Universal-Manager/Universal-Manager.cia?raw=true", "/Universal-Manager-Nightly.cia") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}
			showProgressBar = false;
			DisplayMsg("Now Installing the CIA..");
			installCia("/Universal-Manager-Nightly.cia");
			deleteFile("sdmc:/Universal-Manager-Nightly.cia");

		} else if (is3dsx == true) {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Universal-Manager 3DSX...\nNightly");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Universal-Manager/Universal-Manager.3dsx?raw=true", path3dsx + "Universal-Manager.3dsx") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}
			showProgressBar = false;
			success = true;
		}
	} else {
		if (is3dsx == false) {
			DisplayMsg("Downloading Universal-Manager...\nRelease");
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Universal-Manager CIA...\nRelease");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/Universal-Manager", "Universal-Manager\\.cia", "/Universal-Manager-Release.cia") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}
			showProgressBar = false;
			DisplayMsg("Now Installing the CIA..");
			installCia("/Universal-Manager-Release.cia");
			deleteFile("sdmc:/Universal-Manager-Release.cia");
		} else if (is3dsx == true) {
			DisplayMsg("Downloading Universal-Manager...\nRelease");
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Universal-Manager 3DSX...\nRelease");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/Universal-Manager", "Universal-Manager\\.3dsx", path3dsx + "Universal-Manager.3dsx") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}
			showProgressBar = false;
			success = true;
		}
	}
	doneMsg();
	if (success == true) {
		if (is3dsx == true) {
			Is3dsxUpdated = true;
		}
	}
	success = false;
}

void updateLuma(bool nightly) {
	if(nightly) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Luma 3DS...\nNightly");
		showProgressBar = true;
		progressBarType = 0;
		 Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/hax0kartik/luma-hourlies", "boot\\.firm", "/boot.firm") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}
			showProgressBar = false;
	} else {	
		DisplayMsg("Downloading Luma 3DS...\nRelease");
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Luma 3DS...\nRelease");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/AuroraWright/Luma3DS", "Luma3DS.*\\.zip", "/Luma3DS.zip") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now extracting.\n"
						"(Release)\n\nThis may take a while.");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/Luma3DS.zip", "boot.firm", "/boot.firm");
		showProgressBar = false;

		deleteFile("sdmc:/Luma3DS.zip");
	}
	doneMsg();
}

void downloadGodMode9(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading GodMode9...\nRelease");
		showProgressBar = true;
		progressBarType = 0;
		 Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/D0k3/GodMode9", "GodMode9.*\\.zip", "/GodMode9.zip") != 0) {
			showProgressBar = false;
			downloadFailed();
			return;
		}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now extracting.\n"
						"(Release)\n\nThis may take a while.");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/GodMode9.zip", "GodMode9.firm", "/luma/payloads/GodMode9.firm");
		extractArchive("/GodMode9.zip", "gm9/", "/gm9/");
		showProgressBar = false;

		deleteFile("sdmc:/GodMode9.zip");
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
		themeText += "                B: Back   A: Choose";
		DisplayMsg(themeText.c_str());
	}
}

void downloadScripts(void) {
	int keyRepeatDelay = 0;

	DisplayMsg("Getting Script list...");

	std::vector<ThemeEntry> scriptList;
	scriptList = getThemeList("Universal-Team/extras", "Scripts");
	mkdir("sdmc:/Universal-Manager/scripts", 0777);

	int selectedScript = 0;
	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();
		if(keyRepeatDelay)	keyRepeatDelay--;
		if(hDown & KEY_A) {
			mkdir((scriptList[selectedScript].sdPath).c_str(), 0777);
			DisplayMsg(("Downloading: "+scriptList[selectedScript].name).c_str());
			downloadToFile(scriptList[selectedScript].downloadUrl, "sdmc:/Universal-Manager/scripts/"+scriptList[selectedScript].name);
		} else if(hDown & KEY_B) {
			selectedScript = 0;
			return;
		} else if(hHeld & KEY_UP && !keyRepeatDelay) {
			if(selectedScript > 0) {
				selectedScript--;
				keyRepeatDelay = 3;
			}
		} else if(hHeld & KEY_DOWN && !keyRepeatDelay) {
			if(selectedScript < (int)scriptList.size()-1) {
				selectedScript++;
				keyRepeatDelay = 3;
			}
		} else if(hHeld & KEY_LEFT && !keyRepeatDelay) {
			selectedScript -= 10;
			if(selectedScript < 0) {
				selectedScript = 0;
			}
			keyRepeatDelay = 3;
		} else if(hHeld & KEY_RIGHT && !keyRepeatDelay) {
			selectedScript += 10;
			if(selectedScript > (int)scriptList.size()) {
				selectedScript = scriptList.size()-1;
			}
			keyRepeatDelay = 3;
		}
		std::string scriptText;
		for(int i=(selectedScript<10) ? 0 : selectedScript-10;i<(int)scriptList.size()&&i<((selectedScript<10) ? 11 : selectedScript+1);i++) {
			if(i == selectedScript) {
				scriptText += "> " + scriptList[i].name + "\n";
			} else {
				scriptText += "  " + scriptList[i].name + "\n";
			}
		}
		for(uint i=0;i<((scriptList.size()<10) ? 11-scriptList.size() : 0);i++) {
			scriptText += "\n";
		}
		scriptText += "                B: Back   A: Choose";
		DisplayMsg(scriptText.c_str());
	}
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
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest CIA...\nRelease");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/Universal-Team/pkmn-chest", "pkmn-chest\\.cia", "/PKMN-Chest-Release.cia") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}

	showProgressBar = false;

		if(confirmPopup("Where do you want to place the downloaded NDS File?", "", "sdmc:/_nds/pkmn-chest/pkmn-chest.nds", "sdmc:/pkmn-chest.nds")) {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest NDS...\nRelease");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/pkmn-chest", "pkmn-chest\\.nds", "/_nds/pkmn-chest/pkmn-chest.nds") != 0) {
				showProgressBar = false;
				downloadFailed();
				return;
			}
		} else {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest NDS...\nRelease");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadFromRelease("https://github.com/Universal-Team/pkmn-chest", "pkmn-chest\\.nds", "/pkmn-chest.nds") != 0) {
				showProgressBar = false;
				downloadFailed();
				return;
			}
		}
		showProgressBar = false;

		DisplayMsg("Now Installing the CIA..");
		installCia("/PKMN-Chest-Release.cia");

		deleteFile("sdmc:/PKMN-Chest-Release.cia");
		doneMsg();
}

void updatePKMNChestNightly(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest CIA...\nNightly");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/pkmn-chest/pkmn-chest.cia?raw=true", "/PKMN-Chest-Nightly.cia") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}

	showProgressBar = false;

		if(confirmPopup("Where do you want to place the downloaded NDS File?", "", "sdmc:/_nds/pkmn-chest/pkmn-chest.nds", "sdmc:/pkmn-chest.nds")) {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest NDS...\nNightly");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/pkmn-chest/pkmn-chest.nds?raw=true", "/_nds/pkmn-chest/pkmn-chest.nds") != 0) {
				showProgressBar = false;
				downloadFailed();
				return;
			}
		} else {
			snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading PKMN-Chest NDS...\nNightly");
			showProgressBar = true;
			progressBarType = 0;
			Threads::create((ThreadFunc)displayProgressBar);
			if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/pkmn-chest/pkmn-chest.nds?raw=true", "/pkmn-chest.nds") != 0) {
				showProgressBar = false;
				downloadFailed();
				return;
			}
		}
		showProgressBar = false;

		DisplayMsg("Now Installing the CIA..");
		installCia("/PKMN-Chest-Nightly.cia");

		deleteFile("sdmc:/PKMN-Chest-Nightly.cia");
		doneMsg();
}

void updateRelaunchNightly(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Relaunch...\nNightly");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
	if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Relaunch.7z?raw=true", "/Relaunch-Nightly.7z") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now Extracting...\nNightly");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/Relaunch-Nightly.7z", "Relaunch/3DS/", "/");
		showProgressBar = false;
		DisplayMsg("Now Installing the CIA..");
		installCia("/Relaunch.cia");

		deleteFile("sdmc:/Relaunch-Nightly.7z");
		deleteFile("sdmc:/Relaunch.cia");
		doneMsg();
}

void updateRelaunchRelease(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading Relaunch...\nRelease");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
	if (downloadFromRelease("https://github.com/Universal-Team/Relaunch", "Relaunch\\.7z", "/Relaunch-Release.7z") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
}

		snprintf(progressBarMsg, sizeof(progressBarMsg), "Now Extracting...\nRelease");
		filesExtracted = 0;
		progressBarType = 1;
		extractArchive("/Relaunch-Release.7z", "Relaunch/3DS/", "/");
		showProgressBar = false;
		DisplayMsg("Now Installing the CIA..");
		installCia("/Relaunch.cia");

		deleteFile("sdmc:/Relaunch-Release.7z");
		deleteFile("sdmc:/Relaunch.cia");
		doneMsg();
}


void updateLeafEdit(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading LeafEdit...\nNightly");
		showProgressBar = true;
		progressBarType = 0;
		 Threads::create((ThreadFunc)displayProgressBar);
		if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/LeafEdit/LeafEdit.cia?raw=true", "/LeafEdit-Nightly.cia") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}
		showProgressBar = false;
		DisplayMsg("Now Installing the CIA..");
		installCia("/LeafEdit-Nightly.cia");

		deleteFile("sdmc:/LeafEdit-Nightly.cia");
	doneMsg();
}

void updateLeafEditRelease(void) {
		snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading LeafEdit...\nRelease");
		showProgressBar = true;
		progressBarType = 0;
		Threads::create((ThreadFunc)displayProgressBar);
		if (downloadFromRelease("https://github.com/Universal-Team/LeafEdit", "LeafEdit\\.cia", "/LeafEdit-Release.cia") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}
		showProgressBar = false;
		DisplayMsg("Now Installing the CIA..");
		installCia("/LeafEdit-Release.cia");

		deleteFile("sdmc:/LeafEdit-Release.cia");
	doneMsg();
}


// Boxarts for TWLMenu++. Ported from TWLMenu++ Updater.
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
			// if(region == CFG_REGION_USA) {
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

void scanToCancelBoxArt(void) {
	while(continueNdsScan) {
		hidScanInput();
		if(hidKeysDown() & KEY_B) {
			continueNdsScan = false;
		}
		gspWaitForVBlank();
	}
}

void downloadBoxart(void) {

	vector<DirEntry> dirContents;
	std::string scanDir;

	DisplayMsg("Would you like to choose a directory, or scan\nthe full card?\n\n\n\n\n\n\n\n\n\n       B: Cancel   A: Choose Directory   X: Full SD");

	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();

		if(hDown & KEY_A) {
			chdir("sdmc:/");
			bool dirChosen = false;
			uint selectedDir = 0;
			uint keyRepeatDelay = 0;
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
					const u32 hHeld = hidKeysHeld();
					if(keyRepeatDelay)	keyRepeatDelay--;
					if(hDown & KEY_A) {
						chdir(dirContents[selectedDir].name.c_str());
						selectedDir = 0;
						break;
					} else if(hDown & KEY_B) {
						char path[PATH_MAX];
						getcwd(path, PATH_MAX);
						if(strcmp(path, "sdmc:/") == 0)	return;
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
					} else if(hHeld & KEY_UP && !keyRepeatDelay) {
						if(selectedDir > 0) {
							selectedDir--;
							keyRepeatDelay = 3;
						}
					} else if(hHeld & KEY_DOWN && !keyRepeatDelay) {
						if(selectedDir < dirContents.size()-1) {
							selectedDir++;
							keyRepeatDelay = 3;
						}
					} else if(hHeld & KEY_LEFT && !keyRepeatDelay) {
						selectedDir -= 10;
						if(selectedDir < 0) {
							selectedDir = 0;
						}
						keyRepeatDelay = 3;
					} else if(hHeld & KEY_RIGHT && !keyRepeatDelay) {
						selectedDir += 10;
						if(selectedDir > dirContents.size()) {
							selectedDir = dirContents.size()-1;
						}
						keyRepeatDelay = 3;
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
					DisplayMsg(dirs.c_str());
				}
			}
			break;
		} else if(hDown & KEY_B) {
			return;
		} else if(hDown & KEY_X) {
			scanDir = "sdmc:/";
			break;
		}
	}

	DisplayMsg("Scanning SD card for DS roms...\n\n(Press B to cancel)");

	chdir(scanDir.c_str());
	continueNdsScan = true;
	Threads::create((ThreadFunc)scanToCancelBoxArt);
	findNdsFiles(dirContents);
	continueNdsScan = false;

	mkdir("sdmc:/_nds/TWiLightMenu/boxart/temp", 0777);
	for(int i=0;i<(int)dirContents.size();i++) {
		char path[256];
		snprintf(path, sizeof(path), "sdmc:/_nds/TWiLightMenu/boxart/%s.png", dirContents[i].tid);
		if(access(path, F_OK) != 0) {
			char downloadMessage[50];
			snprintf(downloadMessage, sizeof(downloadMessage), "Downloading \"%s.png\"...\n", dirContents[i].tid);
			DisplayMsg(downloadMessage);

			const char *ba_region = getBoxartRegion(dirContents[i].tid[3]);

			char boxartUrl[256];
			snprintf(boxartUrl, sizeof(boxartUrl), "https://art.gametdb.com/ds/coverS/%s/%s.png", ba_region, dirContents[i].tid);
			char boxartPath[256];
			snprintf(boxartPath, sizeof(boxartPath), "/_nds/TWiLightMenu/boxart/temp/%s.png", dirContents[i].tid);

			downloadToFile(boxartUrl, boxartPath);
		}
	}

	chdir("sdmc:/_nds/TWiLightMenu/boxart/temp/");
	getDirectoryContents(dirContents);

	DisplayMsg("Cleaning up...");
	for(int i=0;i<(int)dirContents.size();i++) {
		if(dirContents[i].size == 0) {
			char path[256];
			snprintf(path, sizeof(path), "sdmc:/_nds/TWiLightMenu/boxart/temp/%s", dirContents[i].name.c_str());
			deleteFile(path);
		} else {
			char tempPath[256];
			snprintf(tempPath, sizeof(tempPath), "sdmc:/_nds/TWiLightMenu/boxart/temp/%s", dirContents[i].name.c_str());
			char path[256];
			snprintf(path, sizeof(path), "sdmc:/_nds/TWiLightMenu/boxart/%s", dirContents[i].name.c_str());
			deleteFile(path);
			rename(tempPath, path);
		}
		rmdir("sdmc:/_nds/TWiLightMenu/boxart/temp");
	}
	doneMsg();
}

// GBARunner2.
void updateGBARunner2(void) 
{
	snprintf(progressBarMsg, sizeof(progressBarMsg), "Downloading GBARunner2... (Nightly)");
	showProgressBar = true;
	progressBarType = 0;
	Threads::create((ThreadFunc)displayProgressBar);

	if (downloadFromRelease("https://github.com/Gericom/GBARunner2", "GBARunner2_arm7dldi_3ds\\.nds", "/_nds/GBARunner2_arm7dldi_3ds.nds") != 0) {
		showProgressBar = false;
		downloadFailed();
		return;
	}
	showProgressBar = false;
	doneMsg();
}