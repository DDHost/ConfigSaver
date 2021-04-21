#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#pragma once
using namespace std;


#ifndef COMMON_H
#define COMMON_H 


class MyException : public std::exception {
    string msg;
    int id;

public:
    MyException(string msg_, int id_) : msg(msg_), id(id_) {}

    string get_msg() const { return msg; }
    int get_id() const { return id; }
};



class Printers
{
public:
	void Print(string msg);
	void Print2Spaces(string str);
	void PrintError(string str, exception error);
	void PrintCenteredText(string str, int w);
};

#endif




