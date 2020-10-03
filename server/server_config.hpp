#ifndef _SERVER_CONFIG_HPP_
#define _SERVER_CONFIG_HPP_

#include<sstream>
#include"../../libs/pugixml/pugixml.hpp"

#include "../common/cmd_def.hpp"
#include"../common/common_def.h"
#include "../common/err_def.hpp"
#include "../common/logger.hpp"
#include "../common/xmlconfig_def.h"

using namespace std;
using namespace pugi;

namespace dpm
{
	class ServerConfigurationParser
	{
	public:
		static void Parse(ERR_ENUM& _err, string _file = SERVER_CONFIG_DEFAULT_FILE_NAME);
	};

	class ServerConfigurationManager
	{
	private:
		static atomic<int> _Port;
		static atomic<int> _PollInterval;
		static string _logFilePath;
	public:

		static void SetPort(int _port);
		static void SetPollInterval(int _pollInterval);
		static void SetLogFilePath(string _filePath);

		static int GetPort();
		static int GetPollInterval();
		static atomic<int>* GetPollIntervalPtr();
		static string GetLogFilePath();

		static string GetSetting();
	};
}

#endif