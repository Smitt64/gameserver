#include "Server.h"
#include "DBConnectionInterface.h"
#include "Parser.h"
#include "PItem.h"
#include <fstream>

using namespace std;

Server::Server() 
	: pSettings(NULL),
	pSettingsRootItem(NULL)
{
}


Server::~Server()
{
}

void Server::loadSettings(const char const *settings)
{
	if (!pSettings)
		pSettings = new Parser();

	ifstream f(settings);
	
	if (f.bad())
		throw ios_base::failure("Can't open settings file");

	pSettingsRootItem = pSettings->parse();
}
