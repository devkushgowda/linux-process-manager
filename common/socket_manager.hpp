
#ifndef __SOCKET_MANAGER_HPP_
#define __SOCKET_MANAGER_HPP_


#include <sstream>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "logger.hpp"
#include "common_def.h"
#include "err_def.hpp"

#define MAX_FILE_BUF_LENGTH 65000
#define MAX_FILE_SIZE 1073741824 // 1024 MB

#define SERVER_CON_QUEUE_BUF 50

using namespace std;

namespace dpm
{
	class BaseSocket
	{
	public:
	static int ReceiveFile(int _socket, string _fileName, int _fileSize);
	static int SendFile(int _socket, string _fileName, int _fileSize);
	static int Send(int _socket, char* _buf, int _len);
	static int Receive(int _socket, char* _buf, int _len);
	};

	class SocketServer
	{
	private:
		int _ServerFD;
		int _Port;

	public:
		int CurrentSocketFD;
		SocketServer();
		~SocketServer();
		int createServerSocket(int _port, int _addressFamily = AF_INET, int _socketType = SOCK_STREAM,
			int _protocolType = IPPROTO_IP,
			int _maxConQueue = SERVER_CON_QUEUE_BUF); // create the socket with bounded port

		int waitForConnection(bool _socketnoDelay = true); // Which accept the connetion

		int getSocketFD() const;
		int getPortNumber() const;
	};

	class SocketHelper
	{
	private:
		const int _SocketFD;

	public:
		SocketHelper(int _socketFD);
		~SocketHelper();
		int send(char* _buf, int _len); // Sends data over socket
		int receive(char* _buf, int _len); // Receive data over socket
		int sendFile(string _fileName, int _fileSize);
		int receiveFile(string _fileName, int _fileSize);
		void closeSocket(); // Close the socket
	};

	class SocketClient
	{
	private:
		int _SocketFD;
		string _IpAddress;
		int _Port;


	public:
		SocketClient();
		~SocketClient();

		bool createSocket(bool _socketnoDelay = true, int _addressFamily = AF_INET,
			int _socketType = SOCK_STREAM, int _protocolType = IPPROTO_IP);
		int connectToServer(string _ipAddress, int _port, bool _createSocket = false);

		void disconnect();

		int getSocketFD() const;
		string getIpAddress() const;
		int getPortNumber() const;
	};
}
#endif
