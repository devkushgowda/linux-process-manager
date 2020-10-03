#ifndef _SHM_TEMPLATES_HPP_
#define _SHM_TEMPLATES_HPP_

#include<map>

#include "shmm.hpp"

using namespace std;

namespace dpm
{
	/*To register new class
	1. Define entry in SHM_OBJECT_ID_ENUM with "{class name}_ID"
	2. Define id with "SHM_{enum entry value, i.e defined in step 1}"
	3. Add SSHM_MAP({ENUM ENTRY,  i.e defined in step 2}) to _SHM_OBJECT_MAP initilization
	4. Add SSHM_CASE(CLASS,LVAL,CONFIG) in to the GetSharedMemoryProducer() -> switch case.
	*/

#define SSHM_MAP(X) {SHM_##X##_ID,X##_ID}

#define SHM_Test1_ID "test1"
#define SHM_Test2_ID "test2"

	enum SHM_OBJECT_ID_ENUM {
		SHM_OBJECT_ID_NONE = -1, Test1_ID, Test2_ID
	};

	class SharedMemoryFactory
	{
	private:
		static map<string, SHM_OBJECT_ID_ENUM> _SHM_OBJECT_MAP;
		static map<string, BaseSharedMemoryProducer*> _SHM_STORE;
	public:

		static BaseSharedMemoryProducer* GetSharedMemoryProducer(string const& _id, bool _create = true, SharedMemoryProducerConfig* _config = NULL);
		static bool IsValid(string const& _id);
		static bool Delete(string const& _id);
	};

	class Test1
	{
	public:
		int x[100];
	};

	class Test2
	{
	public:
		int x[100];
	};

}
#endif