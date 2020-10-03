#ifndef _EXCEPTION_DEF_HPP_
#define _EXCEPTION_DEF_HPP_

#include "logger.hpp"

using namespace std;
using namespace dpm;

#define DPM_TRY try

#define DPM_CATCH_LOG(funcName)\
catch (exception const& ex)\
{\
Logger::LogExcept(funcName,ex);\
}\
catch (...)\
{\
auto p = current_exception();\
Logger::LogExcept(funcName,(p ? p.__cxa_exception_type()->name() : "null"));\
}

#define DPM_CATCH_IGNORE()\
catch (...)\
{\
}

#endif