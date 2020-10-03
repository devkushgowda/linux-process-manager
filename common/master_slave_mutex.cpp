#include "master_slave_mutex.hpp"

using namespace dpm;

MasterSlaveMutex::MasterSlaveMutex() :_isLocked(false), _isPrimary(false), _isSecondary(false), _isPrimaryWaiting(false)
{
}

bool MasterSlaveMutex::lockPrimary()
{
	bool res = false;
	if (!_isPrimary.load(memory_order_acquire))
	{
		_isPrimaryWaiting.store(true, memory_order_release);
		_Mtx.lock();
		_isLocked.store(true, memory_order_release);
		_isPrimaryWaiting.store(false, memory_order_release);
		_isPrimary.store(true, memory_order_release);
		res = true;
	}
	return res;
}

bool MasterSlaveMutex::lockSecondary()
{
	bool res = false;

	if (!_isPrimaryWaiting.load(memory_order_acquire) && !_isLocked.load(memory_order_acquire))
	{
		_isLocked.store(true, memory_order_release);
		_isSecondary.store(true, memory_order_release);
		_Mtx.lock();
		res = true;
	}
	return res;
}

void MasterSlaveMutex::unlockSecondary()
{
	_Mtx.unlock();
	_isLocked.store(false, memory_order_release);
	_isSecondary.store(false, memory_order_release);
}

void MasterSlaveMutex::unlockPrimary()
{
	_Mtx.unlock();
	_isLocked.store(false, memory_order_release);
	_isPrimary.store(false, memory_order_release);
}

bool MasterSlaveMutex::isPrimaryWaiting()
{
	return _isPrimaryWaiting.load(memory_order_acquire);
}

bool MasterSlaveMutex::isLocked()
{
	return _isLocked.load(memory_order_acquire);
}
