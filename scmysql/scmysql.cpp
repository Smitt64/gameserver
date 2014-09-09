// scmysql.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MySqlConnector.h"
#include <DBConnectionInterface.h>

__declspec(dllexport) DBConnectionInterface *instance()
{
	return new MySqlConnector();
}