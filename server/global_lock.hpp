#ifndef _GLOBAL_LOCK_HPP_
#define _GLOBAL_LOCK_HPP_

#include "../common/master_slave_mutex.hpp"

using namespace std;

namespace dpm
{
	class GlobalMutex
	{
	public:
		static MasterSlaveMutex ProcConfigLock;
	};
}

#endif