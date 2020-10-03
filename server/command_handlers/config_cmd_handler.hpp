#ifndef _CONFIG_CMD_HANDLER_HPP_
#define _CONFIG_CMD_HANDLER_HPP_

#include "base_cmd_handler.hpp"

using namespace std;

namespace dpm
{
	class ConfigCmdHandler : public BaseCmdHandler
	{
	public:
		ConfigCmdHandler(RequestData& _requestData);
		void execute();
	};

}
#endif