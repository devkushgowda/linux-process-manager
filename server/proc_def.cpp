#include "proc_def.hpp"

using namespace dpm;

ProcessVariables::ProcessVariables() :_CurrentArgIndex(0), _CurrentEnvIndex(0)
{
	for (int i = 0; i < MAX_ARG_COUNT - 1; ++i)
	{
		Envs[i] = Args[i] = NULL;
	}
}

ProcessVariables::~ProcessVariables()
{
	clear();
}

ProcessVariables& ProcessVariables::operator=(ProcessVariables const& _obj)
{
	clear();
	for (int i = 0; i < MAX_ARG_COUNT - 1; ++i)
	{
		Envs[i] = Args[i] = NULL;
	}

	for (int i = 0; i < _obj._CurrentArgIndex; ++i)
	{
		addArg(_obj.Args[i]);
	}

	for (int i = 0; i < _obj._CurrentEnvIndex; ++i)
	{
		addArg(_obj.Envs[i]);
	}
	return *this;
}

ProcessVariables::ProcessVariables(const ProcessVariables& _obj) :_CurrentArgIndex(0), _CurrentEnvIndex(0)
{
	for (int i = 0; i < MAX_ARG_COUNT - 1; ++i)
	{
		Envs[i] = Args[i] = NULL;
	}

	for (int i = 0; i < _obj._CurrentArgIndex; ++i)
	{
		addArg(_obj.Args[i]);
	}
	for (int i = 0; i < _obj._CurrentEnvIndex; ++i)
	{
		addArg(_obj.Envs[i]);
	}
}

int ProcessVariables::getArgsCount() const
{
	return _CurrentArgIndex + 1;
}

int ProcessVariables::getEnvsCount() const
{
	return _CurrentEnvIndex + 1;
}

void ProcessVariables::clear()
{
	for (int i = 0; i < _CurrentArgIndex; ++i)
	{
		delete[] Args[i];
		Args[i] = NULL;
	}
	for (int i = 0; i < _CurrentEnvIndex; ++i)
	{
		delete[] Envs[i];
		Envs[i] = NULL;
	}
	_CurrentArgIndex = _CurrentEnvIndex = 0;
}

int ProcessVariables::addArg(char const* _arg)
{
	if (_arg == NULL)
		return -1;
	int curIndex = _CurrentArgIndex++;

	if (curIndex > MAX_ARG_COUNT)
		return 0;

	int len = (int)strlen(_arg) + 1;
	Args[curIndex] = new char[len];
	strcpy(Args[curIndex], _arg);
	return len;
}

int ProcessVariables::addEnv(char const* _env)
{
	if (_env == NULL)
		return -1;
	int curIndex = _CurrentEnvIndex++;

	if (curIndex > MAX_ARG_COUNT)
		return 0;

	int len = (int)strlen(_env) + 1;
	Envs[curIndex] = new char[len];
	strcpy(Envs[curIndex], _env);
	return len;
}


ProcessFlags::ProcessFlags()
	:IsSystemCommand(true)
	, IsKillMultiple(true)
	, IsRestartAll(false)
	, IsMonitor(false)
	, IsSharedProcess(false)
{
}

ProcessInfo::ProcessInfo() :Variables(), Flags(), Dependency()
{
}

ProcessInfo::ProcessInfo(string _id) : Id(_id), Variables(), Flags(), Dependency()
{
}

ProcessInfo& ProcessInfo::operator=(ProcessInfo const& _obj)
{
	Enable = _obj.Enable;
	Id = _obj.Id;
	GroupId = _obj.GroupId;
	Name = _obj.Name;
	Path = _obj.Path;
	StartDelay = _obj.StartDelay;
	MaxAllowedInstance = _obj.MaxAllowedInstance;
	Flags = _obj.Flags;
	Variables = _obj.Variables;
	Dependency = _obj.Dependency;
	return *this;
}


ProcessInfo::ProcessInfo(const ProcessInfo& _obj)
	: Enable(_obj.Enable)
	, Id(_obj.Id)
	, GroupId(_obj.GroupId)
	, Name(_obj.Name)
	, Path(_obj.Path)
	, StartDelay(_obj.StartDelay)
	, MaxAllowedInstance(_obj.MaxAllowedInstance)
	, Flags(_obj.Flags)
	, Variables(_obj.Variables)
	, Dependency(_obj.Dependency)
{
}


ProcessGroup::ProcessGroup()
{
}

ProcessGroup::ProcessGroup(string _groupId) :Id(_groupId)
{
}

ProcessGroup::ProcessGroup(bool _enable, string _groupId, string _description, int _pollInterval, string _target)
	: Enable(_enable)
	, Id(_groupId)
	, Description(_description)
{
}

ProcessGroup::ProcessGroup(const ProcessGroup& _obj)
	: Enable(_obj.Enable)
	, Id(_obj.Id)
	, Description(_obj.Description)
{
	for (auto it : _obj._ProcessMap)
	{
		_ProcessMap.insert({ it.first, it.second });
	}
}

ProcessGroup& ProcessGroup::operator=(ProcessGroup const& _obj)
{
	Enable = _obj.Enable;
	Id = _obj.Id;
	Description = _obj.Description;
	_ProcessMap.clear();
	for (auto it : _obj._ProcessMap)
	{
		_ProcessMap.insert({ it.first, it.second });
	}
	return *this;
}

const map<string, ProcessInfo>& ProcessGroup::ProcesMap()
{
	return _ProcessMap;
}

int ProcessGroup::size() const
{
	return (int)_ProcessMap.size();
}

int ProcessGroup::clear()
{
	int count = (int)_ProcessMap.size();
	_ProcessMap.clear();
	return count;
}

bool ProcessGroup::get(ProcessInfo& _pInfo)
{
	bool res = false;

	if (_pInfo.Id.size() <= 0)
		return res;

	auto it = _ProcessMap.find(_pInfo.Id);
	if (it != _ProcessMap.end())
	{
		_pInfo = it->second;
		res = true;
	}
	return res;
}

bool ProcessGroup::processExists(string _pId) const
{
	return _ProcessMap.find(_pId) != _ProcessMap.end();
}

bool ProcessGroup::add(ProcessInfo& _pInfo)
{
	bool res = false;

	if (_pInfo.Id.size() <= 0)
		return res;

	if (_ProcessMap.find(_pInfo.Id) == _ProcessMap.end())
	{
		_ProcessMap.insert({ _pInfo.Id, _pInfo });
		res = true;
	}
	return res;
}

bool ProcessGroup::removeProcess(string _pId)
{
	bool res = false;

	if (_pId.size() <= 0)
		return res;

	auto it = _ProcessMap.find(_pId);
	if (it != _ProcessMap.end())
	{
		_ProcessMap.erase(it);
		res = true;
	}
	return res;
}

bool ProcessGroup::update(ProcessInfo& _pInfo, bool _orAdd)
{
	bool res = false;

	if (_pInfo.Id.size() <= 0)
		return res;

	auto it = _ProcessMap.find(_pInfo.Id);
	if (it != _ProcessMap.end())
	{
		_ProcessMap.erase(it);
		_ProcessMap.insert({ _pInfo.Id, _pInfo });
		res = true;
	}
	else
	{
		if (_orAdd)
		{
			_ProcessMap.insert({ _pInfo.Id, _pInfo });
			res = true;
		}
	}
	return res;
}

bool ProcessGroup::getAllProcesses(vector<ProcessInfo>& _list, bool _enable)
{
	bool res = false;
	for (auto pInfo : _ProcessMap)
	{
		if (_enable)
			if (!pInfo.second.Enable)
				continue;
		_list.push_back(pInfo.second);
		res = true;
	}
	return res;
}

bool ProcessGroup::get(ShmInfo& _shmInfo)
{
	bool res = false;

	if (_shmInfo.Id.size() <= 0)
		return res;

	auto it = _ShmMap.find(_shmInfo.Id);
	if (it != _ShmMap.end())
	{
		_shmInfo = it->second;
		res = true;
	}
	return res;
}

bool ProcessGroup::shmExists(string _sId) const
{
	return _ShmMap.find(_sId) != _ShmMap.end();
}

bool ProcessGroup::add(ShmInfo& _shmInfo)
{
	bool res = false;

	if (_shmInfo.Id.size() <= 0 || !SharedMemoryFactory::IsValid(_shmInfo.Id))
		return res;

	if (_ShmMap.find(_shmInfo.Id) == _ShmMap.end())
	{
		_ShmMap.insert({ _shmInfo.Id, _shmInfo });
		res = true;
	}
	return res;
}

bool ProcessGroup::removeShm(string _sId)
{
	bool res = false;

	if (_sId.size() <= 0)
		return res;

	auto it = _ShmMap.find(_sId);
	if (it != _ShmMap.end())
	{
		_ShmMap.erase(it);
		res = true;
	}
	return res;
}

bool ProcessGroup::update(ShmInfo& _shmInfo, bool _orAdd)
{
	bool res = false;

	if (_shmInfo.Id.size() <= 0)
		return res;

	auto it = _ShmMap.find(_shmInfo.Id);
	if (it != _ShmMap.end())
	{
		_ShmMap.erase(it);
		_ShmMap.insert({ _shmInfo.Id, _shmInfo });
		res = true;
	}
	else
	{
		if (_orAdd)
		{
			_ShmMap.insert({ _shmInfo.Id, _shmInfo });
			res = true;
		}
	}
	return res;
}

bool ProcessGroup::getAllShm(vector<ShmInfo>& _list, bool _enable)
{
	bool res = false;
	for (auto sInfo : _ShmMap)
	{
		if (_enable)
			if (!sInfo.second.Enable)
				continue;
		_list.push_back(sInfo.second);
		res = true;
	}
	return res;
}


ProcessInstanceManager::ProcessInstanceManager(ProcessInfo& _pInfo, bool _isMonitor) :Process_Info(_pInfo), IsMonitor(_IsMonitor), _IsMonitor(_isMonitor)
{
}

ProcessInstanceManager& ProcessInstanceManager::operator=(ProcessInstanceManager const& _obj)
{
	_IsMonitor = _obj._IsMonitor;
	Process_Info = _obj.Process_Info;
	_InstanceMap.clear();
	for (auto const& entry : _obj._InstanceMap)
	{
		_InstanceMap.insert(entry);
	}
	return *this;
}

ProcessInstanceManager::ProcessInstanceManager(const ProcessInstanceManager& _obj) :Process_Info(_obj.Process_Info), IsMonitor(_IsMonitor), _IsMonitor(_obj._IsMonitor)
{
	for (auto const& entry : _obj._InstanceMap)
	{
		_InstanceMap.insert(entry);
	}
}

bool ProcessInstanceManager::addInstance(pid_t _pId, time_t const& _time)
{
	bool res = true;
	auto entry = _InstanceMap.find(_pId);
	if (entry != _InstanceMap.end())
	{
		res = false;
	}
	_InstanceMap.insert({ _pId ,_time });
	return res;
}

bool ProcessInstanceManager::removeInstance(pid_t _pId)
{
	bool res = false;
	auto entry = _InstanceMap.find(_pId);
	if (entry != _InstanceMap.end())
	{
		_InstanceMap.erase(entry);
		res = true;
	}
	return res;
}

void ProcessInstanceManager::clear()
{
	_InstanceMap.clear();
}

int ProcessInstanceManager::getInstanceCount()
{
	return (int)_InstanceMap.size();
}

void ProcessInstanceManager::getInstanceMapCopy(map<pid_t, time_t>& _instanceMap)
{
	for (auto const& instance : _InstanceMap)
		_instanceMap.insert(instance);
}

bool ProcessInstanceManager::getInstanceByPid(pair< pid_t, time_t>& _instancePair)
{
	bool res = false;
	auto instance = _InstanceMap.find(_instancePair.first);
	if (instance != _InstanceMap.end())
	{
		_instancePair.second = instance->second;
		res = true;
	}
	return res;
}

ShmInfo::ShmInfo(string _id) :Id(_id)
{
}

ShmInfo& ShmInfo::operator=(ShmInfo const& _obj)
{
	Enable = _obj.Enable;
	GroupId = _obj.GroupId;
	Id = _obj.Id;
	Name = _obj.Name;
	ShmId = _obj.ShmId;
	ShmPath = _obj.ShmPath;
	return *this;
}

ShmInfo::ShmInfo(const ShmInfo& _obj) :Enable(_obj.Enable), GroupId(_obj.GroupId), Id(_obj.Id), Name(_obj.Name), ShmId(_obj.ShmId), ShmPath(_obj.ShmPath)
{

}

ProcessDependency& ProcessDependency::operator=(ProcessDependency const& _obj)
{
	this->Dependency.clear();
	for (auto entry : _obj.Dependency)
		this->Dependency.push_back(entry);
	return *this;
}

ProcessDependency::ProcessDependency(const ProcessDependency& _obj)
{
	for (auto entry : _obj.Dependency)
		this->Dependency.push_back(entry);
}
