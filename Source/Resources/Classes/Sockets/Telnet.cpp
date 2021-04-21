#include "../../Headers/Sockets/Telnet.h"
#pragma once

int Telnet::Start(string RemoteHost, string user, string pass, vector<string> commands, int id)
{
	this->password = pass;
	this->username = user;
	this->Commands = commands;
	this->TargetIP = RemoteHost;
	this->ID = id;

	this->printer.Print("Connecting to " + this->TargetIP + '\n');

	try
	{
		this->CreateSocket();
		
		cout << this->sock << endl;
		cout << this->sock << endl;
		cout << this->sock << endl;
		this->Login();
	}
	catch (MyException& exp)
	{
		string error = exp.get_msg();

		switch (exp.get_id())
		{
		case 1: // retry

			break;
		case 0: // log to file
			this->files.LogFailed(this->TargetIP, this->ID);
		case -1:
		default:
			this->printer.Print(error + '\n'); // print error
			closesocket(this->sock);
			WSACleanup();
			return 0;
		}
	}

	this->files.LogFailed(this->TargetIP, this->ID);
	closesocket(this->sock);
	return 0;

	int sendResult;
	for (int i = 0; i < this->Commands.size(); i++)
	{
		sendResult = send(sock, this->Commands[i].c_str(), this->Commands[i].size() + 1, 0);
		if (sendResult != -1) {
			if (this->Commands[i] == "sh run \n")
			{
				if (!this->ConfigReciver(this->ID)) {
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
}

void Telnet::CreateSocket()
{
	// Init Winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2); // winsock version
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		throw "Can't start Winsock, Err # ", 0;
	}

	// Create Socket
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock == -1)
	{
		WSACleanup();
		throw MyException("Failed to create a socket with '" + this->TargetIP + "'", 0);
	}

	// The remote device socket
	sockaddr_in listen_addr;
	listen_addr.sin_family = AF_INET; // address family ipv4
	listen_addr.sin_port = htons(this->port); // sets the listen port
	inet_pton(AF_INET, this->TargetIP.c_str(), &listen_addr.sin_addr);

	// Connect to the device
	int connResult = connect(this->sock, (sockaddr*)&listen_addr, sizeof(listen_addr));
	if (connResult == -1)
	{
		closesocket(this->sock);
		WSACleanup();
		throw MyException("Failed to establish connection with '" + this->TargetIP + "'", 0);
	}

	this->printer.Print("Connection has been establish with '" + this->TargetIP + "'\n");
}



void Telnet::Login()
{
	int Retry = 0;

	do {
		this->Recive(false);
		try
		{
			this->ReciveUntil("Username: ", false);
			send(this->sock, this->username.c_str(), this->username.size() + 1, 0);

			this->ReciveUntil("Password: ", false);
			send(this->sock, this->password.c_str(), this->password.size() + 1, 0);

			this->ReciveUntil("#", false);
			break;
		}
		catch (MyException& exp)
		{
			string error = exp.get_msg();

			switch (exp.get_id())
			{
			case 1: // retry
				this->printer.Print(exp.what() + '\n'); // print error
				break;
			case -1:
			default:
				this->printer.Print(error + '\n'); // print error
				throw MyException("Failed to login to '" + this->TargetIP, 0);
			}
		}
	} while (Retry<2 && this->sock != -1);

	this->printer.Print("Successfully logged into '" + this->TargetIP + "'");
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
				throw MyException("Authentication failed retring login to '" + TargetIP + "'", 1);
			}

			if (recvDATA.find(parameter) != string::npos)
			{
				return 1;
			}
		}
		else
		{
			throw MyException("Authentication failed 2 times, aborting from '" + TargetIP + "'", -1);
		}

	} while (this->sock != INVALID_SOCKET);

	throw MyException("Connection has been lost with '" + TargetIP + "'", -1);
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
