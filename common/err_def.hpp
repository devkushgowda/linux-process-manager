#ifndef _ERR_DEF_HPP_
#define _ERR_DEF_HPP_


#include <map>
#include <string>
#include <iterator>
#include <climits>

using namespace std;

namespace dpm
{
	//Macro Error Map Define
#define EMD(X) {X,X##_MESSAGE}

	enum ERR_ENUM
	{
		SUCCESS = 0,
		WARN_PM_MULTI_INSTANCE_OVERFLOW,
		WARN_PM_KILL_MANY_SKIP,

		ERR_CLI_INSUFFICIENT_ARGS = -INT_MAX,
		ERR_CLI_INVALID_COMMAND_USAGE,
		ERR_CLI_INVALID_COMMAND,
		ERR_CLI_INVALID_OPTION,
		ERR_CLI_INVALID_ARG,
		ERR_CLI_INVALID_ATTR_LOG,
		ERR_CLI_INVALID_ATTR_POLL,
		ERR_CLI_INVALID_ATTR_INVALID,

		ERR_SOCKET_CREATION_FAILED,
		ERR_SOCKET_SERVER_CONNECTION_FAILED,
		ERR_SOCKET_SEND_FAILED,
		ERR_SOCKET_RCV_FAILED,
		ERR_SOCKET_SEND_FILE_FAILED,
		ERR_SOCKET_RCV_FILE_FAILED,
		ERR_SOCKET_INVALID_IP,
		ERR_SOCKET_INVALID_PORT,
		ERR_SOCKET_BIND_FAILED,
		ERR_SOCKET_LISTEN_FAILED,

		ERR_PM_BIN_NOT_FOUND,
		ERR_PM_EXEC_FAILED,
		ERR_PM_FORK_FAILED,
		ERR_PM_SYSTEM_COMMAND_FAILED,
		ERR_PM_PROC_OPEN_FAILED,
		ERR_PM_PROC_WITH_PID_NOT_FOUND,

		ERR_CONFIG_PINFO_NOT_FOUND,
		ERR_CONFIG_PGROUP_NOT_FOUND,
		ERR_CONFIG_PINFO_ALREADY_EXISTS,
		ERR_CONFIG_PGROUP_ALREADY_EXISTS,
		ERR_CONFIG_PINFO_UPDATE_SKIPPED,
		ERR_CONFIG_DISABLED,
		ERR_CONFIG_NOT_LOADED,
		ERR_CONFIG_PGROUP_EMPTY,

		ERR_PROC_XML_CONFIG_PARSER_INVALID_CONFIG_FILE,
		ERR_PROC_XML_CONFIG_PARSER_ROOT_NOT_FOUND,
		ERR_PROC_XML_CONFIG_PARSER_GROUPS_NOT_FOUND,
		ERR_PROC_XML_CONFIG_PARSER_PROCESS_NOT_FOUND,


		ERR_SERVER_XML_CONFIG_PARSER_INVALID_CONFIG_FILE,
		ERR_SERVER_XML_CONFIG_PARSER_ROOT_NOT_FOUND,

		ERR_CLIENT_XML_CONFIG_PARSER_INVALID_CONFIG_FILE,
		ERR_CLIENT_XML_CONFIG_PARSER_ROOT_NOT_FOUND,

		ERR_PDU_MAKE_PDU_FAILED,

		ERR_REQUEST_ACK_FAILED,

		ERR_APP_VERSION_MISMATCH,
		ERR_NOT_IMPLEMENTED_ROUTINE,
		ERR_PROC_HANDLER_BUSY,

	};

#define WARN_PM_MULTI_INSTANCE_OVERFLOW_MESSAGE "Skipping process start as multi-instance limit exeeded."
#define WARN_PM_KILL_MANY_SKIP_MESSAGE "Skipping process stop as kill-many is not enabled."

#define ERR_CLI_INSUFFICIENT_ARGS_MESSAGE "Insufficient command arguments. Please refer to the help command for the valid usage."
#define ERR_CLI_INVALID_COMMAND_USAGE_MESSAGE "Invalid command usage. Please refer to the help command the valid usage."
#define ERR_CLI_INVALID_COMMAND_MESSAGE "Invalid command. Please refer to the help command to view available commands."
#define ERR_CLI_INVALID_OPTION_MESSAGE "Invalid command option. Please refer to the help command to view the available command variants."
#define ERR_CLI_INVALID_ARG_MESSAGE "Invalid command arguments. Please refer to the help command for the valid usage."

#define ERR_CLI_INVALID_ATTR_LOG_MESSAGE "Invalid log mode, refer to the documentation."
#define ERR_CLI_INVALID_ATTR_POLL_MESSAGE "Invalid/OutOfRange poll interval, refer to the documentation."
#define ERR_CLI_INVALID_ATTR_INVALID_MESSAGE "Invalid setting attribute, refer to the documentation."

#define ERR_SOCKET_CREATION_FAILED_MESSAGE "Socket creation failed."
#define ERR_SOCKET_SERVER_CONNECTION_FAILED_MESSAGE "Unable to connect to server. Please ensure server is running and also ip/port configured valid."
#define ERR_SOCKET_SEND_FAILED_MESSAGE "Socket send buffer failed."
#define ERR_SOCKET_RCV_FAILED_MESSAGE "Socket recieve buffer failed."
#define ERR_SOCKET_SEND_FILE_FAILED_MESSAGE "Socket send file failed."
#define ERR_SOCKET_RCV_FILE_FAILED_MESSAGE "Socket recieve file failed."
#define ERR_SOCKET_INVALID_IP_MESSAGE "Invalid socket ip address."
#define ERR_SOCKET_INVALID_PORT_MESSAGE "Invalid socket port address."
#define ERR_SOCKET_BIND_FAILED_MESSAGE "Socket binding failed."
#define ERR_SOCKET_LISTEN_FAILED_MESSAGE "Socket listen failed."

#define ERR_PM_BIN_NOT_FOUND_MESSAGE "Binary not found."
#define ERR_PM_EXEC_FAILED_MESSAGE "exec() failed."
#define ERR_PM_FORK_FAILED_MESSAGE "fork() failed."
#define ERR_PM_SYSTEM_COMMAND_FAILED_MESSAGE "System command execution failed."
#define ERR_PM_PROC_OPEN_FAILED_MESSAGE "proc() open failed."
#define ERR_PM_PROC_WITH_PID_NOT_FOUND_MESSAGE "Process with given pid does not exists/not spawned by DPM."

#define ERR_CONFIG_PINFO_NOT_FOUND_MESSAGE "Process configuration not found."
#define ERR_CONFIG_PGROUP_NOT_FOUND_MESSAGE "Process group not found."
#define ERR_CONFIG_PINFO_ALREADY_EXISTS_MESSAGE "Process configuration with given id already exists."
#define ERR_CONFIG_PGROUP_ALREADY_EXISTS_MESSAGE "Process group with given id already exists."
#define ERR_CONFIG_PINFO_UPDATE_SKIPPED_MESSAGE "Process configuration update skipped as the given configuration already exists."
#define ERR_CONFIG_DISABLED_MESSAGE "Requested configurtion is disabled."
#define ERR_CONFIG_NOT_LOADED_MESSAGE "Process configuration not loaded."
#define ERR_CONFIG_PGROUP_EMPTY_MESSAGE "Process group is empty."

#define ERR_PROC_XML_CONFIG_PARSER_INVALID_CONFIG_FILE_MESSAGE "Invalid process configuration file / does not exists."
#define ERR_PROC_XML_CONFIG_PARSER_ROOT_NOT_FOUND_MESSAGE "Process configuration root node not defined."
#define ERR_PROC_XML_CONFIG_PARSER_GROUPS_NOT_FOUND_MESSAGE "Atleast one group should be defined in the process configuration."
#define ERR_PROC_XML_CONFIG_PARSER_PROCESS_NOT_FOUND_MESSAGE "Atleast one process should be defined in any of process group configuration."

#define ERR_SERVER_XML_CONFIG_PARSER_INVALID_CONFIG_FILE_MESSAGE "Invalid server setting file / does not exists."
#define ERR_SERVER_XML_CONFIG_PARSER_ROOT_NOT_FOUND_MESSAGE "Server setting configuration root node not defined."

#define ERR_CLIENT_XML_CONFIG_PARSER_INVALID_CONFIG_FILE_MESSAGE "Invalid client setting file / does not exists."
#define ERR_CLIENT_XML_CONFIG_PARSER_ROOT_NOT_FOUND_MESSAGE "Client setting configuration root node not defined."

#define ERR_PDU_MAKE_PDU_FAILED_MESSAGE "Error occured while writing into PDU buffer, It could be due to buffer overflow."


#define ERR_REQUEST_ACK_FAILED_MESSAGE "Could not receive the client acknoledgement."

#define ERR_APP_VERSION_MISMATCH_MESSAGE "Client and server are running different interface version, please run the same version and try again."

#define ERR_NOT_IMPLEMENTED_ROUTINE_MESSAGE "Not implemented routine."

#define ERR_PROC_HANDLER_BUSY_MESSAGE "Process handler is busy, please try again after sometime."

	class ErrorsDef
	{
	private:
		static  map<ERR_ENUM, string> _ErrorMap;
	public:
		static string GetErrorMessage(ERR_ENUM _errorCode);
	};
}
#endif
