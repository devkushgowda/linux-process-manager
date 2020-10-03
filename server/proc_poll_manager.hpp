#ifndef _PROC_POLL_MANAGER_HPP_
#define _PROC_POLL_MANAGER_HPP_

#include "proc_def.hpp"
#include "global_lock.hpp"
#include "../common/common_def.h"
#include "../common/base_poll_manager.cpp"
#include "server_config.hpp"

using namespace std;

namespace dpm
{

	class ProcessPollManager : public BasePollManager<ProcessInfo>
	{

	public:
		ProcessPollManager();

		~ProcessPollManager();

		void add(ProcessInfo& _pInfo, function<void(ProcessInfo&)> _pollFunc);

		bool remove(ProcessInfo& _pInfo);

	};
}
#endif