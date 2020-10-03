#ifndef _SPD_LOGGER_HPP_
#define _SPD_LOGGER_HPP_


#define SPD_LOGGER_DEFAULT_PATTERN "[%T.%e] [%t] [%L] %v"

#include <string>
#include <mutex>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace std;
using namespace spdlog;
using namespace level;

namespace spdlog
{
	class SpdLogSettings
	{
	public:
		string LogRegistryName;
		string LogFilePathName;
		level_enum FileLogLevel = level_enum::info;
		string FileLogPattern = SPD_LOGGER_DEFAULT_PATTERN;
		bool ConsoleLogRequired = true;
		level_enum ConsoleLogLevel = level_enum::info;
		string ConsoleLogPattern = SPD_LOGGER_DEFAULT_PATTERN;
		SpdLogSettings() {}
	};

	class SpdLogger
	{
		mutex _locker;
		shared_ptr<logger> _logInstance;
		SpdLogSettings _logSetting;

	public:

		SpdLogger()
		{
		}

		SpdLogger(SpdLogSettings& _setting)
		{
			_logSetting = _setting;
			init(_setting);
		}

		bool init(SpdLogSettings& _setting)
		{
			bool res = false;

			if (_setting.LogFilePathName.size() == 0 || _setting.LogRegistryName.size() == 0 || _logInstance != NULL)
				return res;

			lock_guard<mutex> lock(_locker);
			try {
				_logSetting = _setting;
				if (_logInstance == NULL)
				{
					string dirPath = _setting.LogFilePathName;
					auto pos = dirPath.rfind('/');
					if (string::npos != pos)
					{
						dirPath.erase(pos, dirPath.size());
						CreateDirectoryTreeIfNotExists(dirPath);
					}
					auto curLogInstance = create_async<sinks::daily_file_sink_mt>(_setting.LogRegistryName, _setting.LogFilePathName, 0, 0, false);
					res = true;
					curLogInstance->set_level(_setting.FileLogLevel);
					curLogInstance->set_pattern(_setting.FileLogPattern);
					curLogInstance->flush_on(level_enum::trace);

					if (_setting.ConsoleLogRequired)
					{
						auto consoleSink = make_shared<sinks::stdout_color_sink_st>();
						consoleSink->set_level(_setting.ConsoleLogLevel);
						consoleSink->set_pattern(_setting.ConsoleLogPattern);
						curLogInstance->sinks().push_back(consoleSink);
					}
					_logInstance = curLogInstance;
				}
			}
			catch (...)
			{
				//Logging module should not crash the main application.
			}
			return res;
		}

		string getLogFilePathName() const
		{
			return _logSetting.LogFilePathName;
		}

		string getLogPattern() const
		{
			return _logSetting.FileLogPattern;
		}

		bool ready() const
		{
			return _logInstance != NULL;
		}

		bool changeLogLevel(level_enum _level)
		{
			if (_logInstance == NULL)
				return false;

			lock_guard<mutex> lock(_locker);
			bool res = false;
			if (_logInstance != NULL)
			{
				for (auto const& sinks : _logInstance->sinks())
				{
					sinks->set_level(_level);
				}
				res = true;
			}
			return res;
		}

		template<typename ...Args>
		inline void trace(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->trace(fmt, args...);
		}

		template<typename ...Args>
		inline void error(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->error(fmt, args...);
		}

		template<typename ...Args>
		inline void info(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->info(fmt, args...);
		}

		template<typename ...Args>
		inline void warn(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->warn(fmt, args...);
		}

		template<typename ...Args>
		inline void debug(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->debug(fmt, args...);
		}

		template<typename ...Args>
		inline void critical(string_view_t fmt, const Args& ...args)
		{
			if (_logInstance != NULL)
				_logInstance->critical(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_trace(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->trace(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_debug(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->debug(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_info(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->info(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_warn(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->warn(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_error(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->error(fmt, args...);
		}

		template<typename ...Args>
		static inline void log_critical(string _registryName, string_view_t fmt, const Args& ...args)
		{
			auto logger = get(_registryName);
			if (logger != NULL)
				logger->critical(fmt, args...);
		}

		~SpdLogger()
		{
		}

		static bool CreateDirectoryTreeIfNotExists(string const& _dirPath)
		{
			bool res = true;
			string dirPath = _dirPath;
			size_t pos = 0;
			string dir;

			if (dirPath[dirPath.size() - 1] != '/') {
				// force trailing / so we can handle everything in loop
				dirPath += '/';
			}

			while ((pos = dirPath.find_first_of('/', pos)) != string::npos) {
				dir = dirPath.substr(0, pos++);
				if (dir.size() == 0) continue; // if leading / first time is 0 length
				if (mkdir(dir.c_str(), 0755) && errno != EEXIST) {
					res = false;
					break;
				}
			}
			return res;
		}

	};
}
#endif
