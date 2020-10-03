#include "config_manager.hpp"

using namespace dpm;


atomic<bool> ProcessConfigStoreManager::Loaded(false);
mutex ProcessConfigStoreManager::_Locker;

ProcessConfigurationStore ProcessConfigStoreManager::_ProcessConfigStore;

bool ProcessConfigStoreManager::_Save(ERR_ENUM& _err, string _filePath)
{
	_err = SUCCESS;

	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
	}
	return true;
}

bool ProcessConfigStoreManager::_Load(ERR_ENUM& _err, bool _clear, string _filePath)
{
	_err = SUCCESS;
	ProcessConfigurationParser::Parse(_err, _filePath, _ProcessConfigStore, _clear);
	ProcessConfigStoreManager::Loaded.store(_err >= 0, memory_order_release);
	return Loaded.load(memory_order_acquire);
}


bool ProcessConfigStoreManager::ReloadConfiguration(ERR_ENUM& _err, bool _clear, string _filePath)
{
	lock_guard<mutex> lock(_Locker);
	string resolvedFilePath;

	if (!Utils::GetAbsoluteFilePath(_filePath, resolvedFilePath))
	{
		Utils::GetCurrentExeDir(resolvedFilePath);
		resolvedFilePath += PROCESS_CONFIG_DEFAULT_FILE_NAME;
	}
	Logger::LogInfo("ProcessConfigStoreManager::ReloadConfiguration(): Configuration file path resolved to -> " + resolvedFilePath);
	return _Load(_err, _clear, resolvedFilePath);
}

bool ProcessConfigStoreManager::GetProcessConfigById(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId, bool _enabled)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return false;
	}

	lock_guard<mutex> lock(_Locker);

	bool res = false;
	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		if (group->get(_pInfo))
		{
			if (_enabled)
			{
				if (_pInfo.Enable)
				{
					res = true;
				}
				else
				{
					_err = ERR_CONFIG_DISABLED;
				}
			}
			else
			{
				res = true;
			}
		}
		else
		{
			_err = ERR_CONFIG_PINFO_NOT_FOUND;
		}
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}
	return res;
}

bool ProcessConfigStoreManager::AddProcessConfig(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId, string _afterPid)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return false;
	}

	lock_guard<mutex> lock(_Locker);

	bool res = false;
	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		if (group->processExists(_pInfo.Id))
		{
			_err = ERR_CONFIG_PINFO_ALREADY_EXISTS;
		}
		else
		{
			group->add(_pInfo);
			res = true;
		}
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}
	return res;
}

bool ProcessConfigStoreManager::RemoveProcessConfig(ERR_ENUM& _err, string _pId, string _gId)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return false;
	}

	lock_guard<mutex> lock(_Locker);

	bool res = false;
	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		if (group->processExists(_pId))
		{
			group->removeProcess(_pId);
			res = true;
		}
		else
		{
			_err = ERR_CONFIG_PINFO_NOT_FOUND;
		}
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}
	return res;
}

bool ProcessConfigStoreManager::UpdateProcessConfig(ERR_ENUM& _err, ProcessInfo& _pInfo, string _gId, bool _orAdd)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return false;
	}

	lock_guard<mutex> lock(_Locker);

	bool res = false;
	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		if (group->processExists(_pInfo.Id))
		{
			group->update(_pInfo);
			res = true;
		}
		else
		{
			if (_orAdd)
			{
				group->add(_pInfo);
				res = true;
			}
			else
			{
				_err = ERR_CONFIG_PINFO_UPDATE_SKIPPED;
			}
		}
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}
	return res;
}

bool ProcessConfigStoreManager::getAllProcesses(ERR_ENUM& _err, vector<ProcessInfo>& list, string _gId, bool _enabled)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return false;
	}

	lock_guard<mutex> lock(_Locker);

	bool res = false;
	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		res = group->getAllProcesses(list, _enabled);
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}
	return res;
}

void ProcessConfigStoreManager::GetAllGroupConfigDescription(ERR_ENUM& _err, stringstream& _op)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return;
	}

	lock_guard<mutex> lock(_Locker);
	int groupNo = 1;
	for (auto const& group : _ProcessConfigStore.Groups())
	{
		FormatProcessGroup(groupNo++, _op, group.second);
	}

	if (groupNo == 1)
	{
		_err = ERR_CONFIG_PGROUP_EMPTY;
	}
}

void ProcessConfigStoreManager::GetGroupConfigDescription(ERR_ENUM& _err, stringstream& _op, string _gId)
{
	if (!ProcessConfigStoreManager::Loaded.load(memory_order_acquire))
	{
		_err = ERR_CONFIG_NOT_LOADED;
		return;
	}

	lock_guard<mutex> lock(_Locker);

	ProcessGroup* group = NULL;
	_err = SUCCESS;

	if (_ProcessConfigStore.get(group, _gId))
	{
		FormatProcessGroup(1, _op, group);
	}
	else
	{
		_err = ERR_CONFIG_PGROUP_NOT_FOUND;
	}

}

void ProcessConfigStoreManager::GetProcessConfigDescription(ERR_ENUM& _err, stringstream& _op, string _pid, string _gId)
{
	ProcessInfo pInfo(_pid);
	if (GetProcessConfigById(_err, pInfo, _gId))
		FormatProcessInfo(1, _op, pInfo);
}

bool ProcessConfigStoreManager::FormatProcessGroup(int const& _count, stringstream& _op, ProcessGroup* const _pGroup, bool _onlyEnabled)
{
	stringstream logStream;

	logStream << endl << _count << " . Process Group -> Id: '" << _pGroup->Id << "', Description: '" << _pGroup->Description << "'" << endl;
	Logger::OpHead(_op, logStream);

	vector<ProcessInfo> list;
	bool hasProcess = _pGroup->getAllProcesses(list, false);
	int count = 1;

	for (auto pInfo : list)
	{
		if (_onlyEnabled && !pInfo.Enable)
			continue;
		FormatProcessInfo(count++, _op, pInfo);
	}
	if (!hasProcess)
	{
		logStream << "Process group is empty.";
		Logger::OpWarn(_op, logStream);
	}
	return hasProcess;
}

void ProcessConfigStoreManager::FormatProcessInfo(int const& _count, stringstream& _op, ProcessInfo const& _pInfo)
{
	stringstream logStream;

	logStream << endl << _count << " . Process -> Id: '" << _pInfo.Id << "', Enabled: '" << _pInfo.Enable << "', Path: '" << _pInfo.Path << "',  MaxAllowedInstance: '" << _pInfo.MaxAllowedInstance << "', StartDelay: '" << _pInfo.StartDelay << "'" << endl;

	logStream << endl << "Flags," << endl <<
		"\tIsSystemCommand: '" << _pInfo.Flags.IsSystemCommand << "'" << endl <<
		"\tIsMonitor : '" << _pInfo.Flags.IsMonitor << "'" << endl <<
		"\tIsKillMultiple: '" << _pInfo.Flags.IsKillMultiple << "'" << endl <<
		"\tIsSharedProcess: '" << _pInfo.Flags.IsSharedProcess << "'" << endl <<
		"\tIsRestartAll: '" << _pInfo.Flags.IsRestartAll << "'" << endl;

	logStream << endl << "Args" << endl;

	for (int i = 0; i < _pInfo.Variables.getArgsCount() - 1; i++)
		logStream << "\tArg " << i + 1 << " : " << _pInfo.Variables.Args[i] << endl;


	logStream << endl << "Envs," << endl;

	for (int i = 0; i < _pInfo.Variables.getEnvsCount() - 1; i++)
		logStream << "\tEnv " << i + 1 << " : " << _pInfo.Variables.Envs[i] << endl;

	Logger::OpInfo(_op, logStream);

}
