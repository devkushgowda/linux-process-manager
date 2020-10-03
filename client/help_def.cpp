#include "help_def.hpp"

map<CMD_ENUM, string> HelpDef::_HelpMap = {
			HELP_MAP(CMD_ALL),
			HELP_MAP(CMD_START),
			HELP_MAP(CMD_STOP),
			HELP_MAP(CMD_RESTART),
			HELP_MAP(CMD_PING),
			HELP_MAP(CMD_CONFIG),
			HELP_MAP(CMD_SETTING),
			HELP_MAP(CMD_HELP)
};

string HelpDef::getCommandHelpMessage(CMD_ENUM _cmd)
{
	auto res = _HelpMap.find(_cmd);
	if (res != _HelpMap.end())
		return res->second;
	else
		return "Help code " + to_string(_cmd) + " not defined.";
}
