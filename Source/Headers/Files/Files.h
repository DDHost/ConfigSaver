#include "../common.h"
#include <ctime>
#include <direct.h>
#pragma warning(disable : 4996)
#pragma once

using namespace std;

string CurrentDate() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
	std::string str(buffer);

	return str;
}

string CurrentDateTime() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "DATE_%d-%m-%Y.TIME_%H-%M", timeinfo);
	std::string str(buffer);

	return str;
}



void saveToFile(string data, string ip)
{
	string folderName = CurrentDate();
	_mkdir(folderName.c_str());
	ofstream file_;
	file_.open(folderName + "/" + ip + ".txt");
	file_ << data;
	file_.close();

	cout << ip << " Saved" << "\n " << endl;

}

vector<string> readFromFile(string fileName)
{

	cout << "\n Loading file:" << fileName << endl;
	cout << "\n" << endl;

	vector<string> list;

	fstream file;
	file.open(fileName, ios::in);
	if (!file) {
		cout << "File not exist" << "\n" << endl;
	}
	else {
		string row;
		// Get line from files and store each line in the vector variable
		while (getline(file, row)) {
			row.erase(remove(row.begin(), row.end(), ' '), row.end()); // erease white spaces
			list.push_back(row);
		}
		cout << fileName << " has been loaded" << "\n" << endl;
		file.close();
	}
	return list;
}

void LogFailed(string ip)
{
	string folderName = CurrentDate();
	//string filename = CurrentDateTime();
	string filename = folderName;
	_mkdir(folderName.c_str());

	fstream file_;
	file_.open(folderName + "\\log_" + filename + ".txt", fstream::app);
	file_ << ip << endl;
	file_.close();

	cout << "Failed to connect to " << ip << " logged in the file:" << "log_" + filename + ".txt" << "\n " << endl;

}


