#include "request_data.hpp"
using namespace dpm;


using namespace dpm;

RequestData::RequestData(int _sockFD) : _SockFD(_sockFD)
{
	SockHelper = new SocketHelper(_SockFD);
}

RequestData::~RequestData()
{
	delete SockHelper;
}

bool RequestData::SendResponseStream(stringstream& _stream, const OUTPUT_ENUM _type)
{
	if (_stream.str().length() == 0)
		return false;

	Error = SUCCESS;
	const string op = "::OUT::";
	stringstream responseStream;

	switch (_type)
	{
	case OUTPUT_HEAD:
		Logger::LogInfo(op + _stream.str());
		Logger::OpHead(responseStream, _stream);
		break;
	case OUTPUT_SUBHEAD:
		Logger::LogInfo(op + _stream.str());
		Logger::OpSubhead(responseStream, _stream);
		break;
	case OUTPUT_ERROR:
		Logger::LogError(op + _stream.str());
		Logger::OpError(responseStream, _stream);
		break;
	case OUTPUT_WARN:
		Logger::LogWarn(op + _stream.str());
		Logger::OpWarn(responseStream, _stream);
		break;
	case OUTPUT_INFO:
		Logger::LogInfo(op + _stream.str());
		Logger::OpInfo(responseStream, _stream);
		break;
	default:
		Logger::LogTrace(op + _stream.str());
		responseStream << _stream.str();
		break;
	}

	_stream.str("");

	ResponsePDU.clear();

	ResponsePDU.addParameter(responseStream);

	ResponsePDU.makePDU(ResponseBuf, MAX_PDU_BUFFER_LENGTH, ResponseBufLen);

	if (ResponseBufLen > 0)
	{
		ResponseBufLen = SockHelper->send(ResponseBuf, ResponseBufLen);

		if (ResponseBufLen < 0)
		{
			Error = ERR_SOCKET_SEND_FAILED;
		}
	}
	else
	{
		Error = ERR_PDU_MAKE_PDU_FAILED;
	}
	ResponsePDU.clear();

	return Error == SUCCESS;
}

bool RequestData::FlushPDU()
{
	Error = SUCCESS;
	ResponsePDU.makePDU(ResponseBuf, MAX_PDU_BUFFER_LENGTH, ResponseBufLen);

	if (ResponseBufLen > 0)
	{
		ResponseBufLen = SockHelper->send(ResponseBuf, ResponseBufLen);

		if (ResponseBufLen < 0)
		{
			Error = ERR_SOCKET_SEND_FAILED;
		}
	}
	else
	{
		Error = ERR_PDU_MAKE_PDU_FAILED;
	}
	ResponsePDU.clear();
	return Error == SUCCESS;
}

bool RequestData::ReceiveRequestData()
{
	bool res = false;
	if (SockHelper->receive(RequestBuf, MAX_PDU_BUFFER_LENGTH) > 0)
	{
		PDU::FillPDU(RequestBuf, MAX_PDU_BUFFER_LENGTH, RequestPDU);
		res = true;
	}
	return res;
}


bool DummyRequestData::SendResponseStream(stringstream& _stream, const OUTPUT_ENUM _type)
{
	if (_stream.str().length() == 0)
		return false;

	switch (_type)
	{
	case OUTPUT_HEAD:
		Logger::LogPoll(_stream.str(), POLL_HEAD);
		break;
	case OUTPUT_ERROR:
		Logger::LogPoll(_stream.str(), POLL_ERROR);
		break;
	case OUTPUT_WARN:
		Logger::LogPoll(_stream.str(), POLL_WARN);
		break;
	case OUTPUT_INFO:
		Logger::LogPoll(_stream.str(), POLL_SUCCESS);
		break;
	default:
		break;
	}
	_stream.str("");

	return true;
}

bool DummyRequestData::FlushPDU()
{
	return false;
}

bool DummyRequestData::ReceiveRequestData()
{
	return false;
}
