#include "../common.h"
#include <ctime>
#include <direct.h>
#include <sys/stat.h>

#pragma once
#pragma warning(disable : 4996)


#ifndef FILES_H
#define FILES_H
class Files
{
public:
	string CurrentDate();
	string CurrentDateTime();
	void saveToFile(string data, string ip);
	vector<string> readFromFile(string fileName);
	void LogFailed(string ip, int num);
	bool IsPathExist(const string& s);
};

#endif

