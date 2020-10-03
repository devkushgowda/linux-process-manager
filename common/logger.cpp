#include "logger.hpp"

using namespace dpm;

#ifdef LOG_FILE
SpdLogger Logger::_Logger;
#endif

atomic<bool> Logger::_DEBUG(false);
atomic<bool> Logger::_TRACE(false);
atomic<bool> Logger::_INFO(false);
atomic<bool> Logger::_ERROR(true);
atomic<bool> Logger::_WARN(true);

atomic<LOG_MODE> Logger::_CurrentMode(LOG_ERROR);

void Logger::InitLogger(LOG_MODE _mode, string _logFile, bool _logToFile)
{
	if (_mode == LOG_INVALID)
		return;


	_DEBUG.store(false, memory_order_relaxed);
	_TRACE.store(false, memory_order_relaxed);
	_INFO.store(false, memory_order_relaxed);
	_WARN.store(false, memory_order_relaxed);
	_ERROR.store(false, memory_order_relaxed);

	switch (_mode)
	{
	case LOG_TRACE:_TRACE.store(true, memory_order_relaxed);
	case LOG_DEBUG:_DEBUG.store(true, memory_order_relaxed);
	case LOG_INFO:_INFO.store(true, memory_order_relaxed);
	case LOG_WARN:_WARN.store(true, memory_order_relaxed);
	case LOG_ERROR:_ERROR.store(true, memory_order_relaxed);
		_CurrentMode.store(_mode, memory_order_relaxed);
		break;
	default:
		break;
	}
#ifdef LOG_FILE
	//if (_logToFile)
	//{
	//	level_enum spdLogLevel;
	//	switch (_mode)
	//	{
	//	case LOG_DEBUG:
	//		spdLogLevel = level_enum::debug;
	//		break;
	//	case LOG_TRACE:
	//		spdLogLevel = level_enum::trace;
	//		break;
	//	case LOG_INFO:
	//		spdLogLevel = level_enum::info;
	//		break;
	//	case LOG_WARN:
	//		spdLogLevel = level_enum::warn;
	//		break;
	//	case LOG_ERROR:
	//		spdLogLevel = level_enum::err;
	//		break;
	//	default:
	//		break;
	//	}
	//	if (!_Logger.ready())
	//	{
	//		SpdLogSettings setting;
	//		setting.LogRegistryName = LOG_REGISTRY_NAME;
	//		setting.LogFilePathName = _logFile;
	//		setting.FileLogLevel = spdLogLevel;
	//		setting.ConsoleLogRequired = false;
	//		_Logger.init(setting);
	//	}
	//	else
	//	{
	//		_Logger.changeLogLevel(spdLogLevel);
	//	}
	//}
#endif

}

LOG_MODE Logger::ToLogMode(string _str)
{
	LOG_MODE res = LOG_INVALID;
	StringUtil::ToUpper(_str);
	if (LOG_DEBUG_STRING == _str)
	{
		res = LOG_DEBUG;
	}
	else if (LOG_TRACE_STRING == _str)
	{
		res = LOG_TRACE;
	}
	else if (LOG_INFO_STRING == _str)
	{
		res = LOG_INFO;
	}
	else if (LOG_WARN_STRING == _str)
	{
		res = LOG_WARN;
	}
	else if (LOG_ERROR_STRING == _str)
	{
		res = LOG_ERROR;
	}
	return res;
}

void Logger::LogPoll(string _msg, POLL_ENUM _type)
{
	string log = "POLL:: " + _msg;
	switch (_type)
	{
	case POLL_HEAD:
		cout << endl << ConsoleUtil::Green(log) << endl;
		break;
	case POLL_ERROR:
		cout << endl << ConsoleUtil::Red(log) << endl;
		break;
	case POLL_WARN:
		cout << endl << ConsoleUtil::Yellow(log) << endl;
		break;
	case POLL_SUCCESS:
		cout << endl << ConsoleUtil::Magenta(log) << endl;
		break;
	default:
		break;
	}
}

void Logger::LogPoll(stringstream& _msgSs, POLL_ENUM _type)
{
	string log = "POLL:: " + _msgSs.str();
	switch (_type)
	{
	case POLL_HEAD:
		cout << endl << ConsoleUtil::Green(log) << endl;
		break;
	case POLL_ERROR:
		cout << endl << ConsoleUtil::Red(log) << endl;
		break;
	case POLL_WARN:
		cout << endl << ConsoleUtil::Yellow(log) << endl;
		break;
	case POLL_SUCCESS:
		cout << endl << ConsoleUtil::Magenta(log) << endl;
		break;
	default:
		break;
	}
	_msgSs.str("");
}

void Logger::LogInfo(string _msg)
{
#ifdef LOG_FILE
	_Logger.info(_msg);
#endif
	if (_DEBUG.load(memory_order_relaxed) || _INFO.load(memory_order_relaxed))
	{
		string log = "INFO:: " + _msg;
		cout << endl << ConsoleUtil::Blue(log) << endl;
	}
}

void Logger::LogDebug(string _msg)
{
#ifdef LOG_FILE
	_Logger.debug(_msg);
#endif
	if (_DEBUG.load(memory_order_relaxed) || _INFO.load(memory_order_relaxed))
	{
		string log = "DEBUG:: " + _msg;
		cout << endl << log << endl;
	}
}

void Logger::LogTrace(string _msg)
{
#ifdef LOG_FILE
	_Logger.trace(_msg);
#endif
	if (_DEBUG.load(memory_order_relaxed) || _TRACE.load(memory_order_relaxed))
	{
		string log = "TRACE:: " + _msg;
		cout << endl << ConsoleUtil::White(log) << endl;
	}
}

void Logger::LogWarn(string _msg)
{
#ifdef LOG_FILE
	_Logger.warn(_msg);
#endif
	if (_DEBUG.load(memory_order_relaxed) || _WARN.load(memory_order_relaxed))
	{
		string log = "WARN:: " + _msg;
		cout << endl << ConsoleUtil::Yellow(log) << endl;
	}
}

void Logger::LogError(string _msg)
{
#ifdef LOG_FILE
	_Logger.error(_msg);
#endif
	if (_DEBUG.load(memory_order_relaxed) || _ERROR.load(memory_order_relaxed))
	{
		string log = "ERROR:: " + _msg;
		cout << endl << ConsoleUtil::Red(log) << endl;
	}
}

void dpm::Logger::LogExcept(string _functName, string _msg)
{
	const string exp = "EXCEPTION:: func()-> ";
#ifdef LOG_FILE
	_Logger.critical(exp + _functName + _msg);
#endif
	string log = exp + _functName + _msg;
	cout << endl << ConsoleUtil::Red(log) << endl;
}

void dpm::Logger::LogExcept(string _functName, exception const& _except)
{
	const string exp = "EXCEPTION:: func()-> ";
#ifdef LOG_FILE
	_Logger.critical(exp + _functName + _except.what());
#endif
	string log = exp + _functName + _except.what();
	cout << endl << ConsoleUtil::Red(log) << endl;
}

void Logger::LogTrace(stringstream& _ss)
{
	LogTrace(_ss.str());
	_ss.str("");
}

void Logger::LogDebug(stringstream& _ss)
{
	LogDebug(_ss.str());
	_ss.str("");
}

void Logger::LogInfo(stringstream& _ss)
{
	LogInfo(_ss.str());
	_ss.str("");
}

void Logger::LogWarn(stringstream& _ss)
{
	LogWarn(_ss.str());
	_ss.str("");
}

void Logger::LogError(stringstream& _ss)
{
	LogError(_ss.str());
	_ss.str("");
}

void dpm::Logger::LogExcept(string _functName, stringstream& _ss)
{
	LogExcept(_functName, _ss.str());
	_ss.str("");
}

void Logger::OpResult(string _msg)
{
	cout << endl << _msg << endl;
}

void Logger::OpError(stringstream& _ss, string _s)
{
	_ss << ConsoleUtil::Red(_s) << endl;
}

void Logger::OpHead(stringstream& _ss, string _s)
{
	_ss << ConsoleUtil::Green(_s) << endl;
}

void Logger::OpSubhead(stringstream& _ss, string _s)
{
	_ss << ConsoleUtil::Magenta(_s) << endl;

}

void Logger::OpWarn(stringstream& _ss, string _s)
{
	_ss << ConsoleUtil::Yellow(_s) << endl;
}

void Logger::OpInfo(stringstream& _ss, string _s)
{
	_ss << ConsoleUtil::Blue(_s) << endl;
}

void Logger::OpHead(stringstream& _opSs, stringstream& _ss)
{
	OpHead(_opSs, _ss.str());
	_ss.str("");
}

void Logger::OpSubhead(stringstream& _opSs, stringstream& _ss)
{
	OpSubhead(_opSs, _ss.str());
	_ss.str("");
}

void Logger::OpError(stringstream& _opSs, stringstream& _ss)
{
	OpError(_opSs, _ss.str());
	_ss.str("");
}

void Logger::OpWarn(stringstream& _opSs, stringstream& _ss)
{
	OpWarn(_opSs, _ss.str());
	_ss.str("");
}

void Logger::OpInfo(stringstream& _opSs, stringstream& _ss)
{
	OpInfo(_opSs, _ss.str());
	_ss.str("");
}

void Logger::OpResult(stringstream& _ss)
{
	OpResult(_ss.str());
	_ss.str("");
}

LOG_MODE Logger::GetLogMode()
{
	return _CurrentMode.load(memory_order_relaxed);
}

string Logger::GetLogModeString()
{
	switch (_CurrentMode.load(memory_order_relaxed))
	{
	case LOG_DEBUG:
		return LOG_DEBUG_STRING;
	case LOG_TRACE:
		return LOG_TRACE_STRING;
	case LOG_INFO:
		return LOG_INFO_STRING;
	case LOG_WARN:
		return LOG_WARN_STRING;
	case LOG_ERROR:
		return LOG_ERROR_STRING;
	default:
		return LOG_INVALID_STRING;
	}
}
