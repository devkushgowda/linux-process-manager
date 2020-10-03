#ifndef _PROC_CMD_HANDLER_HPP_
#define _PROC_CMD_HANDLER_HPP_

#include "base_cmd_handler.hpp"

using namespace std;

namespace dpm
{
	class ProcCmdHandler : public BaseCmdHandler
	{
	private:
		void _Execute();

	public:
		ProcCmdHandler(RequestData& _requestData);
		void execute();
	};

}
#endif