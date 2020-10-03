#ifndef _SETTINGS_CMD_HANDLER_HPP_
#define _SETTINGS_CMD_HANDLER_HPP_

#include "base_cmd_handler.hpp"

using namespace std;

namespace dpm
{
	class SettingsCmdHandler : public BaseCmdHandler
	{
	public:
		SettingsCmdHandler(RequestData& _requestData);
		void execute();
	};

}
#endif