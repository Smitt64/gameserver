#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <DBConnectionInterface.h>

class MySqlConnector :
	public DBConnectionInterface
{
public:
	MySqlConnector();
	~MySqlConnector();

protected:
	void init();
};

#endif