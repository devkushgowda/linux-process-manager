#ifndef _CLIENT_CONFIG_HPP_
#define _CLIENT_CONFIG_HPP_

#include<sstream>
#include"../libs/pugixml/pugixml.hpp"

#include"../common/common_def.h"
#include "../common/err_def.hpp"
#include "../common/logger.hpp"
#include "../common/xmlconfig_def.h"

using namespace std;
using namespace pugi;

namespace dpm
{
	class ClientConfigurationParser
	{
	public:
		static void Parse(ERR_ENUM& _err, string _file = CLIENT_CONFIG_DEFAULT_FILE_NAME);
	};

	class ClientConfigurationManager
	{
	private:
		static int _Port;
		static string _IpAddress;
	public:
		static int const& Port;
		static string const& IpAddress;

		static void SetPort(int _port);
		static void setIpAddress(string _ipAddress);


		static string GetSetting();
	};
}

#endif