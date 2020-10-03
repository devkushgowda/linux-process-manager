
#ifndef _PDU_HPP_
#define _PDU_HPP_


#include <string.h>
#include <vector>
#include "cmd_def.hpp"
#include "sys_variant.hpp"

using namespace std;
namespace dpm
{
	class PDU
	{
	private:
		// Send PDU member variables
		unsigned short _functionCode;
		vector<SysVar> _parameters;
		vector<SysVar>::iterator _paramItr;

	public:
		PDU();
		PDU(char* _buf, int _buflen);
		~PDU();
		// Send PDU functions
		void addParameter(SysVar _param);
		void setFunctionCode(short _fc);
		CMD_ENUM getFunctionCode();
		int getParametersCount();
		SysVar getPrameterAt(int i);
		void makePDU(char* _buf, int _buflen, int& _retlen);
		void clear();

		// Receive PDU parser functions
		static void FillPDU(char* _buf, int _buflen, PDU& _pdu);
		static void GetFunctionCode(char* _buf, int _buflen, short& _ret);
		static void GetNoOfParameter(char* _buf, int _buflen, short& _ret);
		static void GetParameter(char* _buf, int _buflen, short _parano, SysVar& _ret);
	};
}
#endif