#ifndef _CONFIG_MANAGER_HPP_
#define _CONFIG_MANAGER_HPP_

#include <vector>
#include <iterator>
#include <string>
#include <mutex>
#include <atomic>

#include "../common/logger.hpp"
#include "../common/cmd_def.hpp"
#include "../common/common_def.h"
#include "proc_manager.hpp"
#include "xmlconfig_parser.hpp"

using namespace std;

namespace dpm
{
	class ProcessConfigStoreManager
	{
	private:

		static mutex _Locker;

		static atomic<bool> Loaded;

		static ProcessConfigurationStore _ProcessConfigStore;

		static bool _Save(ERR_ENUM& _err, string _filePath = PROCESS_CONFIG_DEFAULT_FILE_NAME);

		static bool _Load(ERR_ENUM& _err, bool _clear = false, string _filePath = PROCESS_CONFIG_DEFAULT_FILE_NAME);

	public:

		static bool ReloadConfiguration(ERR_ENUM& _err, bool _clear = false, string _filePath = "");

		static bool GetProcessConfigById(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID, bool _enabled = true);
		static bool AddProcessConfig(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID, string _afterPid = "");
		static bool RemoveProcessConfig(ERR_ENUM& _err, string _pId, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID);
		static bool UpdateProcessConfig(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID, bool _orAdd = false);
		static bool getAllProcesses(ERR_ENUM& _err, vector<ProcessInfo>& list, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID, bool _enabled = true);

		static void GetAllGroupConfigDescription(ERR_ENUM& _err, stringstream& _op);
		static void GetGroupConfigDescription(ERR_ENUM& _err, stringstream& _op, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID);
		static void GetProcessConfigDescription(ERR_ENUM& _err, stringstream& _op, string _pid, string _gId = PROCESS_CONFIG_DEFAULT_GROUP_ID);

		static bool FormatProcessGroup(int const& _count, stringstream& _op, ProcessGroup* const _pGroup, bool _onlyEnabled = false);

		static void FormatProcessInfo(int const& _count, stringstream& _op, ProcessInfo const& _pInfo);
	};
}

#endif