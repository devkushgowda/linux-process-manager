#include "settings_cmd_handler.hpp"

using namespace dpm;

SettingsCmdHandler::SettingsCmdHandler(RequestData& _requestData) :BaseCmdHandler(_requestData)
{
}

void SettingsCmdHandler::execute()
{
	auto commandOption = (OPT_ENUM)_RequestData.RequestPDU.getPrameterAt(0).vint;
	stringstream logStream;
	_RequestData.Error = SUCCESS;
	switch (commandOption)
	{
	case OPT_UPDATE:
	{
		auto attr = (ATTR_ENUM)_RequestData.RequestPDU.getPrameterAt(1).vint;
		switch (attr)
		{
		case ATTR_LOG:
		{
			auto logMode = (LOG_MODE)_RequestData.RequestPDU.getPrameterAt(2).vint;
			Logger::InitLogger(logMode,ServerConfigurationManager::GetLogFilePath(),true);
			break;
		}
		case ATTR_POLL:
		{
			auto pollInterval = _RequestData.RequestPDU.getPrameterAt(2).vint;
			ServerConfigurationManager::SetPollInterval(pollInterval);
			break;
		}
		default:
			_RequestData.Error = ERR_NOT_IMPLEMENTED_ROUTINE;
			break;
		}

		if (_RequestData.Error == SUCCESS)
		{
			logStream << "Setting succesfully updated!.";
			_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
			logStream << "Updated server setting,";
			_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
			logStream << ServerConfigurationManager::GetSetting();
			_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
		}

		break;
	}
	case OPT_DISPLAY:
	{
		logStream << ServerConfigurationManager::GetSetting();
		_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
		break;
	}
	case OPT_RESET:
	{
		auto opt = (OPT_ENUM)_RequestData.RequestPDU.getPrameterAt(1).vint;
		switch (opt)
		{
		case OPT_ALL:
		{
			Logger::InitLogger(LOG_WARN);
			ServerConfigurationManager::SetPollInterval(PROCESS_DEFAULT_POLL_INTERVAL);
			break;
		}
		case OPT_BY_ID:
		{
			auto attr = (ATTR_ENUM)_RequestData.RequestPDU.getPrameterAt(2).vint;
			switch (attr)
			{
			case ATTR_LOG:
				Logger::InitLogger(LOG_WARN);
				break;
			case ATTR_POLL:
				ServerConfigurationManager::SetPollInterval(PROCESS_DEFAULT_POLL_INTERVAL);
				break;
			default:
				_RequestData.Error = ERR_NOT_IMPLEMENTED_ROUTINE;
				break;
			}
			break;
		}
		default:
			_RequestData.Error = ERR_NOT_IMPLEMENTED_ROUTINE;
			break;
		}
		if (_RequestData.Error == SUCCESS)
		{
			logStream << "Setting succesfully reset!.";
			_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
			logStream << "Updated server setting,";
			_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
			logStream << ServerConfigurationManager::GetSetting();
			_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
		}
		break;
	}
	default:
		_RequestData.Error = ERR_NOT_IMPLEMENTED_ROUTINE;
		break;
	}
	if (_RequestData.Error != SUCCESS)
	{
		logStream << ErrorsDef::GetErrorMessage(_RequestData.Error);
		_RequestData.SendResponseStream(logStream, OUTPUT_ERROR);
	}
}
