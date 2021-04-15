#include "Main.h"

int main()
{
	settings config = read_INIFile(); // retrive settings from config.ini
	SetConsoleTitleA("ConfigSaver - by DDHost"); // Set Title to the window
	Console(config.width, config.height); // Set the window size (disable resize)
	PrintCenteredText(menu_TITLE,config.width); // Print The Ascii art 'ConfigSaver'

	string username, password, yesORno;
	while (1) 
	{
		try 
		{
			Print2Spaces("Use the default login information(yes/y or enter for no): ");
			cin >> yesORno;
			transform(yesORno.begin(), yesORno.end(), yesORno.begin(), ::tolower);
			if (yesORno == "yes" || yesORno == "y") 
			{
				username = config.username;
				password = config.password;
				break;
			}
			else
			{
				// get the switches user and password
				Print2Spaces("Enter Username: ");
				cin >> username;
				Print2Spaces("Enter Password: ");
				cin >> password;
				break;
			}
		}
		catch (const std::exception& err)
		{
			PrintError("Error: ", err);
		}

	}

	vector<string> ips;
	string filename;
	while (1) {
		try
		{
			Print2Spaces("Use the default file '"+config.file+"' (yes / y or enter for no): ");
			cin >> yesORno;
			transform(yesORno.begin(), yesORno.end(), yesORno.begin(), ::tolower);
			if (yesORno == "yes" || yesORno == "y") {
				filename = config.file;
				ips = readFromFile(filename);
				if(ips.size() > 0)
					break;
			}
			else {
				Print("\n");
				Print2Spaces("Enter the file name, if the file exist in different directory write the full file loction");
				cin >> filename;
				if (typeid(filename).name() == "string") {
					ips = readFromFile(filename);
					if (ips.size() > 0)
						break;
				}
			}
		}
		catch (const std::exception& err)
		{
			PrintError("Error: ", err);
		}

	}

	vector<thread> threads;
	int maxThreads = config.maxThreads, incThread = maxThreads - 1;
	for (int i = 0; i < ips.size(); i++) {
		while (1) {
			try
			{
				if (i + incThread < ips.size())
				{
					for (int j = i, y = 0; y < (1 + incThread); j++, y++)
					{
						threads.push_back(thread(Telnet, ips.at(j), username, password, config.COMMANDS, j));
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
			catch (const std::exception& err)
			{
				PrintError("Error: ", err);
			}
		}
		threads.clear();
	}

	Print("\n");
	Print("FINISHED");
	Print("\n");
	system("pause");
}