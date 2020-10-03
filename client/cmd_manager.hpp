#ifndef _CMD_MANAGER_HPP_
#define _CMD_MANAGER_HPP_

#include <string>
#include <vector>

#include "../common/common_def.h"
#include "../common/cmd_def.hpp"
#include "../common/err_def.hpp"
#include "../common/PDU.hpp"

using namespace std;

namespace dpm
{
	class CommandManager
	{
	private:
		CMD_ENUM _CommandCode;
		int _OptCode;
		ERR_ENUM _ErrorCode;
		string _CommandString;
		string _OptString;
	public:
		CommandManager();
		~CommandManager();

		CMD_ENUM getCommandCode() const;
		int getOptionCode() const;
		ERR_ENUM getErrorCode() const;
		string getCommandString() const;
		string getOptionString() const;

		void makeCommandPDU(int _argCount, char** _args, PDU& _pdu);
	};
}

#endif
