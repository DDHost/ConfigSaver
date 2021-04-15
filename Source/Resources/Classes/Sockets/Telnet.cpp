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

	while (this->sock != INVALID_SOCKET)
	{
		if (this->Login())
		{
			this->printer.Print("Successfully logged into '"+ this->TargetIP +"'  \n");
			break;
		}
		else
		{
			this->files.LogFailed(TargetIP,num);
			closesocket(this->sock);
			return 0;
		}
	}

	int sendResult;
	for (int i = 0; i < this->Commands.size(); i++)
	{
		sendResult = send(sock, this->Commands[i].c_str(), this->Commands[i].size() + 1, 0);
		if (sendResult != -1) {
			if (this->Commands[i] == "sh run \n")
			{
				if (!this->ConfigReciver(false, num)) {
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
	this->bytesReceived = 0;
	this->recvDATA = "";

	while (sock != INVALID_SOCKET)
	{	
		if (this->bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0))
		{
			this->recvDATA = string(this->buffer, 0, this->bytesReceived);
			if (print)
			{
				this->printer.Print(this->recvDATA + '\n');
			}
			break;
		}

	}

}

int Telnet::ReciveUntil(string parameter, bool print)
{
	this->bytesReceived = 0;
	this->recvDATA = "";

	while (this->sock != INVALID_SOCKET)
	{
		this->recvDATA = "";
		this->bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0);
		if (this->bytesReceived)
		{
			this->recvDATA = string(this->buffer, 0, this->bytesReceived);
			if (print)
			{
				this->printer.Print(this->recvDATA + '\n');
			}
			if (this->recvDATA.find("% Authentication failed\r\n") != string::npos)
			{
				return 0;
			}

			if (this->recvDATA.find(parameter) != string::npos)
			{
				break;
			}
		}

	}
	return 1;
}

int Telnet::ConfigReciver(bool save, int num)
{

	this->bytesReceived = 0;
	this->recvDATA = "";
	bool startStore = false;
	
	this->printer.Print("Retriving configuration form " + this->TargetIP+'\n');

	while (this->sock != INVALID_SOCKET) {

		// recive data
		this->bytesReceived = recv(this->sock, this->buffer, sizeof(this->buffer), 0);

		// handle errors
		if (this->bytesReceived == -1) {
			save = false;
			return 0;
		}

		// bytes to string as long as the switch does not start to send running config
		if (!startStore) {
			this->recvDATA = string(this->buffer, 0, this->bytesReceived);
		}

		// start the saving actions when switch try to send running config
		if (this->recvDATA.find("Building configuration...\r\n") != string::npos) {
			this->recvDATA = "";
			startStore = true;
		}
		// start when switch sebd the all running config
		if (startStore) {
			// store the running config
			this->recvDATA += string(this->buffer, 0, bytesReceived);
			if (save == true) {
				bytesReceived = 0;
				this->files.saveToFile(this->recvDATA, this->TargetIP); // store running to file
				save = false;
				break;
			}
			// when to stop storing and allow saving the running config to file
			if (this->recvDATA.find("!\r\nend\r\n") != string::npos && this->recvDATA.find("^@") != string::npos) {
				save = true;
			}
		}

	}
	return 1;
}
