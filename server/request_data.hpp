#ifndef _REQUEST_DATA_HPP_
#define _REQUEST_DATA_HPP_

#include <string>
#include <sstream>
#include <thread>

#include "../common/PDU.hpp"
#include "../common/common_def.h"
#include "../common/err_def.hpp"
#include "../common/socket_manager.hpp"

using namespace std;

namespace dpm
{

	class BaseRequestData
	{
	public:
		virtual bool SendResponseStream(stringstream& _stream, const OUTPUT_ENUM _type) = 0;
		virtual bool FlushPDU() = 0;
		virtual bool ReceiveRequestData() = 0;
		ERR_ENUM Error;
	};

	class DummyRequestData :public BaseRequestData
	{
	public:
		bool SendResponseStream(stringstream& _stream, const OUTPUT_ENUM _type);
		bool FlushPDU();
		bool ReceiveRequestData();
	};

	class RequestData :public BaseRequestData
	{
	private:
		int _SockFD;

	public:
		SocketHelper* SockHelper;
		char RequestBuf[MAX_PDU_BUFFER_LENGTH];
		char ResponseBuf[MAX_PDU_BUFFER_LENGTH];
		int RequestBufLen, ResponseBufLen;
		PDU RequestPDU, ResponsePDU;


		RequestData(int _sockFD);
		~RequestData();

		bool SendResponseStream(stringstream& _stream, const OUTPUT_ENUM _type);
		bool FlushPDU();
		bool ReceiveRequestData();

	};
}
#endif