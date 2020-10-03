#ifndef _REQUEST_HANDLER_HPP_
#define _REQUEST_HANDLER_HPP_

#include <string>
#include <sstream>
#include <thread>

#include "../common/PDU.hpp"
#include "../common/common_def.h"
#include "../common/err_def.hpp"
#include "../common/socket_manager.hpp"
#include "../common/logger.hpp"
#include "../common/exception_def.hpp"

#include "command_handlers/proc_cmd_handler.hpp"
#include "command_handlers/config_cmd_handler.hpp"
#include "command_handlers/settings_cmd_handler.hpp"

#include "config_manager.hpp"
#include "request_data.hpp"

using namespace std;

namespace dpm
{
	class RequestHandler
	{
	private:
		static void _ExecuteRequest(const int& _sockFD);
	public:
		static void ServeRequest(int _sockFD);
	};
}
#endif