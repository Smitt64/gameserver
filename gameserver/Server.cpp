#include "Server.h"
#include "DBConnectionInterface.h"
#include "Parser.h"
#include "PItem.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

using namespace std;

Server::Server() 
	: pSettings(NULL),
	pSettingsRootItem(NULL),
	hMod(NULL)
{
}


Server::~Server()
{
	FreeLibrary((HMODULE)hMod);
}

void Server::loadSettings(const char const *settings) throw(std::exception)
{
	if (!pSettings)
		pSettings = new Parser();

	ifstream f;
	f.open(settings);
	if (!f.is_open()) {
		f.close();
		throw ios_base::failure("Can't load settings file");
	}

	pSettings->attach(&f);
	pSettingsRootItem = pSettings->parse();

	f.close();
}

void Server::init() throw(std::exception)
{
	DBConnectionInstance func = NULL;

	if (!pSettingsRootItem)
		throw std::logic_error("Settings file not loaded");

	PItem *sql_conn = pSettingsRootItem->find_child("sqlcon");

	hMod = (HMODULE)LoadLibrary(sql_conn->get_property("driver").c_str());

	if (!hMod)
		throw std::logic_error("Can't load sql driver");

	func = (DBConnectionInstance)GetProcAddress((HMODULE)hMod, "_instance@0");

	if (!func)
		throw std::logic_error("Can't create sql driver instance");

	pConnection = func();

	pConnection->init();
	std::cout << "SQL driver inited" << std::endl;
}