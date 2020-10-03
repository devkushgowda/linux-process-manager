#ifndef _LOGGER_HPP
#define _LOGGER_HPP


#define LOG_REGISTRY_NAME "DPM"

#include <iostream>
#include <sstream>
#include <atomic>
#include "utils.hpp"

#ifdef LOG_FILE
#include "spd_logger.hpp"
using namespace spdlog;
#endif

using namespace std;

namespace dpm
{
#define LOG_INVALID_STRING "LOG_INVALID"
#define LOG_DEBUG_STRING "LOG_DEBUG"
#define LOG_TRACE_STRING "LOG_TRACE"
#define LOG_INFO_STRING "LOG_INFO"
#define LOG_WARN_STRING "LOG_WARN"
#define LOG_ERROR_STRING "LOG_ERROR"

	enum LOG_MODE
	{
		LOG_INVALID = -1,
		LOG_DEBUG = 1,
		LOG_TRACE,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR
	};

	enum OUTPUT_ENUM {
		OUTPUT_HEAD, OUTPUT_SUBHEAD, OUTPUT_ERROR, OUTPUT_WARN, OUTPUT_INFO
	};

	enum POLL_ENUM {
		POLL_HEAD, POLL_ERROR, POLL_WARN, POLL_SUCCESS
	};

	class Logger
	{
	private:
#ifdef LOG_FILE
		static SpdLogger _Logger;
#endif
		static atomic<bool> _DEBUG, _ERROR, _WARN, _TRACE, _INFO;
		static atomic<LOG_MODE> _CurrentMode;
	public:
		static void InitLogger(LOG_MODE _mode, string _logFile = "", bool _logToFile = false);
		static LOG_MODE ToLogMode(string _str);

		static void LogPoll(string _msg, POLL_ENUM _type);
		static void LogPoll(stringstream& _msgSs, POLL_ENUM _type);

		static void LogTrace(string _msg);
		static void LogInfo(string _msg);
		static void LogDebug(string _msg);
		static void LogWarn(string _msg);
		static void LogError(string _msg);
		static void LogExcept(string _functName, string _msg);
		static void LogExcept(string _functName, exception const& _except);

		static void LogTrace(stringstream& _ss);
		static void LogDebug(stringstream& _ss);
		static void LogInfo(stringstream& _ss);
		static void LogWarn(stringstream& _ss);
		static void LogError(stringstream& _ss);
		static void LogExcept(string _functName, stringstream& _ss);

		static void OpHead(stringstream& _ss, string _s);
		static void OpSubhead(stringstream& _ss, string _s);
		static void OpError(stringstream& _ss, string _s);
		static void OpWarn(stringstream& _ss, string _s);
		static void OpInfo(stringstream& _ss, string _s);
		static void OpResult(string _ss);

		static void OpHead(stringstream& _opSs, stringstream& _ss);
		static void OpSubhead(stringstream& _opSs, stringstream& _ss);
		static void OpError(stringstream& _opSs, stringstream& _ss);
		static void OpWarn(stringstream& _opSs, stringstream& _ss);
		static void OpInfo(stringstream& _opSs, stringstream& _ss);
		static void OpResult(stringstream& _ss);

		static LOG_MODE GetLogMode();

		static string GetLogModeString();
	};
}
#endif