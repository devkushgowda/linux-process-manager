#include "server_config.hpp"

using namespace dpm;

atomic<int> ServerConfigurationManager::_Port(DEFAULT_SERVER_PORT);
string ServerConfigurationManager::_logFilePath = SERVER_CONFIG_DEFAULT_LOG_PATH;
atomic<int> ServerConfigurationManager::_PollInterval(PROCESS_DEFAULT_POLL_INTERVAL);

void ServerConfigurationManager::SetPort(int _port)
{
	stringstream logStream;
	logStream << "ServerConfigurationManager::SetPort(): OldValue: " << _Port.load(memory_order_relaxed) << ", NewValue: " << _port;
	Logger::LogInfo(logStream);
	_Port.store(_port, memory_order_release);
}

void ServerConfigurationManager::SetPollInterval(int _pollInterval)
{
	stringstream logStream;
	logStream << "ServerConfigurationManager::SetPollInterval(): OldValue: " << _PollInterval.load(memory_order_relaxed) << ", NewValue: " << _pollInterval;
	Logger::LogInfo(logStream);
	_PollInterval.store(_pollInterval, memory_order_release);
}

void ServerConfigurationManager::SetLogFilePath(string _filePath)
{
	stringstream logStream;
	logStream << "ServerConfigurationManager::SetLogFilePath(): OldValue: " << _logFilePath << ", NewValue: " << _filePath;
	Logger::LogInfo(logStream);
	_logFilePath = _filePath;
}

int ServerConfigurationManager::GetPort()
{
	return _Port.load(memory_order_relaxed);
}

int ServerConfigurationManager::GetPollInterval()
{
	return _PollInterval.load(memory_order_relaxed);
}

atomic<int>* ServerConfigurationManager::GetPollIntervalPtr()
{
	return &_PollInterval;
}

string ServerConfigurationManager::GetLogFilePath()
{
	return _logFilePath;
}

string ServerConfigurationManager::GetSetting()
{
	stringstream opStream;
	opStream <<
		"rPort: " << GetPort() << endl <<
		"rLogFilePath: " << GetLogFilePath() << endl <<
		"rwLogMode(" << ATTR_LOG_STRING << "): " << Logger::GetLogModeString() << endl <<
		"rwPollInterval(" << ATTR_POLL_STRING << "): " << GetPollInterval();
	return opStream.str();
}


void ServerConfigurationParser::Parse(ERR_ENUM& _err, string _file)
{
	Logger::LogTrace("ServerConfigurationParser::Parse()...");

	xml_document doc;

	_err = SUCCESS;

	if (!doc.load_file(_file.c_str()))
	{
		_err = ERR_SERVER_XML_CONFIG_PARSER_INVALID_CONFIG_FILE;
		return;
	}

	auto rootNode = doc.child(SERVER_SETTING_CONFIG_XML_ROOT);
	if (rootNode)
	{
		auto port = rootNode.attribute(SERVER_SETTING_CONFIG_ATTR_PORT);
		int portValue = port.as_int();

		if (portValue > TCP_PORT_MIN&& portValue < TCP_PORT_MAX)
			ServerConfigurationManager::SetPort(portValue);

		auto pollInterval = rootNode.attribute(SERVER_SETTING_CONFIG_ATTR_DEFAULT_POLL_INTERVAL);
		int pollIntervalValue = pollInterval.as_int();

		if (pollIntervalValue > PROCESS_MIN_POLL_INTERVAL)
		{
			if (pollIntervalValue > PROCESS_MAX_POLL_INTERVAL)
				pollIntervalValue = PROCESS_MAX_POLL_INTERVAL;
			ServerConfigurationManager::SetPollInterval(pollIntervalValue);
		}

		auto logFile = rootNode.attribute(SERVER_SETTING_CONFIG_ATTR_LOG_FILE);
		string logFileValue = logFile.as_string();

		if (logFileValue.size() > 0)
		{
			ServerConfigurationManager::SetLogFilePath(logFileValue);
		}

		auto log = rootNode.attribute(SERVER_SETTING_CONFIG_ATTR_LOG);
		string logValue = log.as_string();

		if (logValue.size() > 0)
		{
			StringUtil::ToUpper(logValue);
			if (logValue == LOG_DEBUG_STRING)
			{
				Logger::InitLogger(LOG_DEBUG, ServerConfigurationManager::GetLogFilePath(), true);
			}
			else if (logValue == LOG_TRACE_STRING)
			{
				Logger::InitLogger(LOG_TRACE, ServerConfigurationManager::GetLogFilePath(), true);
			}
			else if (logValue == LOG_INFO_STRING)
			{
				Logger::InitLogger(LOG_INFO, ServerConfigurationManager::GetLogFilePath(), true);
			}
			else if (logValue == LOG_WARN_STRING)
			{
				Logger::InitLogger(LOG_WARN, ServerConfigurationManager::GetLogFilePath(), true);
			}
			else if (logValue == LOG_ERROR_STRING)
			{
				Logger::InitLogger(LOG_ERROR, ServerConfigurationManager::GetLogFilePath(), true);
			}
		}
	}
	else
	{
		_err = ERR_SERVER_XML_CONFIG_PARSER_ROOT_NOT_FOUND;
	}
}
