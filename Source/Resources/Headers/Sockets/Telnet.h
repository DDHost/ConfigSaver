#ifndef TELNET_H
#define TELNET_H
#include "../common.h"
#include "../Files/Files.h"
#include <WS2tcpip.h>
#pragma once
#pragma comment(lib, "ws2_32.lib")

class Telnet
{
private:
	SOCKET sock;
	int port = 23; // The port to use for connection Telnet is '23'

	Files files;

	string password,
		username,
		TargetIP;

	vector<string> Commands;
	Printers printer;
	char buffer[4096];

	int Login();
	void Recive(bool print);
	int ReciveUntil(string para, bool print);
	int ConfigReciver(int num);
public:
	int Start(string RemoteHost, string user, string pass, vector<string> commands, int num);
};

#endif


