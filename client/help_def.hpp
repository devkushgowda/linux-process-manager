#ifndef _HELP_MANAGER_HPP_
#define _HELP_MANAGER_HPP_

#include <map>
#include <string>
#include <iterator>

#include "../common/utils.hpp"
#include "../common/cmd_def.hpp"

using namespace std;
using namespace dpm;

namespace dpm
{
#define HELP_MAP(X) {X,ADD_LINE2(X##_HELP)}

#define ADD_LINE1(x) x "\n\n"
#define ADD_LINE2(x) "\n\n" x "\n\n"

#define HEAD(x) CCYN(x)
#define  EG(x) CMAG(x)
#define  UL(x) CUNDL(x)
#define  WARN(x) CYEL(x)

#define CMD_ALL_HELP \
"1." ADD_LINE1(CMD_START_HELP)\
"2." ADD_LINE1(CMD_STOP_HELP)\
"3." ADD_LINE1(CMD_RESTART_HELP)\
"4." ADD_LINE1(CMD_PING_HELP)\
"5." ADD_LINE1(CMD_CONFIG_HELP)\
"6." ADD_LINE1(CMD_SETTING_HELP)\
"7." ADD_LINE1(CMD_HELP_HELP)

#define CMD_START_HELP HEAD("START - Start the process or set of processes defined in the configuration.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Without any parameters to start all the processes from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc start '")\
"\n\n\t  2. With process identifier as an argument to start the single process matching the identifier from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc start app-id '")\
"\n\n\t  3. With group identifier as an argument to start all the processes from that group configuration."\
 EG("\n\t     E.g. ' ./pmc start -g my-group'")\
"\n\n\t  4. With process and group identifier as an arguments to start the single process matching the identifier from the specified group configuration."\
 EG("\n\t     E.g. ' ./pmc start app-id -g my-group'")


#define CMD_STOP_HELP HEAD("STOP - Stop/Kill the process or set of processes defined in the configuration.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Without any parameters to stop all the processes from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc stop '")\
"\n\n\t  2. With process identifier as an argument to stop the single process matching the identifier from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc stop app-id '")\
"\n\n\t  3. With group identifier as an argument to stop all the processes from that group configuration."\
 EG("\n\t     E.g. ' ./pmc stop -g my-group'")\
"\n\n\t  4. With process and group identifier as an arguments to stop the single process matching the identifier from the specified group configuration."\
 EG("\n\t     E.g. ' ./pmc stop app-id -g my-group'")\
"\n\n\t  5. With pid as an argument to stop the single process matching the pid from the spawned process store."\
 EG("\n\t     E.g. ' ./pmc stop -pid 1234'")


#define CMD_RESTART_HELP HEAD("RESTART - Restart the process or set of processes defined in the configuration.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Without any parameters to ping all the processes from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc ping '")\
"\n\n\t  2. With process identifier as an argument to ping the single process matching the identifier from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc ping app-id '")\
"\n\n\t  3. With group identifier as an argument to ping all the processes from that group configuration."\
 EG("\n\t     E.g. ' ./pmc ping -g my-group'")\
"\n\n\t  4. With process and group identifier as an arguments to ping the single process matching the identifier from the specified group configuration."\
 EG("\n\t     E.g. ' ./pmc ping app-id -g my-group'")


#define CMD_PING_HELP HEAD("PING - Get the status of process or set of processes defined in the configuration.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Without any parameters to restart all the processes from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc restart '")\
"\n\n\t  2. With process identifier as an argument to restart the single process matching the identifier from the default group configuration."\
 EG("\n\t     E.g. ' ./pmc restart app-id '")\
"\n\n\t  3. With group identifier as an argument to restart all the processes from that group configuration."\
 EG("\n\t     E.g. ' ./pmc restart -g my-group'")\
"\n\n\t  4. With process and group identifier as an arguments to restart the single process matching the identifier from the specified group configuration."\
 EG("\n\t     E.g. ' ./pmc restart app-id -g my-group'")

#define CMD_CONFIG_HELP HEAD("CONFIG - Display or perform an operation on the configuration data.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Display all the process from all the process groups defined in the configuration."\
 EG("\n\t     E.g. ' ./pmc config -d '")\
"\n\n\t  2. Display a specified group processes matching the group identifier from the configuration."\
 EG("\n\t     E.g. ' ./pmc config -d my-group '")\
"\n\n\t  3. Reload the configuration data from the file."\
 EG("\n\t     E.g. ' ./pmc config -reload '")\
 WARN("\n\n\t    Note: For security reasons this will load any additional new processes/groups(i.e. new pid/gid) added in the file. Entries removal/updation will not work.")

#define CMD_SETTING_HELP  HEAD("SETTING - Display or perform an operation on the server settings.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Display all the available server settings."\
 EG("\n\t     E.g. ' ./pmc setting -d '")\
"\n\n\t  2. Update a specified server setting matching the setting attribute. Kindly refer to the documentaion for available setting attributes."\
 EG("\n\t     E.g. ' ./pmc setting -u -log log_info '")\
"\n\n\t  3. Defaults the server settings."\
 EG("\n\t     E.g. ' ./pmc setting -reset '")

#define CMD_HELP_HELP HEAD("HELP - Command used to describe all avilable commands or a specific command.")\
 UL("\n\n  Usage:")\
"\n\n\t  1. Without any parameters to get help for all available commands."\
 EG("\n\t     E.g. ' ./pmc help '")\
"\n\n\t  2. With command as an argument to get help for the specified command."\
 EG("\n\t     E.g. ' ./pmc help start ' - Can be used with start|stop|restart|ping|config|setting|help")


	class HelpDef
	{
		static map<CMD_ENUM, string> _HelpMap;

	public:
		static string getCommandHelpMessage(CMD_ENUM _cmd);
	};
}
#endif
