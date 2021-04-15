#include "../common.h"
#include <unordered_map>
#pragma once

using namespace std;

#ifndef iniFileSettings
struct settings
{
	int height;
	int width;
	int maxThreads;
	string file;
	vector<string> COMMANDS;
	string username;
	string password;
};
#endif

const char* DATA_SETTINGS =
"[CONFIG]\n"
"height=600\n"
"width=800\n"
"maxThreads=6\n"
"file=ips.txt\n"
"COMMANDS=ter len 0,sh run\n"
"username=root\n"
"password=toor\n";

void create_INIFile(string fileName)
{
	fstream file;
	cout << "\n\n" << endl;
	cout << "config.ini is missing" << endl;
	cout << "creating config.ini..." << endl;

	file.open(fileName, fstream::app);
	file << DATA_SETTINGS;

	cout << "config.ini has been created \n\n" << endl;
}

unordered_map<string, string> mapped_Config(string filename)
{
	fstream file;
	file.open(filename, ios::in);
	size_t pos;
	string row, key, value;
	unordered_map<string, string> MappedConfig;
	while (getline(file, row))
	{
		if (row.find("[") == string::npos && row.find("]") == string::npos) {
			pos = row.find("=");
			key = row.substr(0, pos);
			value = row.substr(pos + 1);
			MappedConfig[key] = value;
		}
	}
	return MappedConfig;
}

settings read_INIFile()
{
	string fileName = "config.ini";
	fstream file;
	file.open(fileName, ios::in);
	while(!file)
	{
		create_INIFile(fileName);
		file.open(fileName, ios::in);
	}
	
	size_t pos;
	unordered_map<string, string> tempConfig = mapped_Config(fileName);
	settings config;

	config.height = stoi(tempConfig["height"]);
	config.width = stoi(tempConfig["width"]);
	config.maxThreads = stoi(tempConfig["maxThreads"]);
	config.file = tempConfig["file"];
	config.username = tempConfig["username"];
	config.password = tempConfig["password"];

	// string commands pushed into string vector, separate word by comma(',')
	string command, COMMANDS = tempConfig["COMMANDS"];
	while (COMMANDS.length() > 0)
	{
		if (COMMANDS.find(",") != string::npos)
		{
			pos = COMMANDS.find(",");
			command = COMMANDS.substr(0, pos);
			config.COMMANDS.push_back(command + " \n");
			COMMANDS = COMMANDS.substr(pos + 1);
		}
		else
		{
			config.COMMANDS.push_back(COMMANDS + " \n");
			COMMANDS = "";
		}
	}

	return config;
}