#include "Resources/Headers/Main.h"


int main()
{
	settings config = read_INIFile(); // retrive settings from config.ini
	SetConsoleTitleA("ConfigSaver - by DDHost"); // Set Title to the window
	Main main;
	main.Console(config.width, config.height); // Set the window size (disable resize)
	main.printer.PrintCenteredText(main.GETMENUTITLE(),config.width); // Print The Ascii art 'ConfigSaver'

	string username, password, yesORno;
	while (1) 
	{
		try 
		{
			main.printer.Print2Spaces("Use the default login information(yes/y or enter for no): ");
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
				main.printer.Print2Spaces("Enter Username: ");
				cin >> username;
				main.printer.Print2Spaces("Enter Password: ");
				cin >> password;
				break;
			}
		}
		catch (const std::exception& err)
		{
			main.printer.PrintError("Error: ", err);
		}

	}

	Files files;

	vector<string> ips;
	string filename;
	while (1) {
		try
		{
			main.printer.Print2Spaces("Use the default file '"+config.file+"' (yes / y or enter for no): ");
			cin >> yesORno;
			transform(yesORno.begin(), yesORno.end(), yesORno.begin(), ::tolower);
			if (yesORno == "yes" || yesORno == "y") {
				filename = config.file;
				ips = files.readFromFile(filename);
				if(ips.size() > 0)
					break;
			}
			else {
				main.printer.Print("\n");
				main.printer.Print2Spaces("Enter the file name, if the file exist in different directory write the full file loction");
				cin >> filename;
				if (typeid(filename).name() == "string") {
					ips = files.readFromFile(filename);
					if (ips.size() > 0)
						break;
				}
			}
		}
		catch (const std::exception& err)
		{
			main.printer.PrintError("Error: ", err);
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
						threads.push_back(thread(&Telnet::Start, Telnet(), ips.at(j), username, password, config.COMMANDS, j));
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
				main.printer.PrintError("Error: ", err);
			}
		}
		threads.clear();
	}

	main.printer.Print("\n");
	main.printer.Print("FINISHED");
	main.printer.Print("\n");
	system("pause");
}