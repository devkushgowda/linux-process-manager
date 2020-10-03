
#ifndef _PROC_MANAGER_HPP_
#define _PROC_MANAGER_HPP_

#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "../common/utils.hpp"
#include "../common/logger.hpp"
#include "proc_poll_manager.hpp"
#include "request_data.hpp"
#include "proc_def.hpp"


#define MAX_ARG_COUNT 64
#define MAX_ARG_COUNT 64

using namespace std;

namespace dpm
{
	class ProcessConfigurationStore
	{
	private:
		map<string, ProcessGroup*> _ProcessGroupMap;
	public:
		ProcessConfigurationStore();
		ProcessConfigurationStore(string _configVersion);
		~ProcessConfigurationStore();
		ProcessConfigurationStore(const ProcessConfigurationStore& _obj);
		string ConfigVersion;
		map<string, ProcessGroup*>& Groups();
		int size() const;
		int clear();
		bool get(ProcessGroup* (&_pGroup), string _groupId);
		bool exists(string _groupId) const;
		bool add(ProcessGroup* _pGroup);
		bool remove(string _groupId);
		bool update(ProcessGroup& _pGroup, bool _orAdd = false);

	};

	class ProcessManager
	{

	public:
		static int VerifyRunningInstanceCount(ProcessInfo const& _pInfo, BaseRequestData* _requestData);
		static int ResolveDependency(ProcessInfo const& _pDep, BaseRequestData* _requestData);
		static int StartProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData, bool _isMonitor = true, bool _resolveDependency = true);
		static PING_STATUS_ENUM PingProcess(const pid_t _pid);
		static bool StopProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData);
		static bool StopProcessByPid(const pid_t _pid, BaseRequestData* _requestData);
		static int RestartProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData);
		static pid_t ForkNewProcess(ProcessVariables const& _pvar);
		static bool StopProcess(const pid_t _pid, BaseRequestData* _requestData, const unsigned int _timeOut = 1);

		static void MonitorProcess(ProcessInfo const& _pInfo);

		static pid_t GetPidByProcessName(string _pName);
		static pid_t GetPidByProcessPath(string _path);
		static int GetRunningInstanceCountByPath(string _path);
		static int GetRunningInstanceCountByName(string _name);
	};

	class ProcessMonitorStore
	{
	private:
		static mutex _Locker;
		static list<ProcessInstanceManager*> _ProcessMonitorList;
		static ProcessPollManager _ProcessPollMonitorList;
		static ProcessInstanceManager* _GetProcessMonitorInfo(ProcessInfo const& _pInfo, bool _create = false, bool _monitorOverride = true);
		static bool _RemoveMonitorInfo(ProcessInfo const& _pInfo);

	public:

		static void GetInstanceMapCopy(ProcessInfo const& _pInfo, map<pid_t, time_t>& _instanceMap);
		static bool GetInstanceByPid(pair<pid_t, time_t>& _instancePair);
		static bool AddInstance(ProcessInfo const& _pInfo, pid_t _pId, const bool _create = false, const bool _monitorOverride = true, time_t const& _time = time(NULL));
		static bool RemoveInstance(ProcessInfo const& _pInfo, pid_t _pId, const bool  _removeEntry = false);
		static bool RemoveInstanceByPid(pid_t _pid, const bool _removeEntry = false);
		static int GetInstanceCount(ProcessInfo const& _pInfo);

		static int GetRunningProcessCount();

		static void ClearMonitorInfoStore();
	};

}
#endif