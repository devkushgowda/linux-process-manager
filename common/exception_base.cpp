#include "exception_base.hpp"

using namespace dpm;

ExceptionBase::ExceptionBase(string _reason, string _file, int _no) : lineNo(_no), expFile(_file), expReason(_reason)
{

}

const char* ExceptionBase::reason() const
{
	return expReason.c_str();
}

const char* ExceptionBase::fileName() const
{
	return expFile.c_str();
}

int ExceptionBase::lineNumber() const
{
	return lineNo;
}
