#ifndef _CLI_HANDLER_HPP_
#define _CLI_HANDLER_HPP_

#include <sstream>

#include "../common/PDU.hpp"
#include "../common/socket_manager.hpp"
#include "../common/logger.hpp"
#include "../common/cmd_def.hpp"
#include "../common/common_def.h"
#include "../common/err_def.hpp"

#include "cmd_manager.hpp"
#include "help_def.hpp"

using namespace std;
namespace dpm
{
	class CliRequestHandler
	{
		const string _IPAddress;
		const int _Port;
		const bool _CreateSocket;
	public:
		CliRequestHandler(string _iPAddress, int _port, bool _createSocket = true);
		PDU  RequestPDU;
		char RequestBuf[MAX_PDU_BUFFER_LENGTH];
		char ResponseBuf[MAX_PDU_BUFFER_LENGTH];

		void handleCliRequest(int _argc, char** _args);
		bool checkVersion(ERR_ENUM& _err);
		bool executeRequest(ERR_ENUM& _err);
		void printResponse();
	};
}

#endif