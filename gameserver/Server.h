#ifndef SERVER_H
#define SERVER_H

#include "global.h"

class DBConnectionInterface;
class Parser;
class PItem;
class SERVER_CORE_API Server
{
public:
	Server();
	~Server();

	void loadSettings(const char const *settings) throw(std::exception);

	void init() throw(std::exception);

private:
	DBConnectionInterface *pConnection;
	Parser *pSettings;
	PItem *pSettingsRootItem;

	void *hMod;
};

#endif