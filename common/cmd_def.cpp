#include "cmd_def.hpp"

using namespace dpm;

map<string, OPT_ENUM> CommandsDef::_OptionMap = {
	   STR_MAP(OPT_ADD),
	   STR_MAP(OPT_DELETE),
	   STR_MAP(OPT_UPDATE),
{OPT_UPDATE_SHORT_STRING,OPT_UPDATE},
	   STR_MAP(OPT_DISPLAY),
{OPT_DISPLAY_SHORT_STRING,OPT_DISPLAY},
	   STR_MAP(OPT_RESET),
	   STR_MAP(OPT_RELOAD),
	   STR_MAP(OPT_GROUP),
{OPT_GROUP_SHORT_STRING,OPT_GROUP},
	   STR_MAP(OPT_BY_PID),
{OPT_BY_PID_SHORT_STRING,OPT_BY_PID},

};

map<string, CMD_ENUM> CommandsDef::_CommandMap =
{
	STR_MAP(CMD_VERSION),
	STR_MAP(CMD_ALL),
	STR_MAP(CMD_START),
	STR_MAP(CMD_STOP),
	STR_MAP(CMD_RESTART),
	STR_MAP(CMD_PING),
	STR_MAP(CMD_CONFIG),
	STR_MAP(CMD_SETTING),
	STR_MAP(CMD_HELP)
};

map<string, ATTR_ENUM> CommandsDef::_AttrMap = {
	STR_MAP(ATTR_LOG),
	STR_MAP(ATTR_POLL)
};

CMD_ENUM CommandsDef::ParseCommandCode(string& _command)
{
	StringUtil::ToLower(_command);
	auto res = _CommandMap.find(_command);
	if (res != _CommandMap.end())
		return res->second;
	else
		return CMD_INVALID;
}

OPT_ENUM CommandsDef::ParseOptionCode(string& _opt)
{
	StringUtil::ToLower(_opt);
	auto res = _OptionMap.find(_opt);
	if (res != _OptionMap.end())
		return res->second;
	else
		return OPT_INVALID;
}

ATTR_ENUM CommandsDef::ParseAttrCode(string& _opt)
{
	StringUtil::ToLower(_opt);
	auto res = _AttrMap.find(_opt);
	if (res != _AttrMap.end())
		return res->second;
	else
		return ATTR_INVALID;
}