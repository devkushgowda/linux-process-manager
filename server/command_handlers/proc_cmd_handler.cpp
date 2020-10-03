#include "proc_cmd_handler.hpp"

using namespace dpm;

ProcCmdHandler::ProcCmdHandler(RequestData& _requestData) :BaseCmdHandler(_requestData)
{
}


void ProcCmdHandler::execute()
{
	stringstream logStream;
	if (GlobalMutex::ProcConfigLock.lockPrimary())
	{
		DPM_TRY {
			_Execute();
		}
		DPM_CATCH_LOG("ProcCmdHandler::execute()");
		GlobalMutex::ProcConfigLock.unlockPrimary();
	}
	else
	{
		logStream << ErrorsDef::GetErrorMessage(ERR_PROC_HANDLER_BUSY);
		_RequestData.SendResponseStream(logStream, OUTPUT_WARN);
		_RequestData.Error = ERR_PROC_HANDLER_BUSY;
		return;
	}
}

void ProcCmdHandler::_Execute()
{

	int paramCount = _RequestData.RequestPDU.getParametersCount();
	string groupId, pId;
	vector<ProcessInfo> procList;
	stringstream logStream;
	auto commandOpt = (OPT_ENUM)_RequestData.RequestPDU.getPrameterAt(0).vint;
	//Default all
	if (paramCount == 2 && OPT_ALL == commandOpt)
	{
		groupId = _RequestData.RequestPDU.getPrameterAt(1).vstring;
		ProcessConfigStoreManager::getAllProcesses(_RequestData.Error, procList, groupId);
	}
	else if (paramCount == 3)
	{
		groupId = _RequestData.RequestPDU.getPrameterAt(1).vstring;
		pId = _RequestData.RequestPDU.getPrameterAt(2).vstring;
		ProcessInfo pInfo(pId);
		ProcessConfigStoreManager::GetProcessConfigById(_RequestData.Error, pInfo, groupId);
		if (_RequestData.Error == SUCCESS)
			procList.push_back(pInfo);
	}
	if (_RequestData.Error != SUCCESS)
	{
		logStream << ErrorsDef::GetErrorMessage(_RequestData.Error);
		_RequestData.SendResponseStream(logStream, OUTPUT_ERROR);
	}
	else
	{
		switch (_RequestData.RequestPDU.getFunctionCode())
		{
		case CMD_START:
		{
			for (auto& proc : procList)
			{
				logStream << "Starting process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
				_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
				ProcessManager::StartProcess(proc, &_RequestData);
			}
			break;
		}
		case CMD_STOP:
		{
			switch (commandOpt)
			{
			case OPT_BY_PID:
			{
				pid_t pid = (pid_t)atoi(_RequestData.RequestPDU.getPrameterAt(1).vstring);
				logStream << "Stopping process -> pid: '" << pid << "'";
				_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
				ProcessManager::StopProcessByPid(pid, &_RequestData);
			}
			break;
			default:
				if (procList.size() == 0)
				{
					logStream << "No process are available to operate.";
					_RequestData.SendResponseStream(logStream, OUTPUT_WARN);
					return;
				}
				for (auto& proc : procList)
				{
					logStream << "Stopping process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
					_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
					ProcessManager::StopProcess(proc, &_RequestData);
				}
				break;
			}
			break;
		}
		case CMD_RESTART:
		{
			if (procList.size() == 1)
			{
				for (auto& proc : procList)
				{
					logStream << "Restarting process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
					_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
					ProcessManager::RestartProcess(proc, &_RequestData);
				}
			}
			else
			{
				for (auto& proc : procList)
				{
					logStream << "Stopping process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
					_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
					ProcessManager::StopProcess(proc, &_RequestData);
				}
				for (auto& proc : procList)
				{
					logStream << "Restarting process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
					_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);
					ProcessManager::StartProcess(proc, &_RequestData);
				}
			}
			break;
		}
		case CMD_PING:
		{
			for (auto& proc : procList)
			{
				logStream << "Pinging process -> Id: '" << proc.Id << "', GroupId: '" << proc.GroupId->Id << "' , Name: '" << proc.Name << "' , Path: '" << proc.Path << "'";
				_RequestData.SendResponseStream(logStream, OUTPUT_HEAD);

				if (ProcessMonitorStore::GetInstanceCount(proc) < 0)
				{
					logStream << "No instances are available yet.";
					_RequestData.SendResponseStream(logStream, OUTPUT_WARN);
					break;
				}

				map<pid_t, time_t> instanceMap;
				ProcessMonitorStore::GetInstanceMapCopy(proc, instanceMap);

				if (instanceMap.size() > 0)
				{
					logStream << instanceMap.size() << " instances are available.";
					_RequestData.SendResponseStream(logStream, OUTPUT_SUBHEAD);
					for (auto const& instance : instanceMap)
					{
						if (ProcessManager::PingProcess(instance.first) == PING_RUNNUNG)
						{
							logStream << "Instance with pid: " << instance.first << " running for " << Utils::GetTimeIntervalString(instance.second);
							_RequestData.SendResponseStream(logStream, OUTPUT_INFO);
						}
						else
						{
							//ProcessMonitorStore::RemoveInstance(proc, instance.first);
							logStream << "Instance with pid: " << instance.first << " not running.\nProcess was started at " << ctime(&instance.second);
							logStream << "Instance with pid: " << instance.first << " not running.\nProcess ran for " << Utils::GetTimeIntervalString(instance.second);
							_RequestData.SendResponseStream(logStream, OUTPUT_ERROR);
						}
					}
				}
				else
				{
					logStream << "No instances are running.";
					_RequestData.SendResponseStream(logStream, OUTPUT_WARN);
				}
			}
			break;
		}
		default:
			break;
		}
	}
}
