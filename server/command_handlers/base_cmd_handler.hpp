#ifndef _BASE_CMD_HANDLER_HPP_
#define _BASE_CMD_HANDLER_HPP_

#include "../../common/cmd_def.hpp"
#include "../config_manager.hpp"
#include "../request_data.hpp"

using namespace std;

namespace dpm
{
	class BaseCmdHandler
	{
	protected:
		RequestData & _RequestData;
	public:
		BaseCmdHandler(RequestData& _requestData);
		virtual void execute() = 0;
	};
}
#endif