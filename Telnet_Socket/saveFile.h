#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <direct.h>
#pragma warning(disable : 4996)

using namespace std;

string CurrentDateTime() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
	std::string str(buffer);

	return str;
}


void saveToFile(string data, string ip)
{
	//cout << data << endl;
	string folderName = CurrentDateTime();
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
		while (1) {
			file >> row;
			if (file.eof())
				break;
			list.push_back(row);
		}
		cout << fileName << " has been loaded" << "\n" << endl;
		file.close();
	}
	return list;
}