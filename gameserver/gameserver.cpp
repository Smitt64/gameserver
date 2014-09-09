// gameserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "global.h"
#include "Server.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	Server core;
	try
	{
		core.loadSettings("settings.txt");
		core.init();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what();
	}

	getchar();
	return 0;
}

