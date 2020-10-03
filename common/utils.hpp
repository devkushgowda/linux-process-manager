#ifndef _UTILS_HPP_
#define _UTILS_HPP_


#include <fstream>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "logger.hpp"

using namespace std;


/* ANSI COLORIZERS */
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define KBLD "\x1B[1m"
#define KUDL "\x1B[4m"

#define CRED(x) KRED x RST
#define CGRN(x) KGRN x RST
#define CYEL(x) KYEL x RST
#define CBLU(x) KBLU x RST
#define CMAG(x) KMAG x RST
#define CCYN(x) KCYN x RST
#define CWHT(x) KWHT x RST

#define CBOLD(x) KBLD x RST
#define CUNDL(x) KUDL x RST


namespace dpm
{
	class StringUtil
	{
	public:
		static void ToLower(string& _str);
		static void ToUpper(string& _str);
		static bool EndsWith(string const& _str, string const& _ending);
		static bool StartsWith(string const& _str, string const& _starting);
		static bool Contains(string const& _str, string const& _subStr);
	};

	class Utils
	{
	public:
		static string GetTimeIntervalString(time_t const& _time);
		static int GetFileSize(string const& _fileName);
		static bool IsFileExists(string const& _fileName);
		static bool IsDirectoryExists(string const& _dirName);
		static bool CreateDirectoryTreeIfNotExists(string const& _dirPath);
		static bool GetCurrentExePath(string& _exePath);
		static bool GetCurrentExeDir(string& _exeDir, bool _withEndingSeperator = true);
		static bool GetAbsoluteFilePath(string const& _filePath, string& _absolutepath);
		static bool IsValidIpAddress(string const& _ipAddress);
		static bool ResolveRootPath(string& _filePath);
	};

	class ConsoleUtil
	{
	public:
		static string Red(string _str);
		static string Yellow(string _str);
		static string Green(string _str);
		static string Blue(string _str);
		static string White(string _str);
		static string Magenta(string _str);

		static  string Bold(string _str);
		static  string UnderLine(string _str);
	};
}

#endif
