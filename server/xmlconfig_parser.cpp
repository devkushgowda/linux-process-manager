#include "xmlconfig_parser.hpp"
#include "../common/common_def.h"
using namespace dpm;

void ProcessConfigurationParser::Parse(ERR_ENUM& _err, string _file, ProcessConfigurationStore& _pcs, bool _clear)
{
	Logger::LogTrace("ProcessConfigurationParser::Parse()...");

	if (_clear)
		_pcs.clear();

	xml_document doc;
	_err = SUCCESS;

	if (!doc.load_file(_file.c_str()))
	{
		_err = ERR_PROC_XML_CONFIG_PARSER_INVALID_CONFIG_FILE;
		return;
	}

	auto rootNode = doc.child(PROC_CONFIG_XML_ROOT);
	if (rootNode)
	{
		_ParseRoot(_err, rootNode, _pcs);
	}
	else
	{
		_err = ERR_PROC_XML_CONFIG_PARSER_ROOT_NOT_FOUND;
	}
}

bool ProcessConfigurationParser::_ParseRoot(ERR_ENUM& _err, xml_node& _node, ProcessConfigurationStore& _pcs)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseRoot()...");

	if (!_node)
		return false;

	stringstream logStream;

	_err = SUCCESS;

	auto configVersion = _node.attribute(PROC_CONFIG_XML_ATTR_CONFIG_VERSION);

	_pcs.ConfigVersion = configVersion ? configVersion.as_string() : PROCESS_CONFIG_DEFAULT_VERSION;

	logStream << "ConfigVersion: '" << _pcs.ConfigVersion << "'";
	Logger::LogInfo(logStream);

	auto groupsNode = _node.child(PROC_CONFIG_XML_NODE_GROUPS);

	if (!groupsNode)
	{
		_err = ERR_PROC_XML_CONFIG_PARSER_GROUPS_NOT_FOUND;
		return false;
	}
	else
	{
		return _ParseGroups(_err, groupsNode, _pcs);
	}

}

bool ProcessConfigurationParser::_ParseGroups(ERR_ENUM& _err, xml_node& _node, ProcessConfigurationStore& _pcs)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseGroups()...");

	if (!_node)
		return false;

	_err = SUCCESS;
	stringstream logStream;

	bool hasGroup = false;
	bool hasProcess = false;
	auto groupsNode = _node.children(PROC_CONFIG_XML_NODE_GROUP);
	for (auto groupNode : groupsNode)
	{
		if (groupNode.attribute(PROC_CONFIG_XML_ATTR_IGNORE).as_bool())
		{
			logStream << "Ignoring process configuration group -> gId: '" << groupNode.attribute(PROC_CONFIG_XML_ATTR_ID).as_string() << "'";
			Logger::LogWarn(logStream);
			continue;
		}
		ProcessGroup* pg = new ProcessGroup();
		hasGroup = true;
		bool groupParse = _ParseGroup(groupNode, *pg);
		hasProcess = groupParse || hasProcess;
		if (_pcs.add(pg))
		{
			logStream << "Added group configuration -> gId: '" << pg->Id << "'";
			Logger::LogInfo(logStream);
		}
		else
		{
			logStream << "Ignored group configuration -> gId: '" << pg->Id << "'. Group might already exists or Id is invalid";
			Logger::LogWarn(logStream);
			delete pg;
		}
	}

	if (!hasGroup)
	{
		_err = ERR_PROC_XML_CONFIG_PARSER_GROUPS_NOT_FOUND;
		return false;
	}

	if (!hasProcess)
	{
		_err = ERR_PROC_XML_CONFIG_PARSER_PROCESS_NOT_FOUND;
		return false;
	}

	return true;
}

bool ProcessConfigurationParser::_ParseGroup(xml_node& _node, ProcessGroup& _pg)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseGroup()...");

	if (!_node)
		return false;

	stringstream logStream;

	auto id = _node.attribute(PROC_CONFIG_XML_ATTR_ID);
	_pg.Id = id.as_string();

	if (_pg.Id.size() == 0)
		return false;

	logStream << "Parsing process group configuration -> gId: '" << _pg.Id << "'";
	Logger::LogTrace(logStream);


	auto enable = _node.attribute(PROC_CONFIG_XML_ATTR_ENABLE);
	_pg.Enable = enable ? enable.as_bool() : true;

	logStream << "ProcessGroup::enable: '" << _pg.Enable << "'";
	Logger::LogTrace(logStream);

	auto description = _node.attribute(PROC_CONFIG_XML_ATTR_DESCRIPTION);
	_pg.Description = description.as_string();

	logStream << "ProcessGroup::description: '" << _pg.Description << "'";
	Logger::LogTrace(logStream);

	bool hasProcess = false, hasShm = false;

	auto shmNodes = _node.children(PROC_CONFIG_XML_NODE_SHM);

	for (auto shmNode : shmNodes)
	{
		if (shmNode.attribute(PROC_CONFIG_XML_ATTR_IGNORE).as_bool())
		{

			logStream << "Ignoring shared memory configuration -> gId: '" << _pg.Id << "', pId: '" << shmNode.attribute(PROC_CONFIG_XML_ATTR_ID).as_string();
			Logger::LogWarn(logStream);
			continue;
		}
		ShmInfo si;
		si.GroupId = &_pg;
		if (_ParseShm(shmNode, si))
		{
			hasShm = true;
		}
		else
		{

			logStream << "Ignored shared memory configuration -> gId: '" << _pg.Id << "', pId: '" << si.Id << "', Name: '" << si.Name << "'. Invalid configuration.";
			Logger::LogWarn(logStream);
			continue;
		}
		if (_pg.add(si))
		{

			logStream << "Added shared memory configuration -> gId: '" << _pg.Id << "', pId: '" << si.Id;
			Logger::LogInfo(logStream);
		}
		else
		{

			logStream << "Ignored shared memory configuration -> gId: '" << _pg.Id << "', pId: '" << si.Id << "'. Configuration might already exists or Id is invalid";
			Logger::LogWarn(logStream);
		}
	}

	auto processNodes = _node.children(PROC_CONFIG_XML_NODE_PROCESS);

	for (auto processNode : processNodes)
	{
		if (processNode.attribute(PROC_CONFIG_XML_ATTR_IGNORE).as_bool())
		{

			logStream << "Ignoring process configuration -> gId: '" << _pg.Id << "', pId: '" << processNode.attribute(PROC_CONFIG_XML_ATTR_ID).as_string();
			Logger::LogWarn(logStream);
			continue;
		}
		ProcessInfo pi;
		pi.GroupId = &_pg;
		if (_ParseProcess(processNode, pi))
		{
			hasProcess = true;
		}
		else
		{

			logStream << "Ignored process configuration -> gId: '" << _pg.Id << "', pId: '" << pi.Id << "', Name: '" << pi.Name << "', Path: '" << pi.Path << "'. Invalid process configuration.";
			Logger::LogWarn(logStream);
			continue;
		}
		if (_pg.add(pi))
		{

			logStream << "Added process configuration -> gId: '" << _pg.Id << "', pId: '" << pi.Id;
			Logger::LogInfo(logStream);
		}
		else
		{

			logStream << "Ignored process configuration -> gId: '" << _pg.Id << "', pId: '" << pi.Id << "'. Process might already exists or Id is invalid";
			Logger::LogWarn(logStream);
		}
	}

	return hasProcess || hasShm;
}

bool ProcessConfigurationParser::_ParseProcess(xml_node& _node, ProcessInfo& _pi)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseProcess()...");

	if (!_node)
		return false;

	stringstream logStream;

	auto id = _node.attribute(PROC_CONFIG_XML_ATTR_ID);
	_pi.Id = id.as_string();

	if (_pi.Id.size() == 0)
		return false;

	logStream << "Parsing process configuration -> pId: '" << _pi.Id << "'";
	Logger::LogTrace(logStream);

	auto enable = _node.attribute(PROC_CONFIG_XML_ATTR_ENABLE);
	_pi.Enable = enable ? enable.as_bool() : true;

	logStream << "Process::enable: '" << _pi.Enable << "'";
	Logger::LogTrace(logStream);

	auto name = _node.attribute(PROC_CONFIG_XML_ATTR_NAME);
	_pi.Name = name.as_string();

	logStream << "Process::name: '" << _pi.Name << "'";
	Logger::LogTrace(logStream);

	auto path = _node.attribute(PROC_CONFIG_XML_ATTR_PATH);
	_pi.Path = path.as_string();

	logStream << "Process::path: '" << _pi.Path << "'";
	Logger::LogTrace(logStream);

	auto delay = _node.attribute(PROC_CONFIG_XML_ATTR_DELAY);
	int delayValue = delay.as_int();
	_pi.StartDelay = delayValue > 0 ? delayValue : 0;

	logStream << "Process::delay: '" << _pi.StartDelay << "'";
	Logger::LogTrace(logStream);

	auto maxInstance = _node.attribute(PROC_CONFIG_XML_ATTR_MAX_INSTANCE);
	int maxInstanceValue = maxInstance.as_int();
	_pi.MaxAllowedInstance = maxInstanceValue > 0 ? maxInstanceValue : 1;

	logStream << "Process::maxAllowedInstance: '" << _pi.MaxAllowedInstance << "'";
	Logger::LogTrace(logStream);

	if (_pi.Path.size() == 0)
		return false;

	auto flags = _node.child(PROC_CONFIG_XML_NODE_FLAGS);
	_ParseFlags(flags, _pi.Flags);

	auto dependency = _node.child(PROC_CONFIG_XML_NODE_DEPENDENCY);
	_ParseDependency(dependency, _pi.Dependency);

	if (!_pi.Flags.IsSystemCommand)
	{
		string oldPath = _pi.Path;

		if (Utils::ResolveRootPath(_pi.Path))
		{
			logStream << "Process::Resolved process path to root absolute. oldPath: '" << oldPath << "', newPath: '" << _pi.Path << "'";
			Logger::LogTrace(logStream);
		}
	}

	_pi.Variables.addArg(_pi.Path.c_str());

	auto variables = _node.child(PROC_CONFIG_XML_NODE_VARIABLES);

	_ParseVariables(variables, _pi.Variables);

	return true;

}

bool ProcessConfigurationParser::_ParseShm(xml_node& _node, ShmInfo& _si)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseProcess()...");

	if (!_node)
		return false;

	stringstream logStream;

	logStream << "Shared memory configuration -> sId: '" << _si.Id << "'";
	Logger::LogTrace(logStream);

	auto id = _node.attribute(PROC_CONFIG_XML_ATTR_ID);
	_si.Id = id.as_string();

	logStream << "Shm::Id: '" << _si.Id << "'";
	Logger::LogTrace(logStream);

	auto shmId = _node.attribute(PROC_CONFIG_XML_ATTR_SHM_ID);
	_si.ShmId = shmId.as_int();

	logStream << "Shm::ShmId: '" << _si.ShmId << "'";
	Logger::LogTrace(logStream);

	auto shmPath = _node.attribute(PROC_CONFIG_XML_ATTR_SHM_PATH);
	_si.ShmPath = shmPath.as_string();

	logStream << "Shm::ShmPath: '" << _si.ShmPath << "'";
	Logger::LogTrace(logStream);

	if (_si.Id.size() == 0 || _si.ShmId == 0 || _si.ShmPath.size() == 0)
		return false;

	auto enable = _node.attribute(PROC_CONFIG_XML_ATTR_ENABLE);
	_si.Enable = enable ? enable.as_bool() : true;

	logStream << "Shm::enable: '" << _si.Enable << "'";
	Logger::LogTrace(logStream);

	auto name = _node.attribute(PROC_CONFIG_XML_ATTR_NAME);
	_si.Name = name.as_string();

	logStream << "Shm::name: '" << _si.Name << "'";
	Logger::LogTrace(logStream);

	return true;
}

bool ProcessConfigurationParser::_ParseFlags(xml_node& _node, ProcessFlags& _pi)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseFlags()...");

	if (!_node)
		return false;

	stringstream logStream;

	bool hasFlags = false;

	auto flagNodes = _node.children(PROC_CONFIG_XML_NODE_FLAG);

	for (auto flagNode : flagNodes)
	{
		auto value = flagNode.attribute(PROC_CONFIG_XML_ATTR_VALUE);
		string attribute = flagNode.attribute(PROC_CONFIG_XML_ATTR_NAME).as_string();
		if (attribute.size() > 0 && value)
		{
			auto boolValue = value.as_bool();

			hasFlags = true;

			logStream << "Parsing Flag -> name: '" << attribute << "', value: '" << boolValue << "'";
			Logger::LogTrace(logStream);

			StringUtil::ToLower(attribute);

			if (attribute == PROC_CONFIG_XML_FLAG_IS_SYSTEM_COMMAND)
			{
				_pi.IsSystemCommand = boolValue;
			}
			else if (attribute == PROC_CONFIG_XML_FLAG_IS_KILL_MULTIPLE)
			{
				_pi.IsKillMultiple = boolValue;
			}
			else if (attribute == PROC_CONFIG_XML_FLAG_IS_RESTART_ALL)
			{
				_pi.IsRestartAll = boolValue;
			}
			else if (attribute == PROC_CONFIG_XML_FLAG_IS_MONITOR)
			{
				_pi.IsMonitor = boolValue;
			}
			else if (attribute == PROC_CONFIG_XML_FLAG_IS_SHARED_PROCESS)
			{
				_pi.IsSharedProcess = boolValue;
			}
		}
	}
	return hasFlags;
}

bool ProcessConfigurationParser::_ParseDependency(xml_node& _node, ProcessDependency& _pd)
{
	bool res = false;

	Logger::LogTrace("ProcessConfigurationParser::_ParseDependency()...");

	if (!_node)
		return res;

	for (auto child : _node.children())
	{
		auto id = child.attribute(PROC_CONFIG_XML_ATTR_ID);
		string typeValue = child.name();
		PD_ENUM type = typeValue == PROC_CONFIG_XML_NODE_PD_PR ? PD_Process : (typeValue == PROC_CONFIG_XML_NODE_PD_SM ? PD_SharedMem : PD_NONE);

		if (id == NULL || type == PD_NONE)
			continue;

		_pd.Dependency.push_back({ type ,id.as_string() });
		res = true;

	}
	return res;
}

bool ProcessConfigurationParser::_ParseVariables(xml_node& _node, ProcessVariables& _pv)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseVariables()...");

	if (!_node)
		return false;

	auto args = _node.child(PROC_CONFIG_XML_NODE_ARGS);

	bool res = _ParseArgs(args, _pv);

	auto envs = _node.child(PROC_CONFIG_XML_NODE_ENVS);

	res = _ParseEnvs(envs, _pv) && res;

	return true;
}

bool ProcessConfigurationParser::_ParseArgs(xml_node& _node, ProcessVariables& _pv)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseArgs()...");

	if (!_node)
		return false;

	stringstream logStream;

	bool hasArg = false;

	auto args = _node.children(PROC_CONFIG_XML_NODE_ARG);

	for (auto argNode : args)
	{
		if (argNode.attribute(PROC_CONFIG_XML_ATTR_IGNORE).as_bool())
		{
			logStream << "Ignoring  argument -> value: '" << argNode.attribute(PROC_CONFIG_XML_ATTR_VALUE).as_string() << "', as ignore flag is set";
			Logger::LogWarn(logStream);
			continue;
		}

		string arg = argNode.attribute(PROC_CONFIG_XML_ATTR_VALUE).as_string();
		if (arg.size() > 0)
		{
			if (_pv.addArg(arg.c_str()) > 0)
			{
				logStream << "Adding argument variable -> value: '" << arg << "'";
				Logger::LogTrace(logStream);
				hasArg = true;
			}
			else
			{
				logStream << "Argument variables count limit exceeded, skipping remaining entries. MAX_ARG_COUNT : '" << MAX_ARG_COUNT << "'";
				Logger::LogWarn(logStream);
				break;
			}
		}
	}
	return hasArg;
}


bool ProcessConfigurationParser::_ParseEnvs(xml_node& _node, ProcessVariables& _pv)
{
	Logger::LogTrace("ProcessConfigurationParser::_ParseEnvs()...");

	if (!_node)
		return false;

	stringstream logStream;

	bool hasEnv = false;

	auto envs = _node.children(PROC_CONFIG_XML_NODE_ENV);

	for (auto envNode : envs)
	{
		if (envNode.attribute(PROC_CONFIG_XML_ATTR_IGNORE).as_bool())
		{
			logStream << "Ignoring  environment value -> value: '" << envNode.attribute(PROC_CONFIG_XML_ATTR_VALUE).as_string() << "', as ignore flag is set";
			Logger::LogWarn(logStream);
			continue;
		}
		string env = envNode.attribute(PROC_CONFIG_XML_ATTR_VALUE).as_string();
		if (env.size() > 0)
		{
			if (_pv.addEnv(env.c_str()) > 0)
			{
				logStream << "Adding environment variable -> value: '" << env << "'";
				Logger::LogTrace(logStream);
				hasEnv = true;
			}
			else
			{
				logStream << "Environment variables count limit exceeded, skipping remaining entries. MAX_ARG_COUNT : '" << MAX_ARG_COUNT << "'";
				Logger::LogWarn(logStream);
				break;
			}
		}
	}

	return hasEnv;
}

