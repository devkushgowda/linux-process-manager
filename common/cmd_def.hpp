#ifndef _CMD_DEF_HPP_
#define _CMD_DEF_HPP_


#include <map>
#include <string>
#include <iterator>

#include "utils.hpp"

using namespace std;

namespace dpm
{
	//Command map def
#define STR_MAP(X) {X##_STRING,X}



	// Commands code enum

#define CMD_VERSION_STRING "version"
#define CMD_ALL_STRING "all"
#define CMD_START_STRING "start"
#define CMD_STOP_STRING "stop"
#define CMD_RESTART_STRING "restart"
#define CMD_PING_STRING "ping"
#define CMD_CONFIG_STRING "config"
#define CMD_SETTING_STRING "setting"
#define CMD_HELP_STRING "help"

	enum CMD_ENUM
	{
		CMD_INVALID = 1,
		CMD_VERSION,
		CMD_ALL,
		CMD_START,
		CMD_STOP,
		CMD_RESTART,
		CMD_PING,
		CMD_CONFIG,
		CMD_SETTING,
		CMD_HELP,
		CMD_EXIT
	};

	// Option codes enum

#define OPT_ADD_STRING "-add"
#define OPT_DELETE_STRING "-delete"
#define OPT_UPDATE_STRING "-update"
#define OPT_UPDATE_SHORT_STRING "-u"
#define OPT_DISPLAY_STRING "-display"
#define OPT_DISPLAY_SHORT_STRING "-d"
#define OPT_RESET_STRING "-reset"
#define OPT_RELOAD_STRING "-reload"
#define OPT_GROUP_STRING "-group"
#define OPT_GROUP_SHORT_STRING "-g"
#define OPT_GROUP_SHORT1_STRING "-gid"
#define OPT_BY_PID_STRING "-pid"
#define OPT_BY_PID_SHORT_STRING "-p"

	enum OPT_ENUM
	{
		OPT_INVALID = -1,
		OPT_ALL = 1,
		OPT_BY_ID,
		OPT_ADD,
		OPT_DELETE,
		OPT_UPDATE,
		OPT_DISPLAY,
		OPT_RESET,
		OPT_RELOAD,
		OPT_GROUP,
		OPT_BY_PID,
	};
#define ATTR_POLL_STRING "-poll"
#define ATTR_LOG_STRING "-log"

	enum ATTR_ENUM {
		ATTR_INVALID = -1,
		ATTR_LOG = 1,
		ATTR_POLL
	};


	class CommandsDef
	{
	private:
		static map<string, CMD_ENUM> _CommandMap;
		static map<string, OPT_ENUM> _OptionMap;
		static map<string, ATTR_ENUM> _AttrMap;
	public:
		static CMD_ENUM ParseCommandCode(string& _command);
		static OPT_ENUM ParseOptionCode(string& _opt);
		static ATTR_ENUM ParseAttrCode(string& _opt);
	};
}
#endif