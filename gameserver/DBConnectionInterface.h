#pragma once

#include "global.h"

class SERVER_CORE_API DBConnectionInterface
{
public:
	DBConnectionInterface();
	~DBConnectionInterface();

protected:
	virtual void init() = 0;
};

typedef DBConnectionInterface *(*DBConnectionInstance)();