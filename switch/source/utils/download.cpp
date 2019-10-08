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

#include "utils/download.hpp"

#include <curl/curl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#define  USER_AGENT   APP_TITLE "-" V_STRING

static char* result_buf = NULL;
static size_t result_sz = 0;
static size_t result_written = 0;
std::vector<std::string> _topText;

Result downloadToFile(std::string URL, std::string destination) {
	Result ret = 0;
	socketInitializeDefault();
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char *url = &URL[0]; // string used for curl_easy_setopt
	char *outfilename = &destination[0]; // file used for curl_easy_setopt
	curl_global_init(CURL_GLOBAL_DEFAULT); // initializing/calling functions
	curl = curl_easy_init(); // initializing/calling functions
	if (curl)
	{
		printf("\n\nURL = %s\nFile = %s\n\nDownload...\n", url, outfilename);
		fp = fopen(outfilename,"wb");printf("\n*DEBUG*\n");
			curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
			curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(curl, CURLOPT_STDERR, stdout);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

		res = curl_easy_perform(curl); // perform tasks curl_easy_setopt asked before
		if (res != CURLE_OK) {
			printf("\nfailed: %s\n", curl_easy_strerror(res));
		}
	curl_easy_cleanup(curl); // always cleanup
	fclose(fp); // closing FILE *fp
	}
	curl_global_cleanup();
	socketExit();
	return 0;
}


void downloadFailed(void) {
	Gui::displayMsg("Download Failed!");
	for (int i = 0; i < 60*2; i++) {
		//gspWaitForVBlank();
	}
}

void notImplemented(void) {
	Gui::displayMsg("Not Implemented Yet.");
	for (int i = 0; i < 60*2; i++) {
		//gspWaitForVBlank();
	}
}

void doneMsg(void) {
	Gui::displayMsg("Done!");
	for (int i = 0; i < 60*2; i++) {
		//gspWaitForVBlank();
	}
}

void notConnectedMsg(void) {
	Gui::displayMsg("Please Connect to WiFi.");
	for (int i = 0; i < 60*2; i++) {
		//gspWaitForVBlank();
	}
}

void updateUniversalManager(void) {
	Gui::displayMsg("Downloading Universal-Manager.nro... Please wait.");

	if (downloadToFile("https://github.com/Universal-Team/extras/blob/master/builds/Universal-Manager/Universal-Manager.nro?raw=true", "sdmc:/switch/Universal-Manager/Universal-Manager.nro") != 0) {
		downloadFailed();
		return;
	}
	doneMsg();
}