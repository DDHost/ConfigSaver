#include "common.h"
#include "Sockets/Telnet.h"
#include "Files/inifile.h"
#include "Files/Files.h"
#include <future>
#include <typeinfo>
#include <Windows.h>

#pragma once

class Main
{
private:
	const char* menu_TITLE = // ConfigSaver
		"\n\n"
		"		####################################################\n"
		"		#   ___           __ _      ___                    #\n"
		"		#  / __|___ _ _  / _(_)__ _/ __| __ ___ _____ _ _  #\n"
		"		# | (__/ _ \\ ' \\|  _| / _` \\__ \\/ _` \\ V / -_) '_| #\n"
		"		#  \\___\\___/_||_|_| |_\\__, |___/\\__,_|\\_/\\___|_|   #\n"
		"		#                     |___/                        #\n"
		"		####################################################\n"
		"\n\n";
public:
	Printers printer;
	
	void Console(int w, int h)
	{
		HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r); //stores the console's current size
		MoveWindow(console, r.left, r.top, w, h, TRUE); // set the width and height
		SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	}

	const char* GETMENUTITLE()
	{
		return menu_TITLE;
	}
};