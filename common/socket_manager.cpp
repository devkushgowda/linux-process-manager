#include "socket_manager.hpp"

using namespace dpm;

SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{
	close(_ServerFD);
}

int SocketServer::createServerSocket(
	int _port, int _addressFamily, int _socketType, int _protocolType, int _maxConQueue)
{
	Logger::LogTrace("SocketServer::createServerSocket()...");
	if (_port <= TCP_PORT_MIN || _port >= TCP_PORT_MAX)
		return ERR_SOCKET_INVALID_PORT;

	struct sockaddr_in serv_addr;
	_Port = _port;

	_ServerFD = socket(_addressFamily, _socketType | SOCK_CLOEXEC, _protocolType);
	if (_ServerFD < 0)
	{
		Logger::LogError("SocketServer::waitForConnection(): Socket creation failed.");
		return ERR_SOCKET_CREATION_FAILED;
	}
	serv_addr.sin_family = (unsigned short)_addressFamily;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons((unsigned short)_Port);

	if (bind(_ServerFD, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
	{
		Logger::LogError("SocketServer::waitForConnection(): Socket bind failed.");
		return ERR_SOCKET_BIND_FAILED;
	}
	if (listen(_ServerFD, _maxConQueue) < 0)
	{
		Logger::LogError("SocketServer::waitForConnection(): Socket listen failed.");
		return ERR_SOCKET_LISTEN_FAILED;
	}
	return _ServerFD;
}

int SocketServer::waitForConnection(bool _socketnoDelay)
{
	struct sockaddr_in serv_addr;
	Logger::LogTrace("SocketServer::waitForConnection()...");
	int _addr_len = sizeof(serv_addr);
	CurrentSocketFD = accept(_ServerFD, (struct sockaddr*) & serv_addr, (socklen_t*)&_addr_len);
	if (CurrentSocketFD > 0 && _socketnoDelay)
	{
		int value = 1;
		int error
			= setsockopt(CurrentSocketFD, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(value));
		if (error < 0)
		{
			Logger::LogError("SocketServer::waitForConnection(): Setsockopt options failed.");
		}
	}
	if (CurrentSocketFD < 0)
	{
		Logger::LogError("SocketServer::waitForConnection(): Failed to accept connections.");
	}
	return CurrentSocketFD;
}

int SocketServer::getSocketFD() const
{
	return _ServerFD;
}

int SocketServer::getPortNumber() const
{
	return _Port;
}


//Socket helper

SocketHelper::SocketHelper(int _socketFD)
	: _SocketFD(_socketFD)
{
}

SocketHelper::~SocketHelper()
{
	close(_SocketFD);
}

int SocketHelper::send(char* _buf, int _len)
{
	return BaseSocket::Send(_SocketFD, _buf, _len);
}

int SocketHelper::receive(char* _buf, int _len)
{
	return BaseSocket::Receive(_SocketFD, _buf, _len);
}

int SocketHelper::sendFile(string _fileName, int _fileSize)
{
	return BaseSocket::SendFile(_SocketFD, _fileName, _fileSize);
}

int SocketHelper::receiveFile(string _fileName, int _fileSize)
{
	return BaseSocket::ReceiveFile(_SocketFD, _fileName, _fileSize);
}

void SocketHelper::closeSocket()
{
	close(_SocketFD);
}

//Socket client

SocketClient::SocketClient()
{
}

SocketClient::~SocketClient()
{
	close(_SocketFD);
}

bool SocketClient::createSocket(
	bool _socketnoDelay, int _addressFamily, int _socketType, int _protocolType)
{
	Logger::LogTrace("SocketClient::createSocket()...");

	if ((_SocketFD = socket(_addressFamily, _socketType | SOCK_CLOEXEC, _protocolType)) < 0)
	{
		Logger::LogError("SocketClient::createSocket(): Socket creation failed.");
		return false;
	}
	if (_socketnoDelay)
	{
		int value = 1;
		int error = setsockopt(_SocketFD, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(value));
		if (error < 0)
			Logger::LogWarn("SocketClient::createSocket(): setsockopt failed.");
	}
	return true;
}

int SocketClient::connectToServer(string _ipAddress, int _port, bool _createSocket)
{
	Logger::LogTrace("SocketClient::connectToServer()...");

	if (_port <= TCP_PORT_MIN || _port >= TCP_PORT_MAX)
		return ERR_SOCKET_INVALID_PORT;

	struct sockaddr_in serv_addr;
	_IpAddress = _ipAddress;
	_Port = _port;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)_Port);

	if (_createSocket && !createSocket(true, AF_INET, SOCK_STREAM, IPPROTO_IP))
	{
		return ERR_SOCKET_CREATION_FAILED;
	}
	if (inet_pton(AF_INET, _ipAddress.c_str(), &serv_addr.sin_addr) < 0)
	{
		Logger::LogError("SocketClient::connectToServer(): Invalid ip address/ type not supported.");
		return ERR_SOCKET_INVALID_IP;
	}

	if (connect(_SocketFD, (struct sockaddr*) & serv_addr, sizeof(serv_addr)))
	{
		return ERR_SOCKET_SERVER_CONNECTION_FAILED;
	}

	return _SocketFD;
}

void SocketClient::disconnect()
{
	close(_SocketFD);
}


int SocketClient::getSocketFD() const
{
	return _SocketFD;
}

string SocketClient::getIpAddress() const
{
	return _IpAddress;
}

int SocketClient::getPortNumber() const
{
	return _Port;
}


//Extern functions

int BaseSocket::Send(int _socket, char* _buf, int _len)
{
	if (_socket <= 0 || _buf == NULL || _len <= 0)
		return ERR_SOCKET_SEND_FAILED;

	int totalSent = 0;
	int sentDataTimes = 0;

	char* tempBuffer = (char*)_buf;
	while (totalSent < _len)
	{
		int curSent = (int)send(_socket, (const char*)tempBuffer, _len - totalSent, MSG_DONTROUTE);

		sentDataTimes++;

		if (curSent == -1)
			return ERR_SOCKET_SEND_FAILED;

		totalSent += curSent;
		tempBuffer += curSent;
	}
	if (sentDataTimes > 1)
	{
		stringstream log;
		log << "BaseSocket::SendFile(): sent data " << sentDataTimes << " times.";
		Logger::LogTrace(log.str());
	}

	return totalSent;
}


int BaseSocket::Receive(int _socket, char* _buf, int _len)
{
	if (_socket <= 0 || _buf == NULL || _len <= 0)
		return ERR_SOCKET_RCV_FAILED;

	int curRecvd = 0, totalRecvd = 0, totalToReceive = 0;

	if (_buf == NULL)
		return ERR_SOCKET_RCV_FAILED;

	memset(_buf, 0, _len);

	// check the amount of data on the buffer

	curRecvd = (int)recv(_socket, _buf, 2, 0);

	if (curRecvd == 0 || curRecvd == -1)
		return ERR_SOCKET_RCV_FAILED;

	unsigned short amount = 0;
	char* ptr = (char*)&amount;
	*ptr = *(_buf + 1);
	*(ptr + 1) = *(_buf);

	totalToReceive = amount;

	if ((curRecvd < 2) || (curRecvd - 2 > totalToReceive))
		return ERR_SOCKET_RCV_FAILED;


	memmove(_buf, _buf + 2, ((_len < curRecvd) ? _len : curRecvd - 2));

	totalRecvd = curRecvd - 2;

	// continue to receive data

	while (totalRecvd < totalToReceive)
	{
		curRecvd = (int)recv(_socket, _buf + totalRecvd, totalToReceive - totalRecvd, 0);

		if (curRecvd == -1)
			return ERR_SOCKET_RCV_FAILED;

		totalRecvd += curRecvd;
	}

	return totalRecvd;
}

int BaseSocket::ReceiveFile(int _socket, string _fileName, int _fileSize)
{
	if (_socket <= 0 || _fileName.size() < 1 || _fileSize <= 0)
		return ERR_SOCKET_RCV_FILE_FAILED;

	int _res = 0;
	char* fileBuffer = new char[_fileSize];
	int totalRecvd = 0, totalToReceive = 0;

	totalToReceive = _fileSize;
	totalRecvd = 0;


	while (totalRecvd < totalToReceive)
	{
		_res = (int)recv(_socket, fileBuffer + totalRecvd, totalToReceive - totalRecvd, 0);

		if (_res == 0 || _res == -1)
			break;

		totalRecvd += _res;
	}

	if (totalRecvd != totalToReceive)
		return ERR_SOCKET_RCV_FILE_FAILED;

	// save file
	ofstream file(_fileName.c_str(), ios::binary);
	file.write(fileBuffer, _fileSize);
	file.close();
	delete[] fileBuffer;
	return totalRecvd;
}

int BaseSocket::SendFile(int _socket, string _fileName, int _fileSize)
{
	if (_socket <= 0 || _fileName.size() < 1 || _fileSize <= 0)
		return ERR_SOCKET_SEND_FILE_FAILED;

	int sentDataTimes = 0, bufferSize = 0, _res = 0, lastSentBytes = 0, totalSentBytes = 0,
		lastReadBytes = 0, totalReadBytes = 0;

	bufferSize
		= (_fileSize > MAX_FILE_BUF_LENGTH) ? MAX_FILE_BUF_LENGTH : _fileSize; // Send buffer size
	FILE* fs = fopen(_fileName.c_str(), "rb"); // Open file in Read Binary mode
	char* tempFileBuffer = new char[bufferSize]; // File size
	totalReadBytes = lastReadBytes = (int)fread(tempFileBuffer, sizeof(char), bufferSize, fs);

	while (totalSentBytes >= _fileSize)
	{
		_res = (int)send(_socket, tempFileBuffer, lastReadBytes - lastSentBytes, 0);

		if (_res == -1)
			break;

		lastSentBytes += _res;
		totalSentBytes += _res;

		if (totalSentBytes == totalReadBytes)
		{
			lastSentBytes = 0;
			totalReadBytes += lastReadBytes = (int)fread(tempFileBuffer, sizeof(char), bufferSize, fs);
			if (lastReadBytes < 0)
				break;
		}
		sentDataTimes++;
	}
	if (sentDataTimes > 1)
	{
		stringstream log;
		log << "BaseSocket::SendFile(): sent data " << sentDataTimes << " times.";
		Logger::LogTrace(log.str());
	}
	fclose(fs);
	delete[] tempFileBuffer;
	if (totalSentBytes != _fileSize)
		return ERR_SOCKET_SEND_FILE_FAILED;
	return totalSentBytes;
}
