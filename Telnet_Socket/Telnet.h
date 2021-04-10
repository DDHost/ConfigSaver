#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include "Files.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int Reciver(SOCKET sock, string ip, bool save, int _bufSize)
{
	//char* buf = new char[_bufSize];
	char buf[4096];
	string data;
	bool startStore = false;
	int bytesReceived = 0;

	while (sock != INVALID_SOCKET) {
		// recive data
		bytesReceived = recv(sock, buf, sizeof(buf), 0);

		// handle errors
		if (bytesReceived == -1) {
			save = false;
			return 0;
		}

		// bytes to string as long as the switch does not start to send running config
		if (!startStore) {
			data = string(buf, 0, bytesReceived);
		}

		// start the saving actions when switch try to send running config
		if (data.find("Building configuration...\r\n") != string::npos){
			data = "";
			startStore = true;
		}

		// start when switch sebd the all running config
		if (startStore) {
			// store the running config
			data += string(buf, 0, bytesReceived);
			if (save == true) {
				bytesReceived = 0;
				saveToFile(data, ip); // store running to file
				save = false;
				break;
			}
			// when to stop storing and allow saving the running config to file
			if (data.find("!\r\n!\r\nend\r\n") != string::npos) {
				save = true;
			}
		}

	}
	return 1;
}

int Telnet(string RemoteHost, string username, string password, vector<string> commands)
{
	Sleep(10);
	cout << "Connecting to " << RemoteHost << " \r\n" << endl;

	int  bufSize = 4096; // the buffer size (for dynamic range)
	bool save = false;

	string TargetIP = RemoteHost;		// The remote device you want to connect
	int port = 23;					// The port to use for connection Telnet is '23'

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << "\n" << endl;

		return 0;
	}


	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << "\n" << endl;
		WSACleanup();
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, TargetIP.c_str(), &hint.sin_addr);

	// init the connection 
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		//cerr << "Failed to connect to " << TargetIP << " Error: " << WSAGetLastError() << endl;
		LogFailed(TargetIP);
		closesocket(sock);
		WSACleanup();
		return 0;
	}


	int sendResult;

	// login

	sendResult = send(sock, username.c_str(), username.size() + 1, 0);
	if (sendResult == -1)
	{
		return 0;
	}
	
	sendResult = send(sock, password.c_str(), password.size() + 1, 0);
	if (sendResult == -1)
	{
		return 0;
	}

	// need to add login verfiction 
	
	for (int i = 0; i < commands.size(); i++)
	{
		sendResult = send(sock, commands[i].c_str(), commands[i].size() + 1, 0);
		if (sendResult != -1) {
			if (commands[i] == "sh run \n")
			{
				if (Reciver(sock, TargetIP, save, bufSize) == 0) {
					return 0;
				}
			}
		}
		else 
		{
			return 0;
		}
	}

	closesocket(sock);
	WSACleanup();

	return 1;
}