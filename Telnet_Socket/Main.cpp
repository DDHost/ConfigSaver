#include <iostream>
#include <string>
#include <vector>
#include "Telnet.h"
#include <thread>
#include <future>
#include <typeinfo>

using namespace std;

int main()
{
	string username, password;
	while (1) 
	{
		try 
		{
			// get the switches user and password
			cout << "Enter Username: ";
			std::cin >> username;
			cout << "Enter Password: ";
			std::cin >> password;
			break;
		}
		catch (const std::exception& e)
		{
			cout << "\n" << endl;
			cout << "Error: " << e.what() << endl;
			cout << "\n" << endl;
		}

	}

	vector<string> ips;
	vector<string> commands;
	commands.push_back("terminal len 0 \n");
	commands.push_back("sh run \n");

	string filename, yesORno;
	while (1) {
		try
		{
			cout << "Use the default file 'ips.txt' (yes/y or enter for your choice): ";
			std::cin >> yesORno;
			transform(yesORno.begin(), yesORno.end(), yesORno.begin(), ::tolower);
			if (yesORno == "yes" || yesORno == "y") {
				filename = "ips.txt";
				ips = readFromFile(filename);
				if(ips.size() > 0)
					break;
			}
			else {
				cout << " \n Enter the file name, if the file exist in different directory write the full file loction";
				std::cin >> filename;
				if (typeid(filename).name() == "string") {
					ips = readFromFile(filename);
					if (ips.size() > 0)
						break;
				}
			}
		}
		catch (const std::exception& e)
		{
			cout << "\n" << endl;
			cout << "Error: " << e.what() << endl;
			cout << "\n" << endl;
		}

	}

	for (int i = 0; i < ips.size(); i++) {
		try 
		{
			if (i + 5 < ips.size()) {
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				thread Telnet2(Telnet, ips.at(i + 1), username, password, commands);
				thread Telnet3(Telnet, ips.at(i + 2), username, password, commands);
				thread Telnet4(Telnet, ips.at(i + 3), username, password, commands);
				thread Telnet5(Telnet, ips.at(i + 4), username, password, commands);
				thread Telnet6(Telnet, ips.at(i + 5), username, password, commands);
				Telnet1.join();
				Telnet2.join();
				Telnet3.join();
				Telnet4.join();
				Telnet5.join();
				Telnet6.join();
				i += 5;
			}
			else if (i + 4 < ips.size()) {
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				thread Telnet2(Telnet, ips.at(i + 1), username, password, commands);
				thread Telnet3(Telnet, ips.at(i + 2), username, password, commands);
				thread Telnet4(Telnet, ips.at(i + 3), username, password, commands);
				thread Telnet5(Telnet, ips.at(i + 4), username, password, commands);
				Telnet1.join();
				Telnet2.join();
				Telnet3.join();
				Telnet4.join();
				Telnet5.join();
				i += 4;
			}
			else if (i + 3 < ips.size()) {
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				thread Telnet2(Telnet, ips.at(i + 1), username, password, commands);
				thread Telnet3(Telnet, ips.at(i + 2), username, password, commands);
				thread Telnet4(Telnet, ips.at(i + 3), username, password, commands);
				Telnet1.join();
				Telnet2.join();
				Telnet3.join();
				Telnet4.join();
				i += 3;
			}
			else if (i+2 < ips.size()) {
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				thread Telnet2(Telnet, ips.at(i + 1), username, password, commands);
				thread Telnet3(Telnet, ips.at(i + 2), username, password, commands);
				Telnet1.join();
				Telnet2.join();
				Telnet3.join();
				i += 2;
			}			
						
			else if (i + 1 < ips.size()) {
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				thread Telnet2(Telnet, ips.at(i + 1), username, password, commands);
				Telnet1.join();
				Telnet2.join();
				i += 1;
			}
			else 
			{
				thread Telnet1(Telnet, ips.at(i), username, password, commands);
				Telnet1.join();
			}
		}
		catch (const std::exception& e)
		{
			cout << "\n" << endl;
			cout << "Error: " << e.what() << endl;
			cout << "\n" << endl;
		}
	}

	cout << "FINISHED" << endl;
	cout << "\n" << endl;
	system("pause");
}