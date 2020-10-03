#include "proc_poll_manager.hpp"

using namespace dpm;

ProcessPollManager::ProcessPollManager() : BasePollManager(ServerConfigurationManager::GetPollIntervalPtr(), &(GlobalMutex::ProcConfigLock))
{
}

ProcessPollManager::~ProcessPollManager()
{
}



void ProcessPollManager::add(ProcessInfo& _pInfo, function<void(ProcessInfo&)> _pollFunc)
{
	lock_guard<mutex> lock(_Locker);
	_stop();
	_TasksList.push_back({ _pInfo ,_pollFunc });
	_start();
}

bool ProcessPollManager::remove(ProcessInfo& _pInfo)
{
	lock_guard<mutex> lock(_Locker);
	_stop();
	bool res = false;
	auto entry = std::find_if(_TasksList.begin(), _TasksList.end(), [_pInfo](pair<ProcessInfo, function<void(ProcessInfo&)>> const& _pMonitorInfo)
		{
			return _pMonitorInfo.first.Id == _pInfo.Id && _pMonitorInfo.first.GroupId == _pInfo.GroupId;
		});

	if (entry != _TasksList.end())
	{
		_TasksList.erase(entry);
		res = true;
	}
	_start();
	return res;
}




