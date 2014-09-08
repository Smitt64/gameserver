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

void Server::loadSettings(const char const *settings) throw(std::exception)
{
	if (!pSettings)
		pSettings = new Parser();

	ifstream f;
	f.open(settings);
	if (!f.is_open()) {
		f.close();
		throw ios_base::failure("Can't open settings file ");
	}

	pSettings->attach(&f);
	pSettingsRootItem = pSettings->parse();

	f.close();
}
