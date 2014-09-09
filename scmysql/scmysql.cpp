// scmysql.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MySqlConnector.h"
#include <DBConnectionInterface.h>

extern "C"{
	__declspec(dllexport) DBConnectionInterface  * CALLBACK instance()
	{
		return new MySqlConnector();
	}
}