#include "StdAfx.h"
#include "PItem.h"
#include "Parser.h"
#include <algorithm>

using namespace std;

int PItem::counter = 0;

PItem::PItem(Parser *parser, PItem *parent)
	:_p(parser),
	_parent(parent)
{
}


PItem::~PItem(void)
{
	_property.clear();

	for_each(_childs.begin(), _childs.end(), [](PItem *item)
	{
		delete item;
	});

	_childs.clear();
}

void PItem::add_property(const std::string &name, const std::string &value)
{
	_property[name] = value;
}

void PItem::add_child(PItem *child)
{
	_childs.push_back(child);
}

void PItem::set_name(const string &name)
{
	_name = name;
}

string &PItem::get_property(const string &name)
{
	return _property[name];
}

PItem *PItem::find_child(const std::string &name)
{
	int k;
	vector<PItem*>::iterator iter = find_if(_childs.begin(), _childs.end(), [name](PItem *item)
	{
		return (item->_name == name);
	});
	
	return (PItem*)(*iter);
}

void PItem::export_sql(ostream *stream, const int &parent_id)
{
	int cur = (++ PItem::counter);
	int _id = parent_id;

	*stream << "insert into tree_table(node_id,parent_id,node_name,node_value) values(" 
			<< PItem::counter << "," << _id << "," << "'" << _name << "'" << ",'" << "" << "'"
 			<< ");"
			<< endl;

	for_each(_property.begin(), _property.end(), [&stream, cur](pair<std::string, std::string> p)
	{
		++ PItem::counter;
		*stream << "insert into tree_table(node_id,parent_id,node_name,node_value) values(" 
			<< PItem::counter << "," << cur << "," << "'" << p.first << "'" << ",'" << p.second << "'"
 			<< ");"
			<< endl;
	});
	
	for_each(_childs.begin(), _childs.end(), [&stream, cur](PItem *item)
	{
		item->export_sql(stream, cur);
	});
}

void PItem::parse() throw(int)
{
	char ch = 0;
	string str, str2;

	while(!_p->stream_eof()) {
		_p->escape_symbols();
		//запомнить текущую позицию в потоке
		int pos = _p->stream_pos();

		ch = _p->read_symbol();

		if (ch == '}')
			return;

		_p->stream_seek(_p->stream_pos() - 1);
		_p->read_string(str);//прочитать имя
		_p->escape_symbols();

		if (_p->read_symbol() != '=') // если не символ =, завершить разбор
			throw PARSE_ERROR();

		_p->escape_symbols();

		ch = _p->read_symbol();

		if (ch == '"')
		{
			_p->read_string(str2);
			//str2.pop_back();
			add_property(str, str2);
		}  else if (ch == '{')
		{
			PItem *item = new PItem(_p, this);
			item->set_name(str);
			add_child(item);
			item->parse();
		} else {
			throw PARSE_ERROR();
		}

		str = "";
		str2 = "";
	}
}