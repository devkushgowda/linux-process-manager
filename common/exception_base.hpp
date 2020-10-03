#ifndef _EXCEPTION_BASE_HPP_
#define _EXCEPTION_BASE_HPP_


#include <string>

using namespace std;

namespace dpm
{

	class ExceptionBase
	{
	private:
		int lineNo;
		string expFile;
		string expReason;

	public:
		ExceptionBase(string _reason, string _file, int _no = -1);
		int lineNumber() const;
		const char* fileName() const;
		const char* reason() const;
	};
}


#endif