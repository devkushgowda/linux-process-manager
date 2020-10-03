#ifndef _SHMM_HPP_
#define _SHMM_HPP_

#define ENABLE_LOG_TRACE 1

#define DEFAULT_SHM_PATH "/root"

#if ENABLE_LOG_TRACE
#include  "logger.hpp"
#endif

#include  <sys/types.h>
#include  <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

#include  <string>
#include  <sstream>
#include  <iostream>
#include <ctime>

using namespace std;

namespace dpm
{
	template<typename T>
	class ConsumerSharedMemoryData
	{
		typedef  T SmDataType;
	protected:
		bool _waitInit;
	};

	template<typename T>
	class ProducerSharedMemoryData
	{
		typedef  T SmDataType;
	protected:
		time_t _lastWriteTime;
		bool _destroy;
	public:
		time_t const& LastWriteTime = _lastWriteTime;
	};


	template<typename T>
	class SharedMemoryData
	{
		typedef  T SmDataType;

	protected:
		bool _created = false;
		int _id;
		string _pathName;
		key_t _shmKey;
		int _shmId;
		SmDataType* _shmDataPtr;

	public:

		SmDataType Data = Data;
		const SmDataType* const& SharedDataPtr = _shmDataPtr;

		size_t size()const
		{
			return sizeof(SmDataType);
		}
	};

	class SharedMemoryHelper
	{
	public:
		static sem_t* CreateSharedSemaphore(string const& _id, bool _override = false)
		{
#if ENABLE_LOG_TRACE
			stringstream ss;
			ss << "CreateSharedSemaphore()-> _id: '" << _id << "', _override: '" << _override << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			if (_override)
				RemoveSharedSemaphore(_id);
			auto res = sem_open(_id.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

#if ENABLE_LOG_TRACE
			ss << "CreateSharedSemaphore()-> res: '" << res << "' ";
			Logger::LogTrace(ss);
#endif // ENABLE_LOG_TRACE

			return res;
		}

		static int RemoveSharedSemaphore(string const& _id)
		{
#if ENABLE_LOG_TRACE
			stringstream ss;
			ss << "RemoveSharedSemaphore()-> _id: '" << _id << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = sem_unlink(_id.c_str());

#if ENABLE_LOG_TRACE
			ss << "RemoveSharedSemaphore()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE
			return res;
		}

		static sem_t* AttachSharedSemaphore(string const& _id)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "AttachSharedSemaphore()-> _id: '" << _id << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = sem_open(_id.c_str(), 0, 0);
#if ENABLE_LOG_TRACE

			ss << "AttachSharedSemaphore()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE
			return res;

		}


		static key_t GetSharedKey(string const& _path, int const& _projId)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "GetSharedKey()-> _path: '" << _path << "', _projId: '" << _projId << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = ftok(_path.c_str(), _projId);
#if ENABLE_LOG_TRACE

			ss << "GetSharedKey()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}


		static int SharedMemoryExists(key_t const& _key)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "SharedMemoryExists()-> _key: '" << _key << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = shmget(_key, 0, 0);
#if ENABLE_LOG_TRACE

			ss << "SharedMemoryExists()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}

		static int CreateSharedMemory(key_t const& _key, size_t _size, bool _override = false, int _permission = 0666)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "CreateSharedMemory()-> _key: '" << _key << "', _size: '" << _size << "', _override: '" << _override << "', _permission: '" << _permission << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			if (_override)
			{
				auto id = SharedMemoryExists(_key);
				if (-1 != id)
					RemoveSharedMemory(id);
			}
			auto res = shmget(_key, _size, IPC_CREAT | _permission);
#if ENABLE_LOG_TRACE

			ss << "CreateSharedMemory()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}

		static int GetSharedMemory(key_t const& _key, size_t _size, int _permission = 0666)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "GetSharedMemory()-> _key: '" << _key << "', _size: '" << _size << "', _permission: '" << _permission << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = shmget(_key, _size, _permission);
#if ENABLE_LOG_TRACE

			ss << "GetSharedMemory()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}

		template<typename T>
		static T* AttachSharedMemory(const int _id)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "AttachSharedMemory()-> _id: '" << _id << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = shmat(_id, NULL, 0);
#if ENABLE_LOG_TRACE

			ss << "AttachSharedMemory()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return (T*)res;
		}

		static int DetachSharedMemory(void const* _address)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "DetachSharedMemory()-> _address: '" << _address << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = shmdt(_address);
#if ENABLE_LOG_TRACE

			ss << "DetachSharedMemory()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}

		static int RemoveSharedMemory(const int _id)
		{
#if ENABLE_LOG_TRACE

			stringstream ss;
			ss << "RemoveSharedMemory()-> _id: '" << _id << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			auto res = shmctl(_id, IPC_RMID, NULL);
#if ENABLE_LOG_TRACE

			ss << "RemoveSharedMemory()-> res: '" << res << "' ";
			Logger::LogTrace(ss);

#endif // ENABLE_LOG_TRACE

			return res;
		}
	};

	class BaseSharedMemoryProducer
	{
		virtual bool create(bool _overide = true) = 0;
		virtual bool destroy() = 0;
		virtual bool write() = 0;
	};

	class SharedMemoryProducerConfig
	{
	public:
		int ID;
		bool Destroy = true;
		string PathName = DEFAULT_SHM_PATH;
	};

	template<typename T>
	class SharedMemoryProducer :public BaseSharedMemoryProducer, public SharedMemoryData<T>, public ProducerSharedMemoryData<T>
	{
	public:
		SharedMemoryProducer(int __id, bool __destroy = true, string __pathName = DEFAULT_SHM_PATH)
		{
			this->_destroy = __destroy;
			this->_shmKey = SharedMemoryHelper::GetSharedKey(__pathName, __id);
			this->_id = __id;
			this->_pathName = __pathName;
		}
		virtual ~SharedMemoryProducer()
		{
			if (this->_destroy)
				this->destroy();
		}

		bool create(bool _overide = true) override {
			bool res = false;
			if (!this->_created)
			{
				this->_shmId = SharedMemoryHelper::CreateSharedMemory(this->_shmKey, this->size(), true);
				res = -1 != this->_shmId;
				if (res)
				{
					this->_shmDataPtr = SharedMemoryHelper::AttachSharedMemory<T>(this->_shmId);
					res = (void*)-1 != this->_shmDataPtr;
					if (res)
					{
						*(this->_shmDataPtr) = this->Data;
						time(&this->_lastWriteTime);
					}
				}
			}
			this->_created = res;
			return res;
		}

		bool destroy() override {
			bool res = false;
			if (this->_created)
			{
				SharedMemoryHelper::DetachSharedMemory(this->_shmDataPtr);
				SharedMemoryHelper::RemoveSharedMemory(this->_shmId);
				this->_created = false;
				res = true;
			}
			return res;
		}

		bool write() override {
			bool res = false;
			if (this->_created)
			{
				*(this->_shmDataPtr) = this->Data;
				time(&this->_lastWriteTime);
			}
			return res;
		}
	};

	class BaseSharedMemoryConsumer
	{
		virtual bool init() = 0;
		virtual bool destroy() = 0;
		virtual bool update() = 0;
	};

	template<typename T>
	class SharedMemoryConsumer :public BaseSharedMemoryConsumer, public SharedMemoryData<T>, public ConsumerSharedMemoryData<T>
	{

	public:
		SharedMemoryConsumer(int __id, bool __waitInit = true, string __pathName = DEFAULT_SHM_PATH)
		{
			this->_waitInit = __waitInit;
			this->_shmKey = SharedMemoryHelper::GetSharedKey(__pathName, __id);
			this->_id = __id;
			this->_pathName = __pathName;
		}

		virtual ~SharedMemoryConsumer()
		{
			destroy();
		}

		bool destroy() override
		{
			bool res = false;
			if (this->_created)
			{
				SharedMemoryHelper::DetachSharedMemory(this->_shmDataPtr);
				this->_created = false;
				res = true;
			}
			return res;
		}

		bool init() override {
			bool res = false;

			for (;;)
			{
				this->_shmId = SharedMemoryHelper::GetSharedMemory(this->_shmKey, this->size());
				res = -1 != this->_shmId;
				if (!this->_waitInit)
					break;
				if (res)
				{
					this->_shmDataPtr = SharedMemoryHelper::AttachSharedMemory<T>(this->_shmId);
					res = (void*)-1 != this->_shmDataPtr;
					if (res)
						this->Data = *(this->_shmDataPtr);
					break;
				}
			}
			this->_created = res;
			return res;
		}

		bool update() override {
			bool res = false;
			if (this->_created)
			{
				this->Data = *(this->_shmDataPtr);
			}
			return res;
		}
	};
}
#endif