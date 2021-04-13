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
			cin >> username;
			cout << "Enter Password: ";
			cin >> password;
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
			cin >> yesORno;
			transform(yesORno.begin(), yesORno.end(), yesORno.begin(), ::tolower);
			if (yesORno == "yes" || yesORno == "y") {
				filename = "ips.txt";
				ips = readFromFile(filename);
				if(ips.size() > 0)
					break;
			}
			else {
				cout << " \n Enter the file name, if the file exist in different directory write the full file loction";
				cin >> filename;
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

	vector<thread> threads;
	int maxThreads = 6 , incThread = maxThreads - 1;

	for (int i = 0; i < ips.size(); i++) {
		while (1) {
			try
			{
				if (i + incThread < ips.size())
				{
					for (int j = i, y = 0; y < (1 + incThread); j++, y++)
					{
						threads.push_back(thread(Telnet, ips.at(j), username, password, commands, j));
					}

					for (auto& thread : threads)
					{
						thread.join();
					}

					i += incThread;
					incThread = maxThreads - 1;
					break;
				}
				else
				{
					incThread--;
				}

				
			}
			catch (const std::exception& e)
			{

				cout << "\n" << endl;
				cout << "Error: " << e.what() << endl;
				cout << "\n" << endl;
			}
		}
		threads.clear();
	}


	cout << "FINISHED" << endl;
	cout << "\n" << endl;
	system("pause");
}