#include "../Headers/common.h"
#pragma once

void Printers::Print(string msg)
{
	cout << msg;
}

void Printers::Print2Spaces(string str)
{
	Print("  " + str);
}

void Printers::PrintError(string str, exception error)
{
	string msg = str + error.what();
	Print("\n");
	Print2Spaces(msg);
	Print("\n");
}

void Printers::PrintCenteredText(string str, int w)
{
	int len = str.length();
	int pos = (int)((w - len) / 2);

	//for (int i = 0; i < pos; i++)
	//	cout << " ";

	Print(str);
}
