/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/Debug.h>
#include <time.h>

// Max string size : (wide screan console nb caractere)
#define EDN_LOG_MAX_LENGTH 250


#define FUNCTION_NAME_SIZE	(70)

void TOOLS_DisplayFuncName(int32_t ligne, const char* className, const char* funcName, const char* libName)
{
	char tmpName[FUNCTION_NAME_SIZE] = "";
	
	if (NULL == className) {
		if (NULL == libName) {
			snprintf(tmpName, FUNCTION_NAME_SIZE-1, "???????? | (l=%5d) %s                                             ",ligne, funcName);
		} else {
			snprintf(tmpName, FUNCTION_NAME_SIZE-1, "%s | (l=%5d) %s                                             ",libName, ligne, funcName);
		}
	} else {
		if (NULL == libName) {
			snprintf(tmpName, FUNCTION_NAME_SIZE-1, "???????? | (l=%5d) %s::%s                                             ",ligne, className, funcName);
		} else {
			snprintf(tmpName, FUNCTION_NAME_SIZE-1, "%s | (l=%5d) %s::%s                                             ", libName, ligne, className, funcName);
		}
	}
	tmpName[FUNCTION_NAME_SIZE-4] = ' ';
	tmpName[FUNCTION_NAME_SIZE-3] = '|';
	tmpName[FUNCTION_NAME_SIZE-2] = ' ';
	tmpName[FUNCTION_NAME_SIZE-1] = '\0';
	etk::cout << tmpName;
}

void TOOLS_DisplayTime(void)
{
	char tmpdata[50];
#ifdef __TARGET_OS__Android
	struct timeval  now;
	gettimeofday(&now, NULL);
	sprintf(tmpdata, " %2dh %2dmin %2ds | ", (int32_t)(now.tv_sec/3600)%24, (int32_t)(now.tv_sec/60)%60, (int32_t)(now.tv_sec%60));
#else
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(tmpdata, " %2dh %2dmin %2ds | ", (timeinfo->tm_hour)%24, timeinfo->tm_min, timeinfo->tm_sec);
#endif
	etk::cout << tmpdata ;
}


#ifdef __TARGET_OS__Android
etk::logLevel_te g_requestedLevel = etk::LOG_LEVEL_VERBOSE;
#else
etk::logLevel_te g_requestedLevel = etk::LOG_LEVEL_ERROR;
#endif
void GeneralDebugSetLevel(etk::logLevel_te ccc) {
	g_requestedLevel = ccc;
}

