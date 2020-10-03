#include "proc_manager.hpp"

using namespace dpm;

int ProcessManager::VerifyRunningInstanceCount(ProcessInfo const& _pInfo, BaseRequestData* _requestData)
{
	stringstream logStream;
	if (_pInfo.Flags.IsSystemCommand)
	{
		return -1;
	}
	else
	{
		auto count = ProcessMonitorStore::GetInstanceCount(_pInfo);
		if (count >= _pInfo.MaxAllowedInstance)
		{
			if (count > 0)
			{
				logStream << "Ignoring start for process Id:: '" << _pInfo.Id
					<< "' as 'MaxAllowedInstance' limit reached. " << count
					<< " instances are running.";
				_requestData->SendResponseStream(logStream, OUTPUT_WARN);
				_requestData->Error = WARN_PM_MULTI_INSTANCE_OVERFLOW;
				return -1;
			}
		}
	}

	return 1;

}

int ProcessManager::ResolveDependency(ProcessInfo const& _pInfo, BaseRequestData* _requestData)
{
	Logger::LogTrace("ProcessManager::ResolveDependency()...");
	stringstream logStream;
	for (auto const& entry : _pInfo.Dependency.Dependency)
	{
		logStream << "Resolving dependency for Pid: " << _pInfo.Id << " -> DependencyId: " << entry.second << ", Type: " << ((entry.first == PD_Process) ? "Process" : "Shared Memory");
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);
		switch (entry.first)
		{
		case PD_Process:
		{
			_requestData->SendResponseStream(logStream, OUTPUT_INFO);
			ProcessInfo process(entry.second);

			if (ProcessMonitorStore::GetInstanceCount(_pInfo) <= 0 && _pInfo.GroupId->get(process))
			{
				StartProcess(process, _requestData, true, false);
			}
			else
			{
				logStream << "Skipping to start dependency process -> Pid: " << _pInfo.Id << " , Already instance is running/invalid Id";
				_requestData->SendResponseStream(logStream, OUTPUT_WARN);
			}
			break;
		}
		case PD_SharedMem:
		{
			ShmInfo shm(entry.second);
			if (_pInfo.GroupId->get(shm))
			{
				SharedMemoryProducerConfig smpc;
				smpc.ID = shm.ShmId;
				smpc.PathName = shm.ShmPath;
				smpc.Destroy = true;
				if (SharedMemoryFactory::GetSharedMemoryProducer(entry.second, true, &smpc))
				{
					logStream << "Created/Exists  shared memory -> ShmId: " << smpc.ID << ", ShmPath: " << smpc.PathName;
					_requestData->SendResponseStream(logStream, OUTPUT_INFO);
				}
				else
				{
					logStream << "Failed to create shared memory for Pid: " << _pInfo.Id << " -> DependencyId: " << entry.second << ", Type: " << ((entry.first == PD_Process) ? "Process" : "Shared Memory") << ", ShmId: " << smpc.ID << ", ShmPath: " << smpc.PathName;
					_requestData->SendResponseStream(logStream, OUTPUT_ERROR);
				}
			}
			break;
		}
		default:
			break;
		}
	}
	return 0;
}

int ProcessManager::StartProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData, bool _monitorOveride, bool _resolveDependency)
{
	int res = -1;

	Logger::LogTrace("ProcessManager::StartProcess()...");

	stringstream logStream;

	_requestData->Error = SUCCESS;

	if (VerifyRunningInstanceCount(_pInfo, _requestData) < 0)
	{
		return -1;
	}

	if (_pInfo.StartDelay > 0)
	{
		logStream << "Delaying process Id:: '" << _pInfo.Id << "' for " << _pInfo.StartDelay << "s.";
		_requestData->SendResponseStream(logStream, OUTPUT_WARN);
		sleep(_pInfo.StartDelay);
	}

	if (_pInfo.Flags.IsSystemCommand)
	{
		string ampersand = "&";
		string pPath = StringUtil::EndsWith(_pInfo.Path, ampersand) ? _pInfo.Path : _pInfo.Path + ampersand;

		if (0 != system(pPath.c_str()))
			_requestData->Error = ERR_PM_SYSTEM_COMMAND_FAILED;
	}
	else
	{
		if (VerifyRunningInstanceCount(_pInfo, _requestData) < 0)
		{
			return -1;
		}
		if (_resolveDependency)
			ResolveDependency(_pInfo, _requestData);

		auto pid = ForkNewProcess(_pInfo.Variables);
		if (pid > 0)
		{
			res = pid;
			ProcessMonitorStore::AddInstance(_pInfo, pid, true, _monitorOveride);
			logStream << "Succesfully created the process. " << "pid: " << pid;
			_requestData->SendResponseStream(logStream, OUTPUT_INFO);
		}
		else
		{
			;
			logStream << "Process start failed with error message,  " << ErrorsDef::GetErrorMessage((ERR_ENUM)pid);
			_requestData->SendResponseStream(logStream, OUTPUT_ERROR);
		}
	}

	return res;
}

PING_STATUS_ENUM ProcessManager::PingProcess(const pid_t _pid)
{
	PING_STATUS_ENUM ret;

	int status = 0;

	auto res = waitpid(_pid, &status, WNOHANG);

	if (res == 0)
	{
		ret = PING_RUNNUNG;
	}
	else if (res < 0)
	{
		ret = PING_NOT_EXISTS;
	}
	else if (WIFEXITED(status))
	{
		ret = PING_STOPPED;
	}
	else if (WIFSIGNALED(status))
	{
		if (WCOREDUMP(status))
		{
			ret = PING_CORE_DUMPED;
		}
		else
		{
			ret = PING_SIGNALED;
		}
	}

	return ret;
}

bool ProcessManager::StopProcess(const pid_t _pid, BaseRequestData* _requestData, const unsigned int _timeOut)
{
	int timeOut = _timeOut > 0 ? _timeOut : 1;
	bool exited = false;
	int status;
	int curWaitTime = 0;
	stringstream logStream;

	kill(_pid, SIGTERM);

	logStream << "Sending signal 'SIGTERM'";
	_requestData->SendResponseStream(logStream, OUTPUT_INFO);

	while (curWaitTime++ < timeOut)
	{
		logStream << "Try: " << curWaitTime << " - Waiting for a second to check status";
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);

		sleep(1);
		auto pid = waitpid(_pid, &status, WNOHANG);

		if (pid < 0)
		{
			logStream << "Try: " << curWaitTime << " - Process with pid: " << _pid << " does not exists.";
			_requestData->SendResponseStream(logStream, OUTPUT_WARN);
			return false;
		}


		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			logStream << "Try: " << curWaitTime << " - Process exited succesfully.";
			_requestData->SendResponseStream(logStream, OUTPUT_INFO);
			exited = true;
			break;

		}
	}
	if (!exited)
	{

		logStream << "Process could not be stopped, sending signal 'SIGKILL'";
		_requestData->SendResponseStream(logStream, OUTPUT_WARN);


		kill(_pid, SIGKILL);

		logStream << "Waiting for a second to check the status";
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);


		sleep(1);
		auto pid = waitpid(_pid, &status, WNOHANG);

		if (pid < 0)
		{
			logStream << "Try: " << curWaitTime << " - Process with pid: " << _pid << " does not exists.";
			_requestData->SendResponseStream(logStream, OUTPUT_WARN);
			return false;
		}

		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			logStream << "Process killed succesfully.";
			_requestData->SendResponseStream(logStream, OUTPUT_INFO);
			exited = true;
		}
	}

	return exited;
}

void ProcessManager::MonitorProcess(ProcessInfo const& _pInfo)
{
	stringstream logStream;

	map<pid_t, time_t> instanceMap;
	ProcessMonitorStore::GetInstanceMapCopy(_pInfo, instanceMap);
	logStream << "Triggered process monitor for process ProcessId: '" << _pInfo.Id << "' , GroupId: '" << _pInfo.GroupId->Id << "'" << endl;
	Logger::LogPoll(logStream, POLL_HEAD);
	if (instanceMap.size() > 0)
	{
		for (auto const& instance : instanceMap)
		{
			;
			auto curProcessState = ProcessManager::PingProcess(instance.first);
			if (curProcessState == PING_RUNNUNG)
			{
				logStream << "Instance with pid: " << instance.first << " running for " << Utils::GetTimeIntervalString(instance.second);
				Logger::LogPoll(logStream, POLL_SUCCESS);
			}
			else
			{
				logStream << "Instance with pid: " << instance.first << " not running.\nProcess  ran for " << Utils::GetTimeIntervalString(instance.second) << " and was started at " << ctime(&instance.second);
				Logger::LogPoll(logStream, POLL_ERROR);
				ProcessMonitorStore::RemoveInstance(_pInfo, instance.first);
				DummyRequestData requestData;
				auto pid = StartProcess(_pInfo, &requestData);
				if (pid > 0)
				{
					logStream << "Replaced failed instance with pid: " << instance.first << " , with new instance pid: " << pid;
					Logger::LogPoll(logStream, POLL_WARN);
					ProcessMonitorStore::AddInstance(_pInfo, pid, true);
				}
			}
		}
	}
	else
	{
		;
		logStream << "No instances are running!." << endl;
		Logger::LogError(logStream);
	}
}

bool ProcessManager::StopProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData)
{
	Logger::LogTrace("ProcessManager::StopProcess()...");

	if (_pInfo.Flags.IsSystemCommand)
		return false;

	stringstream logStream;

	_requestData->Error = SUCCESS;
	int killCount = 0;

	map<pid_t, time_t> instanceMap;
	ProcessMonitorStore::GetInstanceMapCopy(_pInfo, instanceMap);

	if (!_pInfo.Flags.IsKillMultiple && instanceMap.size() > 1)
	{
		logStream << "Ignoring stop process as  'IsKillMultiple' is set and " << instanceMap.size() << " instances are running.";
		_requestData->SendResponseStream(logStream, OUTPUT_WARN);
		return false;
	}

	for (auto const& instance : instanceMap)
	{
		logStream << "Stopping instance with p_id: '" << instance.first << "', Process ran for " << Utils::GetTimeIntervalString(instance.second);
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);
		StopProcess(instance.first, _requestData);
		killCount++;
		ProcessMonitorStore::RemoveInstance(_pInfo, instance.first, true);
	}

	if (killCount == 0)
	{
		logStream << "No instances are available to kill for the process Id:: '" << _pInfo.Id << "'.";
		_requestData->SendResponseStream(logStream, OUTPUT_WARN);
	}
	else
	{
		logStream << "Killed " << killCount << " instances of process Id:: '" << _pInfo.Id << "'.";
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);
	}

	return _requestData->Error >= SUCCESS;
}

bool ProcessManager::StopProcessByPid(const pid_t _pid, BaseRequestData* _requestData)
{
	Logger::LogTrace("ProcessManager::StopProcessByPid()...");


	stringstream logStream;

	_requestData->Error = SUCCESS;

	pair<pid_t, time_t> instancePair;
	instancePair.first = _pid;
	if (ProcessMonitorStore::GetInstanceByPid(instancePair))
	{
		logStream << "Stopping instance with p_id: '" << instancePair.first << "', Process ran for " << Utils::GetTimeIntervalString(instancePair.second);
		_requestData->SendResponseStream(logStream, OUTPUT_INFO);
		StopProcess(instancePair.first, _requestData);
		ProcessMonitorStore::RemoveInstanceByPid(instancePair.first, true);
	}
	else
	{
		_requestData->Error = ERR_PM_PROC_WITH_PID_NOT_FOUND;
		logStream << ErrorsDef::GetErrorMessage(_requestData->Error);
		_requestData->SendResponseStream(logStream, OUTPUT_ERROR);

	}

	return _requestData->Error >= SUCCESS;
}

int ProcessManager::RestartProcess(ProcessInfo const& _pInfo, BaseRequestData* _requestData)
{
	Logger::LogTrace("ProcessManager::RestartProcess()...");

	_requestData->Error = SUCCESS;

	ProcessManager::StopProcess(_pInfo, _requestData);

	return ProcessManager::StartProcess(_pInfo, _requestData);
}

pid_t ProcessManager::ForkNewProcess(ProcessVariables const& _pvar)
{
	stringstream logStream;

	Logger::LogTrace("ProcessManager::ForkNewProcess...");
	/* This function creates the brand new process and returns the PID.
   If fails to create the process, returns negative value */
	string fileName = _pvar.Args[0];
	if (!Utils::IsFileExists(fileName))
	{
		return ERR_PM_BIN_NOT_FOUND;
	}

	auto childPID = fork();
	if (0 == childPID)
	{
		//Unlink all the resources.

		const char* prefix = "/proc/";
		const char* suffix = "/fd/";
		char child_proc_dir[32];
		sprintf(child_proc_dir, "%s%d%s", prefix, getpid(), suffix);

		DIR* dir;
		struct dirent* ent;

		if ((dir = opendir(child_proc_dir)) != NULL) {
			// get files and directories within directory
			while ((ent = readdir(dir)) != NULL) {
				// convert file name to int
				char* end;
				long fd = strtol(ent->d_name, &end, 32);
				if (!*end) // valid file descriptor
				{
					fcntl((int)fd, F_SETFD, FD_CLOEXEC);
				}
			}
			closedir(dir);
		}

		if (-1 == execve(_pvar.Args[0], _pvar.Args, _pvar.Envs)) // Use any convenient exec family members.
		{
			exit(0); // Exit the child process if exec fails, else ignore as control will be
			// terminated in the child process.
		}
	}
	else if (childPID < 0)
	{
		return ERR_PM_FORK_FAILED;
	}

	return childPID;
}

pid_t ProcessManager::GetPidByProcessName(string _name)
{
	Logger::LogTrace("ProcessManager::GetPidByProcessName()...");
	DIR* dir;
	struct dirent* ent;
	char buf[512];

	long pid;
	char pname[256] = {
		0,
	};
	char state;
	FILE* fp = NULL;

	if (!(dir = opendir("/proc")))
	{
		Logger::LogError("ProcessManager::GetPidByProcessName():: can't open /proc.");
		return -1;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		long lpid = atol(ent->d_name);
		if (lpid < 0)
			continue;
		snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
		fp = fopen(buf, "r");

		if (fp)
		{
			if ((fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3)
			{
				fclose(fp);
				closedir(dir);
				return -1;
			}
			if (!strcmp(pname, _name.c_str()))
			{
				fclose(fp);
				closedir(dir);
				return (pid_t)lpid;
			}
			fclose(fp);
		}
	}
	closedir(dir);
	return -1;
}

pid_t ProcessManager::GetPidByProcessPath(string _path)
{
	Logger::LogTrace("ProcessManager::GetPidByProcessPath()...");
	DIR* dir;
	struct dirent* ent;
	char sPathBuf[128];

	if (!(dir = opendir("/proc")))
	{
		Logger::LogError("ProcessManager::GetPidByProcessPath():: can't open /proc.");
		return -1;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		long lpid = atol(ent->d_name);
		if (lpid < 0)
			continue;
		snprintf(sPathBuf, sizeof(sPathBuf), "/proc/%ld/exe", lpid);
		string strPathBuf = sPathBuf;
		string fullPath;
		if (Utils::GetAbsoluteFilePath(strPathBuf, fullPath) && !fullPath.compare(_path))
		{
			closedir(dir);
			return (pid_t)lpid;
		}
	}
	closedir(dir);
	return -1;
}

int ProcessManager::GetRunningInstanceCountByName(string _name)
{
	Logger::LogTrace("ProcessManager::GetRunningInstanceCountByName...");
	DIR* dir;
	struct dirent* ent;
	char buf[512];

	long pid;
	char pname[256] = {
		0,
	};

	int count = 0;

	char state;
	FILE* fp = NULL;

	if (!(dir = opendir("/proc")))
	{
		Logger::LogError("ProcessManager::GetRunningInstanceCountByName():: can't open /proc.");
		return -1;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		long lpid = atol(ent->d_name);
		if (lpid < 0)
			continue;
		snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
		fp = fopen(buf, "r");

		if (fp)
		{
			if ((fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3)
			{
				fclose(fp);
				continue;
			}
			if (!strcmp(pname, _name.c_str()))
			{
				count++;
			}
			fclose(fp);
		}
	}
	closedir(dir);
	return count;
}

int ProcessManager::GetRunningInstanceCountByPath(string _path)
{
	Logger::LogTrace("ProcessManager::GetRunningInstanceCountByPath()...");
	DIR* dir;
	struct dirent* ent;
	char sPathBuf[128];
	int count = 0;

	if (!(dir = opendir("/proc")))
	{
		Logger::LogError("ProcessManager::GetRunningInstanceCountByPath():: can't open /proc.");
		return -1;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		long lpid = atol(ent->d_name);
		if (lpid < 0)
			continue;
		snprintf(sPathBuf, sizeof(sPathBuf), "/proc/%ld/exe", lpid);
		string strPathBuf = sPathBuf;
		string fullPath;
		if (Utils::GetAbsoluteFilePath(strPathBuf, fullPath) && !fullPath.compare(_path))
		{
			count++;
		}
	}
	closedir(dir);
	return count;
}


ProcessConfigurationStore::ProcessConfigurationStore()
{
}

ProcessConfigurationStore::ProcessConfigurationStore(string _configVersion) :ConfigVersion(_configVersion)
{
}

ProcessConfigurationStore::~ProcessConfigurationStore()
{
	for (auto _pGroup : _ProcessGroupMap)
	{
		delete _pGroup.second;
	}
}

ProcessConfigurationStore::ProcessConfigurationStore(const ProcessConfigurationStore& _obj) :ConfigVersion(_obj.ConfigVersion)
{
	for (auto it : _obj._ProcessGroupMap)
	{
		_ProcessGroupMap.insert({ it.first,new ProcessGroup(*it.second) });
	}
}

map<string, ProcessGroup*>& ProcessConfigurationStore::Groups()
{
	return _ProcessGroupMap;
}

int ProcessConfigurationStore::size() const
{
	return (int)_ProcessGroupMap.size();
}

int ProcessConfigurationStore::clear()
{
	int count = (int)_ProcessGroupMap.size();
	for (auto _pGroup : _ProcessGroupMap)
	{
		delete _pGroup.second;
	}
	_ProcessGroupMap.clear();
	return count;
}

bool ProcessConfigurationStore::get(ProcessGroup* (&_pGroup), string _groupId)
{
	bool res = false;
	auto it = _ProcessGroupMap.find(_groupId);
	if (it != _ProcessGroupMap.end())
	{
		_pGroup = (it->second);
		res = true;
	}
	return res;
}

bool ProcessConfigurationStore::exists(string _groupId) const
{
	return _ProcessGroupMap.find(_groupId) != _ProcessGroupMap.end();
}

bool ProcessConfigurationStore::add(ProcessGroup* _pGroup)
{
	bool res = false;
	if (_ProcessGroupMap.find(_pGroup->Id) == _ProcessGroupMap.end())
	{
		_ProcessGroupMap.insert({ _pGroup->Id, _pGroup });
		res = true;
	}
	return res;
}

bool ProcessConfigurationStore::remove(string _groupId)
{
	bool res = false;
	auto it = _ProcessGroupMap.find(_groupId);
	if (it != _ProcessGroupMap.end())
	{
		_ProcessGroupMap.erase(it);
		delete it->second;
		res = true;
	}
	return res;
}

bool ProcessConfigurationStore::update(ProcessGroup& _pGroup, bool _orAdd)
{
	bool res = false;
	auto it = _ProcessGroupMap.find(_pGroup.Id);
	if (it != _ProcessGroupMap.end())
	{
		_ProcessGroupMap.erase(it);
		delete it->second;
		_ProcessGroupMap.insert({ _pGroup.Id, new ProcessGroup(_pGroup) });
		res = true;
	}
	else
	{
		if (_orAdd)
		{
			_ProcessGroupMap.insert({ _pGroup.Id, new ProcessGroup(_pGroup) });
			res = true;
		}
	}
	return res;
}

mutex ProcessMonitorStore::_Locker;

list<ProcessInstanceManager*> ProcessMonitorStore::_ProcessMonitorList;

ProcessPollManager ProcessMonitorStore::_ProcessPollMonitorList;

ProcessInstanceManager* ProcessMonitorStore::_GetProcessMonitorInfo(ProcessInfo const& _pInfo, bool _create, bool _monitorOverride)
{
	ProcessInstanceManager* res;

	if (_pInfo.Flags.IsSystemCommand)
		return NULL;

	auto entry = std::find_if(_ProcessMonitorList.begin(), _ProcessMonitorList.end(), [_pInfo](ProcessInstanceManager* _pMonitorInfo)
		{
			//Shared process check

			if (_pInfo.Flags.IsSharedProcess)
				return _pMonitorInfo->Process_Info.Flags.IsSharedProcess &&
				_pMonitorInfo->Process_Info.Id == _pInfo.Id &&
				_pMonitorInfo->Process_Info.Path == _pInfo.Path;

			return _pMonitorInfo->Process_Info.Id == _pInfo.Id &&
				_pMonitorInfo->Process_Info.GroupId == _pInfo.GroupId;
		});

	if (entry != _ProcessMonitorList.end())
	{
		res = *entry;
	}
	else if (_create)
	{
		res = new ProcessInstanceManager(const_cast <ProcessInfo&>(_pInfo), _monitorOverride);
		_ProcessMonitorList.push_back(res);

		if (!_pInfo.Flags.IsSystemCommand && _pInfo.Flags.IsMonitor && _monitorOverride)
		{
			_ProcessPollMonitorList.add(const_cast <ProcessInfo&>(_pInfo), ProcessManager::MonitorProcess);
		}
	}
	else
	{
		res = NULL;
	}

	return res;
}

bool ProcessMonitorStore::_RemoveMonitorInfo(ProcessInfo const& _pInfo)
{
	bool res = false;

	auto entry = std::find_if(_ProcessMonitorList.begin(), _ProcessMonitorList.end(), [_pInfo](ProcessInstanceManager* _pMonitorInfo)
		{
			return _pMonitorInfo->Process_Info.Id == _pInfo.Id && _pMonitorInfo->Process_Info.GroupId == _pInfo.GroupId;
		});
	if (entry != _ProcessMonitorList.end())
	{
		res = true;
		delete (*entry);
		_ProcessMonitorList.erase(entry);
		_ProcessPollMonitorList.remove(const_cast <ProcessInfo&>(_pInfo));
	}
	return res;
}

void ProcessMonitorStore::GetInstanceMapCopy(ProcessInfo const& _pInfo, map<pid_t, time_t>& _instanceMap)
{
	lock_guard<mutex> lock(_Locker);
	auto pMonitorInfo = _GetProcessMonitorInfo(_pInfo);
	if (pMonitorInfo)
		pMonitorInfo->getInstanceMapCopy(_instanceMap);
}

bool ProcessMonitorStore::GetInstanceByPid(pair<pid_t, time_t>& _instancePair)
{
	lock_guard<mutex> lock(_Locker);
	bool res = false;
	for (auto& pMonitor : _ProcessMonitorList)
	{
		if (pMonitor->getInstanceByPid(_instancePair))
		{
			res = true;
			break;
		}
	}
	return res;
}

bool ProcessMonitorStore::AddInstance(ProcessInfo const& _pInfo, pid_t _pId, const bool  _create, const bool _monitorOverride, time_t const& _time)
{
	lock_guard<mutex> lock(_Locker);
	bool res = false;
	auto pMonitorInfo = _GetProcessMonitorInfo(_pInfo, _create, _monitorOverride);
	if (pMonitorInfo)
		res = pMonitorInfo->addInstance(_pId, _time);
	return res;
}

bool ProcessMonitorStore::RemoveInstance(ProcessInfo const& _pInfo, pid_t _pId, const bool  _removeEntry)
{
	lock_guard<mutex> lock(_Locker);
	bool res = false;
	auto pMonitorInfo = _GetProcessMonitorInfo(_pInfo);
	if (pMonitorInfo)
	{
		res = pMonitorInfo->removeInstance(_pId);
		if (_removeEntry && !pMonitorInfo->getInstanceCount())
			_RemoveMonitorInfo(_pInfo);
	}
	return res;
}

bool ProcessMonitorStore::RemoveInstanceByPid(pid_t _pid, const bool  _removeEntry)
{
	lock_guard<mutex> lock(_Locker);
	bool res = false;
	for (auto pMonitorInfo : _ProcessMonitorList)
	{
		if (pMonitorInfo->removeInstance(_pid))
		{
			if (_removeEntry && !pMonitorInfo->getInstanceCount())
				_RemoveMonitorInfo(pMonitorInfo->Process_Info);
			res = true;
			break;
		}
	}
	return res;
}

int ProcessMonitorStore::GetInstanceCount(ProcessInfo const& _pInfo)
{
	int res = 0;
	lock_guard<mutex> lock(_Locker);
	auto pMonitorInfo = _GetProcessMonitorInfo(_pInfo);
	if (pMonitorInfo)
		res = pMonitorInfo->getInstanceCount();
	return res;
}

int ProcessMonitorStore::GetRunningProcessCount()
{
	lock_guard<mutex> lock(_Locker);
	return (int)_ProcessMonitorList.size();

}

void ProcessMonitorStore::ClearMonitorInfoStore()
{

	lock_guard<mutex> lock(_Locker);

	for (auto const& entry : _ProcessMonitorList)
	{
		delete entry;
	}
	_ProcessMonitorList.clear();
	_ProcessPollMonitorList.clear();
}