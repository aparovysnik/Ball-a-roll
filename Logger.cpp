#include "Logger.h"
#include <stdarg.h>
#include "Windows.h"
#define DEBUG


Logger::Logger()
{
	Init();
	//the constructor doesn't do anything, but we need
	//it for our singleton to work correctly
}

bool Logger::Init()
{
	appLog.open("applog.txt");
	clientLog.open("clntlog.txt");
	serverLog.open("srvrlog.txt");
	//user errors get logged to client

	//load the strings file
	if (!LoadStrings())
		return false;

	return true;
}

void Logger::Write(int target, const char *msg, ...)
{
	va_list args; va_start(args, msg);
	char szBuf[1024];
	vsprintf(szBuf, msg, args);

	if (target&LOG_APP)
	{
		appLog << szBuf << "\n";
#ifdef DEBUG
		appLog.flush();
#endif
	}
	if (target&LOG_CLIENT)
	{
		clientLog << szBuf << "\n";
#ifdef DEBUG
		clientLog.flush();
#endif
	}
	if (target&LOG_SERVER)
	{
		serverLog << szBuf << "\n";
#ifdef DEBUG
		serverLog.flush();
#endif
	}
	if (target&LOG_USER)
	{
#ifdef WIN32
		MessageBox(NULL, szBuf, "Message", MB_OK);
#else
#error User-level logging is not yet implemented for this platform.
#endif
	}
}

void Logger::Write(int target, unsigned long msgID, ...)
{
	va_list args;
	va_start(args, msgID);
	char szBuf[1024];
	vsprintf(szBuf, logStrings[msgID].c_str(), args);
	Write(target, szBuf);
}

#ifdef WIN32
//under Win32, the strings get read in from a string table resource
bool Logger::LoadStrings()
{
	for (unsigned long i = 0; i<MAX_LOG_STRINGS; i++)
	{
		char szBuf[1024];
		if (!LoadString(GetModuleHandle(NULL), i, szBuf, 1024))
			break; //returning 0 means no more strings
		logStrings[i] = szBuf;
	}
	return true;
}

#else
//other platforms load the strings in from strings.txt
bool Logger::LoadStrings()
{
	std::ifstream in("strings.txt");
	if (!in.is_open())return false;

	DWORD index = 0;

	while (!in.eof())
	{
		char szBuf[1024];
		in.getline(szBuf, 1024);
		stringsFile[index++] = szBuf;
	}

	return true;
}

#endif
