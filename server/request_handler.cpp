#include "request_handler.hpp"
using namespace dpm;

void RequestHandler::_ExecuteRequest(const int& _sockFD)
{
	Logger::LogTrace("Entering _ExecuteRequest()...");

	stringstream logStream;
	RequestData rd(_sockFD);
	if (rd.ReceiveRequestData())
	{
		auto fc = (CMD_ENUM)rd.RequestPDU.getFunctionCode();

		switch (fc)
		{
		case CMD_VERSION:
		{
			rd.ResponsePDU.setFunctionCode((short)CMD_VERSION);
			rd.ResponsePDU.addParameter((float)APP_VERSION);
			rd.FlushPDU();
			break;
		}
		case CMD_START:
		case CMD_STOP:
		case CMD_RESTART:
		case CMD_PING:
		{
			ProcCmdHandler(rd).execute();
			break;
		}
		case CMD_CONFIG:
		{
			ConfigCmdHandler(rd).execute();
			break;
		}
		case CMD_SETTING:
		{
			SettingsCmdHandler(rd).execute();
			break;
		}
		default:
		{
			rd.ResponsePDU.setFunctionCode((short)CMD_INVALID);
			logStream << ERR_NOT_IMPLEMENTED_ROUTINE_MESSAGE;
			rd.SendResponseStream(logStream, OUTPUT_ERROR);
			break;
		}
		}

		rd.ResponsePDU.setFunctionCode((short)CMD_EXIT);

		rd.FlushPDU();

		if (rd.ReceiveRequestData() && rd.RequestPDU.getFunctionCode() == CMD_EXIT)
		{
			rd.Error = SUCCESS;
		}
		else
		{
			rd.Error = ERR_REQUEST_ACK_FAILED;
		}

		if (rd.Error == SUCCESS)
		{
			logStream << "Response sent succesfully! -> " << "FC: " << rd.RequestPDU.getFunctionCode() << ", /length: " << rd.ResponseBufLen << ", t_id: " << this_thread::get_id();
			Logger::LogInfo(logStream);
		}
		else
		{
			logStream << "Failed to send response! -> " << "FC: " << rd.RequestPDU.getFunctionCode() << ", Async thread id:: " << this_thread::get_id();
			Logger::LogError(logStream);
		}
	}
	else
	{
		logStream << "ExecuteRequest() failed to receive request data. t_id:: " << this_thread::get_id();
		Logger::LogError(logStream);
	}
}


void RequestHandler::ServeRequest(int _sockFD)
{
	Logger::LogTrace("Entering ServeRequest()...");
	DPM_TRY
	{
		RequestHandler::_ExecuteRequest(_sockFD);
	}
	DPM_CATCH_LOG("RequestHandler::ServeRequest()");
	Logger::LogTrace("Exiting ServeRequest()...");

}
