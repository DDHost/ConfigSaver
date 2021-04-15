#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#ifndef COMMON_H
#define COMMON_H 
#pragma once
using namespace std;

class Printers
{
public:
	void Print(string msg);
	void Print2Spaces(string str);
	void PrintError(string str, exception error);
	void PrintCenteredText(string str, int w);
};

#endif




