#pragma once

#include <list>
#include <map>
#include <vector>
#include <string>

#include "global.h"

class Parser;
class SERVER_CORE_API PItem
{
	friend class Parser;
public:
	PItem(Parser *parser = NULL, PItem *parent = NULL);
	~PItem(void);

	inline void add_property(const std::string &name, const std::string &value);
	void set_name(const std::string &name);
	void add_child(PItem *child);

	std::string name() const{return _name;}

	PItem *find_child(const std::string &name);
	std::string &get_property(const std::string &name);

	PItem *parent() {return _parent;};

	void export_sql(std::ostream *stream, const int &parent_id = 1);

private:
	void parse() throw(int);
	std::string _name;

	std::map<std::string, std::string> _property;
	std::vector<PItem*> _childs;

	PItem *_parent;
	Parser *_p;

	static int counter;
};

