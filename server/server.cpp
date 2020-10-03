#include <sstream>
#include <future>  
#include <stdlib.h>  
#include "../common/socket_manager.hpp"
#include "../common/cmd_def.hpp"
#include "../common/err_def.hpp"
#include "../common/logger.hpp"
#include "request_handler.hpp"
#include "config_manager.hpp"
#include "server_config.hpp"

using namespace std;
using namespace dpm;

int main(int argc, char** args)
{
	SocketServer serverSocket;
	ERR_ENUM errCode = SUCCESS;
	stringstream logStream;
	string cwd;
	Utils::GetCurrentExeDir(cwd);

	if (0 != chdir(cwd.c_str()))
	{
		logStream << "Failed to change current working directory to the binary path " << cwd;
		Logger::LogWarn(logStream);
	}


	ServerConfigurationParser::Parse(errCode);

	if (errCode != SUCCESS)
	{
		logStream << "ServerConfigurationParser::Parse(): failed with error: " << ErrorsDef::GetErrorMessage(errCode) <<
			endl << "Defaulted server settings!.";
		Logger::LogWarn(logStream);
	}

	if (!ProcessConfigStoreManager::ReloadConfiguration(errCode))
	{
		logStream << "Loading configuration failed with error message: " << ErrorsDef::GetErrorMessage(errCode);
		Logger::LogError(logStream);
		exit(0);
	}

	int sockErr = serverSocket.createServerSocket(ServerConfigurationManager::GetPort());

	if (sockErr < 0)
	{
		Logger::LogError(ErrorsDef::GetErrorMessage((ERR_ENUM)sockErr));
	}
	else
	{
		int socketFD = serverSocket.waitForConnection();

		while (socketFD > 0)
		{
			logStream << "Accepted connection, SocketFD:: " << socketFD << " , Started request handler asynchronusly.";
			Logger::LogInfo(logStream);
			async(launch::async, RequestHandler::ServeRequest, socketFD);
			socketFD = serverSocket.waitForConnection();
		}
		Logger::LogError("Socket failure. Exiting...");
	}
	exit(0);
}
