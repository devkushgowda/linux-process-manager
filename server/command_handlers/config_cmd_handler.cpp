#include "config_cmd_handler.hpp"

using namespace dpm;

ConfigCmdHandler::ConfigCmdHandler(RequestData& _requestData) : BaseCmdHandler(_requestData)
{
}

void ConfigCmdHandler::execute()
{
	int paramCount = _RequestData.RequestPDU.getParametersCount();
	string groupId, pId;
	stringstream logStream;
	//Default all
	auto commandOption = (OPT_ENUM)_RequestData.RequestPDU.getPrameterAt(0).vint;
	switch (commandOption)
	{
	case OPT_DISPLAY:
	{
		stringstream outputStream;
		if (paramCount == 2)
		{
			ProcessConfigStoreManager::GetAllGroupConfigDescription(_RequestData.Error, outputStream);
		}
		else if (paramCount == 3)
		{
			groupId = _RequestData.RequestPDU.getPrameterAt(2).vstring;
			ProcessConfigStoreManager::GetGroupConfigDescription(_RequestData.Error, outputStream, groupId);
		}
		else
		{
			_RequestData.Error = ERR_CLI_INVALID_ARG;
		}
		if (_RequestData.Error == SUCCESS)
			_RequestData.SendResponseStream(outputStream, OUTPUT_INFO);
		break;
	}
	case OPT_RELOAD:
	{
		if (ProcessConfigStoreManager::ReloadConfiguration(_RequestData.Error))
		{
			logStream << "Configuration reloaded succesfully!";
			_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
		}
		break;
	}
	case OPT_ADD:
	case OPT_DELETE:
	case OPT_UPDATE:
	{
		_RequestData.Error = ERR_NOT_IMPLEMENTED_ROUTINE;
		break;
	}
	default:
		_RequestData.Error = ERR_CLI_INVALID_OPTION;
		break;
	}

	if (_RequestData.Error != SUCCESS)
	{
		logStream << ErrorsDef::GetErrorMessage(_RequestData.Error);
		_RequestData.SendResponseStream(logStream, OUTPUT_ERROR);
	}
}
