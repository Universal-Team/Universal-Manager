
#include "datetime.h"

std::string getTime(void)
{
	char buf[80] = {0};
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	
	sprintf(buf, "%02i:%02i:%02i", timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
	
	std::string ret(buf);
	return ret;
}

std::string getPathDateTime(void)
{
	char buf[80] = {0};
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	
	sprintf(buf, "%04i%02i%02i-%02i%02i%02i", timeStruct->tm_year + 1900, timeStruct->tm_mon + 1, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
	
	std::string ret(buf);
	return ret;	
}