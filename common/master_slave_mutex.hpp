#ifndef _MASTER_SLAVE_MUTEX_HPP_
#define _MASTER_SLAVE_MUTEX_HPP_

#include<atomic>
#include<mutex>
using namespace std;

namespace dpm
{
	class MasterSlaveMutex {
	private:
		atomic<bool> _isLocked;
		atomic<bool> _isPrimary;
		atomic<bool> _isSecondary;
		atomic<bool> _isPrimaryWaiting;
		mutex _Mtx;
	public:

		MasterSlaveMutex();

		bool lockPrimary();

		bool lockSecondary();

		void unlockSecondary();

		void unlockPrimary();

		bool isPrimaryWaiting();

		bool isLocked();
	};
}

#endif