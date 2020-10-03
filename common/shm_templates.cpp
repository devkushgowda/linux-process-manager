#include "shm_templates.hpp"

using namespace dpm;

map<string, SHM_OBJECT_ID_ENUM> SharedMemoryFactory::_SHM_OBJECT_MAP = {
	SSHM_MAP(Test1),
	SSHM_MAP(Test2),
};

map<string, BaseSharedMemoryProducer*> SharedMemoryFactory::_SHM_STORE;

BaseSharedMemoryProducer* SharedMemoryFactory::GetSharedMemoryProducer(string const& _id, bool _create, SharedMemoryProducerConfig* _config)
{
	BaseSharedMemoryProducer* res = NULL;
	auto entry = _SHM_OBJECT_MAP.find(_id);

	if (entry != _SHM_OBJECT_MAP.end())
	{
		auto tempRes = _SHM_STORE.find(_id);
		if (tempRes == _SHM_STORE.end() && _create)
		{
			bool created = true;
			switch (entry->second)
			{
			case Test1_ID:
			{
				res = new SharedMemoryProducer< Test1>(_config->ID, _config->Destroy, _config->PathName);
				break;
			}
			case Test2_ID:
			{
				res = new SharedMemoryProducer< Test2>(_config->ID, _config->Destroy, _config->PathName);
				break;
			}
			default:
				created = false;
			}
			if (created)
				_SHM_STORE.insert({ _id ,res });
		}
		else
		{
			res = tempRes->second;
		}
	}
	return res;
}

bool SharedMemoryFactory::IsValid(string const& _id)
{
	return _SHM_OBJECT_MAP.find(_id) != _SHM_OBJECT_MAP.end();
}

bool SharedMemoryFactory::Delete(string const& _id)
{
	bool res = false;
	auto entry = _SHM_STORE.find(_id);
	if (entry != _SHM_STORE.end())
	{
		res = true;
		delete entry->second;
		_SHM_STORE.erase(entry);
	}
	return res;
}


