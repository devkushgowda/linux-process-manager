#include "cli_handler.hpp"
using namespace dpm;

bool CliRequestHandler::checkVersion(ERR_ENUM& _err)
{
	bool res = false;

	SocketClient sc;

	int resInt = sc.connectToServer(_IPAddress, _Port, _CreateSocket);

	if (resInt < 0)
	{
		_err = (ERR_ENUM)resInt;
	}
	else
	{
		PDU pdu;
		pdu.setFunctionCode((short)CMD_VERSION);
		int pduLength;
		pdu.makePDU(RequestBuf, MAX_PDU_BUFFER_LENGTH, pduLength);

		SocketHelper sh(resInt);

		resInt = sh.send(RequestBuf, pduLength);

		if (resInt < 0)
		{
			_err = (ERR_ENUM)resInt;
		}
		else
		{
			resInt = sh.receive(ResponseBuf, MAX_PDU_BUFFER_LENGTH);

			if (resInt < 0)
			{
				_err = (ERR_ENUM)resInt;
			}
			else
			{
				SysVar version;
				PDU::GetParameter(ResponseBuf, MAX_PDU_BUFFER_LENGTH, 1, version);
				if (version == (float)APP_VERSION)
				{
					res = true;
				}
				else
				{
					_err = ERR_APP_VERSION_MISMATCH;
				}
				pdu.clear();
				pdu.setFunctionCode((short)CMD_EXIT);
				pdu.makePDU(RequestBuf, MAX_PDU_BUFFER_LENGTH, pduLength);
				sh.send(RequestBuf, pduLength);
			}
		}
	}
	return res;
}

bool CliRequestHandler::executeRequest(ERR_ENUM& _err)
{
	bool res = false;

	SocketClient sc;

	int resInt = sc.connectToServer(_IPAddress, _Port, _CreateSocket);

	if (resInt < 0)
	{
		_err = (ERR_ENUM)resInt;
	}
	else
	{
		int pduLength;

		RequestPDU.makePDU(RequestBuf, MAX_PDU_BUFFER_LENGTH, pduLength);

		SocketHelper sh(resInt);

		resInt = sh.send(RequestBuf, pduLength);

		if (resInt < 0)
		{
			_err = (ERR_ENUM)resInt;
		}
		else
		{
			while (sh.receive(ResponseBuf, MAX_PDU_BUFFER_LENGTH) > 0)
			{
				short fc;
				PDU::GetFunctionCode(ResponseBuf, MAX_PDU_BUFFER_LENGTH, fc);
				if ((CMD_ENUM)fc == CMD_EXIT)
				{
					RequestPDU.clear();
					RequestPDU.setFunctionCode((short)CMD_EXIT);
					RequestPDU.makePDU(RequestBuf, MAX_PDU_BUFFER_LENGTH, pduLength);
					sh.send(RequestBuf, pduLength);
					break;
				}
				printResponse();
			}
		}
	}
	sc.disconnect();
	return res;
}

void CliRequestHandler::printResponse()
{
	SysVar response;
	PDU::GetParameter(ResponseBuf, MAX_PDU_BUFFER_LENGTH, 1, response);
	Logger::OpResult(response.vstring);
}

CliRequestHandler::CliRequestHandler(string _iPAddress, int _port, bool _createSocket) :_IPAddress(_iPAddress), _Port(_port), _CreateSocket(_createSocket)
{
}

void CliRequestHandler::handleCliRequest(int _argc, char** _args)
{
	stringstream logStream;
	CommandManager cm;
	cm.makeCommandPDU(_argc, _args, RequestPDU);
	ERR_ENUM errorCode = cm.getErrorCode();

	if (errorCode < 0)
	{
		Logger::LogError(ErrorsDef::GetErrorMessage((ERR_ENUM)errorCode));
	}
	else
	{
		if (cm.getCommandCode() == CMD_HELP)
		{
			Logger::OpResult(HelpDef::getCommandHelpMessage((CMD_ENUM)cm.getOptionCode()));
		}
		else
		{
			if (checkVersion(errorCode))
			{
				logStream << "CliRequestHandler::handleCliRequest(): " << "Handshake successful!, and version match.";
				Logger::LogTrace(logStream);
				executeRequest(errorCode);
			}
			else
			{
				logStream << "CliRequestHandler::handleCliRequest(): " << ErrorsDef::GetErrorMessage((ERR_ENUM)errorCode) + "\n";
				Logger::LogError(logStream);
			}
		}
	}
}
