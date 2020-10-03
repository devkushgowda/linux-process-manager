#include "cli_handler.hpp"
#include "client_config.hpp"

using namespace std;
using namespace dpm;

int main(int argc, char** argv)
{

	ERR_ENUM errCode = SUCCESS;
	stringstream logStream;
	string cwd;

	Utils::GetCurrentExeDir(cwd);

	if (0 != chdir(cwd.c_str()))
	{
		logStream << "Failed to change current working directory to the binary path " << cwd;
		Logger::LogWarn(logStream);
	}

	ClientConfigurationParser::Parse(errCode);

	if (errCode != SUCCESS)
	{
		logStream << ErrorsDef::GetErrorMessage(errCode) << endl << "Connection settings are defaulted to -> ServerIp: " << ClientConfigurationManager::IpAddress << ", Port: " << ClientConfigurationManager::Port;
		Logger::LogWarn(logStream);
	}

	CliRequestHandler cliHandler(ClientConfigurationManager::IpAddress, ClientConfigurationManager::Port);

	cliHandler.handleCliRequest(argc - 1, argc > 1 ? argv + 1 : NULL);

	return 0;
}

