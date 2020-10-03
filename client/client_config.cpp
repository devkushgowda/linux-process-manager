#include "client_config.hpp"

using namespace dpm;

int ClientConfigurationManager::_Port = DEFAULT_SERVER_PORT;
string ClientConfigurationManager::_IpAddress = DEFAULT_SERVER_IP;

int const& ClientConfigurationManager::Port = _Port;
string const& ClientConfigurationManager::IpAddress = _IpAddress;

void ClientConfigurationManager::SetPort(int _port)
{
	_Port = _port;
}

void ClientConfigurationManager::setIpAddress(string _ipAddress)
{
	_IpAddress = _ipAddress;
}

string ClientConfigurationManager::GetSetting()
{
	stringstream opStream;
	opStream <<
		"Port: " << Port << endl <<
		"IpAddress: " << IpAddress << endl;
	return opStream.str();
}


void ClientConfigurationParser::Parse(ERR_ENUM& _err, string _file)
{
	Logger::LogTrace("ClientConfigurationParser::Parse()...");

	stringstream logStream;

	xml_document doc;

	_err = SUCCESS;

	if (!doc.load_file(_file.c_str()))
	{
		_err = ERR_CLIENT_XML_CONFIG_PARSER_INVALID_CONFIG_FILE;
		return;
	}

	auto rootNode = doc.child(CLIENT_SETTING_CONFIG_XML_ROOT);

	if (!rootNode)
	{
		_err = ERR_CLIENT_XML_CONFIG_PARSER_ROOT_NOT_FOUND;
		return;
	}

	auto log = rootNode.attribute(CLIENT_SETTING_CONFIG_ATTR_LOG);
	if (log)
	{
		string val = log.as_string();
		StringUtil::ToUpper(val);
		if (val == LOG_DEBUG_STRING)
		{
			Logger::InitLogger(LOG_DEBUG);
		}
		else if (val == LOG_TRACE_STRING)
		{
			Logger::InitLogger(LOG_TRACE);
		}
		else if (val == LOG_INFO_STRING)
		{
			Logger::InitLogger(LOG_INFO);
		}
		else if (val == LOG_WARN_STRING)
		{
			Logger::InitLogger(LOG_WARN);
		}
		else if (val == LOG_ERROR_STRING)
		{
			Logger::InitLogger(LOG_ERROR);
		}
	}

	auto port = rootNode.attribute(CLIENT_SETTING_CONFIG_ATTR_PORT);
	if (port)
	{
		int portValue = port.as_int();
		if (portValue > TCP_PORT_MIN&& portValue < TCP_PORT_MAX)
		{
			ClientConfigurationManager::SetPort(portValue);
			logStream << "ClientConfigurationParser::Parse(): " << "Server port set to " << portValue;
			Logger::LogTrace(logStream);
		}
		else
		{
			logStream << "ClientConfigurationParser::Parse(): " << "Server port is invalid " << portValue << ", defaulted to " << DEFAULT_SERVER_PORT;
			Logger::LogWarn(logStream);
		}
	}

	auto ip = rootNode.attribute(CLIENT_SETTING_CONFIG_ATTR_IP);
	if (ip)
	{
		string ipValue = ip.as_string();
		if (Utils::IsValidIpAddress(ipValue))
		{
			ClientConfigurationManager::setIpAddress(ipValue);
			logStream << "ClientConfigurationParser::Parse(): " << "Server ip set to " << ipValue;
			Logger::LogTrace(logStream);
		}
		else
		{
			logStream << "ClientConfigurationParser::Parse(): " << "Server ip is invalid " << ipValue << ", defaulted to " << DEFAULT_SERVER_IP;
			Logger::LogWarn(logStream);
		}
	}

}
