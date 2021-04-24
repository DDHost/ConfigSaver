#include "Resources/Headers/Main.h"

int main()
{
	settings config = read_INIFile(); // retrive settings from config.ini
	SetConsoleTitleA("ConfigSaver - by DDHost"); // Set Title to the window
	Main main;
	main.Console(config.width, config.height); // Set the window size (disable resize)
	main.printer.PrintCenteredText(main.GETMENUTITLE(),config.width); // Print The Ascii art 'ConfigSaver'

	string msg = "Username: "+ config.username + "\n" + "Password: " + config.password + "\n" 
		+ "Ip list: " + config.file + "\n\n";

	main.printer.Print(msg);

	// want to add settings option by mark selecting

	main.printer.Print2Spaces("Enter any key to start: ");
	cin.get();

	string username = config.username, 
		password = config.password,
		filename = config.file;

	Files files;
	vector<string> ips;
	ips = files.readFromFile(filename);

	vector<thread> threads;
	int maxThreads = config.maxThreads, incThread = maxThreads - 1;
	for (int i = 0; i < ips.size(); i++) {
		do{
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
		}while (1);
		threads.clear();
	}

	main.printer.Print("\n");
	main.printer.Print("FINISHED");
	main.printer.Print("\n");
	system("pause");
}