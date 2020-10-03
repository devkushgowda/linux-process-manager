#ifndef __BASE_POLL_MANAGER_CPP_
#define __BASE_POLL_MANAGER_CPP_

#include <thread>
#include <atomic>
#include <list>
#include <mutex>
#include <utility>
#include <functional>
#include "master_slave_mutex.hpp"
#include "exception_def.hpp"

#define DEFAULT_POLL_INTERVAL 1000

using namespace std;
namespace dpm
{
	template<typename T>
	class BasePollManager
	{
	protected:
		MasterSlaveMutex* _MsLock;
		atomic<int>* _Interval;
		const int _DefaultInterval;
		atomic<bool> _Execute;
		thread _Thread;
		mutex _Locker;
		list<pair<T, function<void(T&)>>> _TasksList;

		void _poll()
		{
			if (_Execute.load(memory_order_acquire))
			{
				if (_MsLock != NULL)
					if (!_MsLock->lockSecondary())
						return;
				DPM_TRY
				{
					for (auto& func : _TasksList)
					{
						//If master thread is waiting  exit and give chance to it.
						if (_MsLock != NULL)
							if (_MsLock->isPrimaryWaiting())
								break;
						func.second(func.first);
					}
				}
				DPM_CATCH_LOG("BasePollManager::_poll()"); //Code should be in safe block as the lock will be held, hence generic catch.
				if (_MsLock != NULL)
					_MsLock->unlockSecondary();
			}
		}

		void _stop()
		{
			_Execute.store(false, memory_order_release);
		}

		void _start()
		{
			_Execute.store(true, memory_order_release);
		}


		void _init()
		{
			_Thread = thread([this]()
				{
					for (;;)
					{
						_poll();
						this_thread::sleep_for(chrono::milliseconds(_Interval ? _Interval->load(memory_order_relaxed) : _DefaultInterval));
					}
				});
		}

	public:
		BasePollManager(atomic<int>* _interval, MasterSlaveMutex* _mSLock = NULL, int _defaultInterval = DEFAULT_POLL_INTERVAL) :_MsLock(_mSLock), _Interval(_interval), _DefaultInterval(_defaultInterval), _Execute(false)
		{
			_init();
		}

		virtual ~BasePollManager()
		{
			if (_Execute.load(memory_order_acquire))
			{
				_stop();
			};
			if (_Thread.joinable())
				_Thread.join();
		}

		int clear()
		{
			lock_guard<mutex> lock(_Locker);
			_stop();
			int count = (int)_TasksList.size();
			_TasksList.clear();
			return count;
		}

		bool isRunning() const noexcept
		{
			return (_Execute.load(memory_order_acquire) && _Thread.joinable());
		}

		int getInterval() const
		{
			return _Interval ? _Interval->load(memory_order_relaxed) : _DefaultInterval;
		}

		virtual void add(T& _pInfo, function<void(T&)> _pollFunc) = 0;

		virtual bool remove(T& _pInfo) = 0;

	};

}

#endif