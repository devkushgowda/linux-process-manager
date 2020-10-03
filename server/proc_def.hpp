#ifndef _PROC_DEF_HPP_
#define _PROC_DEF_HPP_

#include <string>
#include <string.h>
#include <sstream>
#include <map>
#include <list>
#include <mutex>
#include <vector>
#include <iterator>

#include "../common/shmm.hpp"
#include "../common/utils.hpp"
#include "../common/logger.hpp"
#include "../common/err_def.hpp"
#include "../common/shm_templates.hpp"


#define MAX_ARG_COUNT 64
#define MAX_ARG_COUNT 64

using namespace std;

namespace dpm
{
	//Forward declarations.
	class ProcessGroup;
	class ShmInfo;
	class ProcessInfo;

	class ShmInfo
	{
	public:
		bool Enable;
		ProcessGroup* GroupId;
		string Id;
		string Name; // Shared memory name.
		int ShmId; // Shared memory name.
		string ShmPath; // Shared memory name.
		ShmInfo() = default;
		ShmInfo(string _id);
		ShmInfo& operator=(ShmInfo const& _obj);
		ShmInfo(const ShmInfo& _obj);
	};

	enum PD_ENUM { PD_NONE = -1, PD_Process, PD_SharedMem };

	class ProcessDependency
	{
	public:
		list<pair< PD_ENUM, string>> Dependency;
		ProcessDependency() = default;
		ProcessDependency& operator=(ProcessDependency const& _obj);
		ProcessDependency(const ProcessDependency& _obj);
	};

	class ProcessVariables
	{
	private:
		int _CurrentArgIndex;
		int _CurrentEnvIndex;
	public:
		char* Args[MAX_ARG_COUNT];
		char* Envs[MAX_ARG_COUNT];
		ProcessVariables();
		~ProcessVariables();
		ProcessVariables& operator=(ProcessVariables const& _obj);
		ProcessVariables(const ProcessVariables& _obj);

		int getArgsCount() const;
		int getEnvsCount() const;
		void clear();
		int addArg(char const* _arg);
		int addEnv(char const* _env);

	};

	class ProcessFlags
	{
	public:
		bool IsSystemCommand; // Is it system command.
		bool IsSharedProcess; // Is shared process.
		bool IsKillMultiple; // When multiple instances are running, Stop all instances or ignore.
		bool IsRestartAll; // Does restart requires restart all ?
		bool IsMonitor; // Does requires process monitoring.
		ProcessFlags();
	};

	class ProcessInfo
	{
	public:
		bool Enable;
		ProcessGroup* GroupId;
		string Id;
		string Name; // Process name.
		string Path; // Binary path.
		int StartDelay; // Initial delay in seconds.
		int MaxAllowedInstance;
		ProcessFlags Flags;
		ProcessVariables Variables;
		ProcessDependency Dependency;
		ProcessInfo();
		ProcessInfo(string _id);
		ProcessInfo& operator=(ProcessInfo const& _obj);
		ProcessInfo(const ProcessInfo& _obj);

	};

	class ProcessGroup
	{
	private:
		map<string, ProcessInfo> _ProcessMap;
		map<string, ShmInfo> _ShmMap;
	public:
		bool Enable;
		string Id;
		string Description;

		ProcessGroup();
		ProcessGroup(string _groupId);
		ProcessGroup(bool _enable, string _groupId, string _description, int _pollInterval, string _target = "localhost");
		ProcessGroup(const ProcessGroup& _obj);
		ProcessGroup& operator=(ProcessGroup const& _obj);

		const map<string, ProcessInfo>& ProcesMap();
		int size() const;
		int clear();
		bool get(ProcessInfo& _pInfo);
		bool processExists(string _pId) const;
		bool add(ProcessInfo& _pInfo);
		bool removeProcess(string _pId);
		bool update(ProcessInfo& _pInfo, bool _orAdd = false);
		bool getAllProcesses(vector<ProcessInfo>& _list, bool _enable = true);

		bool get(ShmInfo& _shmInfo);
		bool shmExists(string _sId) const;
		bool add(ShmInfo& _shmInfo);
		bool removeShm(string _sId);
		bool update(ShmInfo& _shmInfo, bool _orAdd = false);
		bool getAllShm(vector<ShmInfo>& _list, bool _enable = true);
	};


	enum PING_STATUS_ENUM
	{
		PING_NOT_EXISTS = -1,
		PING_RUNNUNG,
		PING_STOPPED,
		PING_SIGNALED,
		PING_CORE_DUMPED,
	};

	class ProcessInstanceManager
	{
		bool _IsMonitor;
		map<pid_t, time_t> _InstanceMap;

	public:
		ProcessInstanceManager(ProcessInfo& _pInfo, bool _isMonitor);
		ProcessInstanceManager& operator=(ProcessInstanceManager const& _obj);
		ProcessInstanceManager(const ProcessInstanceManager& _obj);

		ProcessInfo Process_Info;

		bool const& IsMonitor;
		void getInstanceMapCopy(map<pid_t, time_t>& _instanceMap);
		bool getInstanceByPid(pair<pid_t, time_t>& _instancePair);
		bool addInstance(pid_t _pId, time_t const& _time = time(NULL));
		bool removeInstance(pid_t _pId);
		int getInstanceCount();

		void clear();
	};
}
#endif