#include "../../Headers/Sockets/Telnet.h"
#pragma once

int Telnet::Start(string RemoteHost, string user, string pass, vector<string> commands, int num)
{
	this->password = pass;
	this->username = user;
	this->Commands = commands;
	this->TargetIP = RemoteHost;

	this->printer.Print("Connecting to "+TargetIP + '\n');

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		this->printer.Print("Can't start Winsock, Err #" + wsResult + '\n');
		return 0;
	}

	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock == INVALID_SOCKET)
	{
		this->printer.Print("Can't create socket, Err #" + WSAGetLastError() + '\n');
		WSACleanup();
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(this->port);
	inet_pton(AF_INET, this->TargetIP.c_str(), &hint.sin_addr);

	// init the connection 
	int connResult = connect(this->sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		this->files.LogFailed(this->TargetIP,num);
		closesocket(this->sock);
		WSACleanup();
		return 0;
	}

	if (this->Login())
	{
		this->printer.Print("Successfully logged into '" + this->TargetIP + "'  \n");
	}
	else
	{
		this->files.LogFailed(TargetIP, num);
		closesocket(this->sock);
		return 0;
	}

	int sendResult;
	for (int i = 0; i < this->Commands.size(); i++)
	{
		sendResult = send(sock, this->Commands[i].c_str(), this->Commands[i].size() + 1, 0);
		if (sendResult != -1) {
			if (this->Commands[i] == "sh run \n")
			{
				if (!this->ConfigReciver(num)) {
					return 0;
				}
				else 
				{
					break;
				}
			}
		}
		else
		{
			return 0;
		}
	}

	closesocket(this->sock);
	WSACleanup();
	return 1;
}

int Telnet::Login()
{

	this->Recive(false);


	if (this->ReciveUntil("Username: ", false))
	{
		send(this->sock, this->username.c_str(), this->username.size() + 1, 0);
	}
	else
	{
		return 0;
	}

	if (this->ReciveUntil("Password: ", false))
	{
		send(this->sock, this->password.c_str(), this->password.size() + 1, 0);
	}
	else
	{
		return 0;
	}

	if (this->ReciveUntil("#", false))
	{
		// successfully loggedd in
		return 1;
	}

	return 0;
}

void Telnet::Recive(bool print)
{
	int bytesReceived;
	string recvDATA = "";

	do
	{	
		if (bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0))
		{
			recvDATA = string(this->buffer, 0, bytesReceived);
			if (print)
			{
				this->printer.Print(recvDATA + '\n');
			}
			break;
		}

	} while (sock != INVALID_SOCKET);

}

int Telnet::ReciveUntil(string parameter, bool print)
{
	int bytesReceived;
	string recvDATA = "";

	do
	{
		recvDATA = "";
		bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0);
		if (bytesReceived)
		{
			recvDATA = string(this->buffer, 0, bytesReceived);
			if (print)
			{
				this->printer.Print(recvDATA + '\n');
			}
			if (recvDATA.find("% Authentication failed\r\n") != string::npos)
			{
				return 0;
			}

			if (recvDATA.find(parameter) != string::npos)
			{
				return 1;
			}
		}

	} while (this->sock != INVALID_SOCKET);
	return 0;
}

int Telnet::ConfigReciver(int num)
{
	int bytesReceived;
	string recvDATA = "";
	bool startStore = false;
	
	this->printer.Print("Retriving configuration form " + this->TargetIP+'\n');

	do {

		// recive data
		bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0);

		// handle errors
		if (bytesReceived == -1) {
			return 0;
		}

		// bytes to string as long as the switch does not start to send running config
		if (!startStore) {
			recvDATA = string(this->buffer, 0, bytesReceived);
		}

		// start the saving actions when switch try to send running config
		if (recvDATA.find("Building configuration...") != string::npos) {
			recvDATA = "";
			startStore = true;
		}
		// start when switch sebd the all running config
		if (startStore) {
			// store the running config to string
			recvDATA += string(this->buffer, 0, bytesReceived);

			// Detect when fully recived config
			if (recvDATA.find("end\r\n") != string::npos && recvDATA[recvDATA.length()-1] == '#' || recvDATA.find("^@") != string::npos) {
				this->files.saveToFile(recvDATA, this->TargetIP); // save config to txt file
				return 1;
			}
		}

	} while (this->sock != INVALID_SOCKET);
	return 0;
}
