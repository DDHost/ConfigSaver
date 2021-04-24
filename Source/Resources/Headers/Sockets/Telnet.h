#include "../common.h"
#include "../Files/Files.h"
#include <WS2tcpip.h>

#pragma once
#pragma comment(lib, "ws2_32.lib")

#ifndef TELNET_H
#define TELNET_H

class Telnet
{
private:

	enum Exceptions
	{
		excClose = 0, excLog, excRetry, excEnableMode
	};

	SOCKET sock;

	int port = 23, // The port to use for connection Telnet is '23'
		ID;

	Files files;

	string password,
		username,
		TargetIP;

	vector<string> Commands;
	Printers printer;
	char buffer[4096];

	void CreateSocket();
	void Recive(bool print);
	void sendCommands();

	void Login();
	int ReciveUntil(string parameter);
	int ConfigReciver(int num);

public:
	void Start(string RemoteHost, string user, string pass, vector<string> commands, int num);
};

#endif


