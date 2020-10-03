#include "cmd_manager.hpp"

using namespace dpm;

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

CMD_ENUM CommandManager::getCommandCode() const
{
	return _CommandCode;
}

int CommandManager::getOptionCode() const
{
	return _OptCode;
}

ERR_ENUM CommandManager::getErrorCode() const
{
	return _ErrorCode;
}

string CommandManager::getCommandString() const
{
	return _CommandString;
}

string CommandManager::getOptionString() const
{
	return _OptString;
}

void CommandManager::makeCommandPDU(int _argCount, char** _args, PDU& _pdu)
{
	_ErrorCode = SUCCESS;

	_pdu.setFunctionCode((short)CMD_INVALID);

	if (_argCount <= 0)
	{
		_ErrorCode = ERR_CLI_INSUFFICIENT_ARGS;
		return;
	}
	_CommandString = _args[0];
	_CommandCode = CommandsDef::ParseCommandCode(_CommandString);

	switch (_CommandCode)
	{
	case CMD_STOP:
	case CMD_START:
	case CMD_RESTART:
	case CMD_PING:
	{
		_pdu.setFunctionCode((short)_CommandCode);
		if (_argCount == 1)
		{
			_OptCode = OPT_ALL;
			_pdu.addParameter(_OptCode);
			_pdu.addParameter(PROCESS_CONFIG_DEFAULT_GROUP_ID);
		}
		else if (_argCount == 2)
		{
			_OptCode = OPT_BY_ID;
			_pdu.addParameter(_OptCode);
			_pdu.addParameter(PROCESS_CONFIG_DEFAULT_GROUP_ID);
			_pdu.addParameter(_args[1]);
		}
		else if (_argCount == 3)
		{
			string opt = _args[1];
			auto option = CommandsDef::ParseOptionCode(opt);
			switch (option)
			{
			case OPT_BY_PID:
				if (_CommandCode == CMD_STOP)
				{
					_OptCode = OPT_BY_PID;
					_pdu.addParameter(_OptCode);
					_pdu.addParameter(_args[2]);
				}
				else
				{
					_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
				}
				break;
			case OPT_GROUP:
				_OptCode = OPT_ALL;
				_pdu.addParameter(_OptCode);
				_pdu.addParameter(_args[2]);
				break;
			default:
				_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
			}
		}
		else if (_argCount == 4)
		{
			string opt = _args[2];
			auto option = CommandsDef::ParseOptionCode(opt);
			if (option == OPT_GROUP)
			{
				_OptCode = OPT_BY_ID;
				_pdu.addParameter(_OptCode);
				_pdu.addParameter(_args[3]);
				_pdu.addParameter(_args[1]);
			}
		}
		else
		{
			_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
		}
		break;
	}
	case CMD_CONFIG:
	{
		_pdu.setFunctionCode((short)CMD_CONFIG);

		if (_argCount > 1)
		{
			_OptString = _args[1];
			_OptCode = CommandsDef::ParseOptionCode(_OptString);

			switch (_OptCode)
			{
			case OPT_DISPLAY:
			{
				_pdu.addParameter(_OptCode);

				if (_argCount == 2)
				{
					_OptCode = OPT_ALL;
					_pdu.addParameter(_OptCode);
				}
				else if (_argCount == 3)
				{
					_OptCode = OPT_BY_ID;
					_pdu.addParameter(_OptCode);
					_pdu.addParameter(_args[2]);
				}
				else
				{
					_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
				}
				break;
			}
			case OPT_RELOAD:
			{
				_OptCode = OPT_RELOAD;
				_pdu.addParameter(_OptCode);
				break;
			}
			default:
				_ErrorCode = ERR_CLI_INVALID_OPTION;
				break;
			}
		}
		else
		{
			_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
		}
		break;
	}
	case CMD_SETTING:
	{
		_pdu.setFunctionCode((short)CMD_SETTING);

		if (_argCount > 1)
		{
			_OptString = _args[1];
			_OptCode = CommandsDef::ParseOptionCode(_OptString);

			switch (_OptCode)
			{
			case OPT_DISPLAY:
			{
				_pdu.addParameter(_OptCode);
				if (_argCount == 2)
				{
					_OptCode = OPT_ALL;
					_pdu.addParameter(_OptCode);
				}
				else if (_argCount == 3)
				{
					_OptCode = OPT_BY_ID;
					_pdu.addParameter(_OptCode);
					_pdu.addParameter(_args[2]);
				}
				else
				{
					_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
				}
				break;
			}
			case OPT_UPDATE:
			{
				_pdu.addParameter(_OptCode);
				if (_argCount == 4)
				{
					string attrString = _args[2];
					auto attr = CommandsDef::ParseAttrCode(attrString);
					switch (attr)
					{
					case ATTR_LOG:
					{
						auto logMode = Logger::ToLogMode(_args[3]);
						if (logMode != LOG_INVALID)
						{
							_pdu.addParameter(attr);
							_pdu.addParameter(logMode);
						}
						else
						{
							_ErrorCode = ERR_CLI_INVALID_ATTR_LOG;
						}
						break;
					}
					case ATTR_POLL:
					{
						auto pollInterval = atoi(_args[3]);
						if (pollInterval >= PROCESS_MIN_POLL_INTERVAL && pollInterval <= PROCESS_MAX_POLL_INTERVAL)
						{
							_pdu.addParameter(attr);
							_pdu.addParameter(pollInterval);
						}
						else
						{
							_ErrorCode = ERR_CLI_INVALID_ATTR_POLL;
						}
						break;
					}
					default:
						_ErrorCode = ERR_CLI_INVALID_ATTR_INVALID;

					}
				}
				else
				{
					_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
				}
				break;
			}
			
			case OPT_RESET:
			{
				_pdu.addParameter(_OptCode);

				if (_argCount == 2)
				{
					_OptCode = OPT_ALL;
					_pdu.addParameter(_OptCode);
				}
				else if (_argCount == 3)
				{
					_OptCode = OPT_BY_ID;
					_pdu.addParameter(_OptCode);
					string attrString = _args[2];
					auto attr = CommandsDef::ParseAttrCode(attrString);
					if (attr != ATTR_INVALID)
					{
						_pdu.addParameter(attr);
					}
					else
					{
						_ErrorCode = ERR_CLI_INVALID_ATTR_INVALID;
					}
				}
				else
				{
					_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
				}
				break;
			}
			default:
				_ErrorCode = ERR_CLI_INVALID_OPTION;
				break;
			}
		}
		else
		{
			_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
		}
		break;
	}
	case CMD_HELP:
	{
		_pdu.setFunctionCode((short)CMD_HELP);

		if (_argCount == 1)
		{
			_OptCode = CMD_ALL;
			_pdu.addParameter(_OptCode);
		}
		else if (_argCount == 2)
		{
			_OptString = _args[1];

			auto cmdCode = CommandsDef::ParseCommandCode(_OptString);

			switch (cmdCode)
			{
			case CMD_START:
			case CMD_STOP:
			case CMD_RESTART:
			case CMD_PING:
			case CMD_CONFIG:
			case CMD_SETTING:
			case CMD_HELP:
				_OptCode = cmdCode;
				break;
			default:
			{
				_ErrorCode = ERR_CLI_INVALID_ARG;
			}
			break;
			}
		}
		else
		{
			_ErrorCode = ERR_CLI_INVALID_COMMAND_USAGE;
		}
		break;
	}
	case CMD_INVALID:
	default:
	{
		_ErrorCode = ERR_CLI_INVALID_COMMAND;
		break;
	}
	}
}