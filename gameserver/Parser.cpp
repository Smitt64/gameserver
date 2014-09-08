#include "StdAfx.h"
#include "Parser.h"
#include <algorithm>
#include "PItem.h"

using namespace std;

#define TAB '\t'

Parser::Parser(void)
	:root(NULL)
{
	add_escape_symbol(32);
	add_escape_symbol('\r');
	add_escape_symbol('\n');
	add_escape_symbol(TAB);
	//add_escape_symbol('}');
}


Parser::~Parser(void)
{
	if (root)
		delete root;

	_escape.clear();
}

void Parser::add_escape_symbol(const char &ch)
{
	_escape.push_back(ch);
}

#ifndef QT_VERSION
void Parser::attach(std::istream *stream)
{
	if (!stream->good())
		return;
#elif
void Parser::attach(QIODevice *stream)
{
	if (!stream->isOpen())
		return;
#endif
	_stream = stream;
#ifndef QT_VERSION
	_stream->seekg(0, ios::beg);
#elif
	_stream->seek(0);
#endif

}

void Parser::escape_symbols()
{
	char ch = 0;

	list<char>::iterator f;

	do {
		ch = read_symbol();
		if (ch == '#') {
			do {
				ch = read_symbol();
			} while (ch != '\n');
		}
		f = find(_escape.begin(), _escape.end(), ch);
	}while (f != _escape.end() /*|| _stream->eof()*/);

	//вернуться на символ назад
#ifndef QT_VERSION
	_stream->seekg(stream_pos() - 1, ios::beg);
#elif
	_stream->seek(stream_pos() - 1);
#endif
	
}

int Parser::stream_pos()
{
#ifndef QT_VERSION
	return (int)_stream->tellg();
#elif
	return (int)_stream->pos();
#endif
}

char Parser::read_symbol()
{
	char ch = 0;
#ifndef QT_VERSION
	_stream->read(&ch, sizeof(char));
#elif
	_stream->getChar(&ch);
#endif

	return ch;
}

void Parser::read_symbol(char &ch)
{
#ifndef QT_VERSION
	_stream->read(&ch, sizeof(char));
#elif
	_stream->getChar(&ch);
#endif
}

bool Parser::stream_eof()
{
#ifndef QT_VERSION
	return _stream->eof();
#elif
	return _stream->atEnd();
#endif
}

void Parser::stream_seek(int pos)
{
#ifndef QT_VERSION
	_stream->seekg(pos, ios::beg);
#endif
}

#ifndef QT_VERSION
	void Parser::read_string(std::string &value)
#elif
	void Parser::read_string(QString &value)
#endif
{
	char ch = 0, pch = 0;
	list<char>::iterator f;
	while (!_stream->eof())
	{
		ch = read_symbol();
		f = find(_escape.begin(), _escape.end(), ch);

		if (f != _escape.end())
			break;

		if (ch == '=')
		{
#ifndef QT_VERSION
			_stream->seekg(stream_pos() - 1, ios::beg);
#elif
#endif
			break;
		}

		if (ch == '"' && pch != '\\')
			break;

		if (ch == '\\') {
			if (pch == '\\')
				value += '\\';

			pch = ch;
			continue;
		}

		if (ch == 't' && pch == '\\') {
			value += '\t';
			pch = ch;
			continue;
		}
		value += ch;
		pch = ch;
	}

#ifndef QT_VERSION
	if (_stream->eof())
#endif
		throw PARSE_ERROR();
}

PItem *Parser::parse()
{
	try {
		//пропустить ненужные символы перед первым узлом
		escape_symbols();
#ifndef QT_VERSION
		string str;
#elif
		QString str;
#endif
		read_string(str);

		bool f = false;
		char ch = read_symbol();

		if (ch == '=')
			f = true;
		
		escape_symbols();

		if (read_symbol() != '{' || !f)
			throw PARSE_ERROR();

		root = new PItem(this);
		root->set_name(str);
		root->parse();
	} catch(std::exception &e) {
		cout << e.what() << endl; 
		if (root) {
			delete root;
			root = NULL;
		}
	}

	return root;
}


void Parser::export_sql(std::ostream *stream)
{
	*stream << "create table tree_table (" << endl << TAB;
	*stream << "node_id number not null primary key," << endl << TAB;
	*stream << "parent_id number references tree_table( node_id )," << endl << TAB;
	*stream << "node_name varchar2(100)," << endl << TAB;
	*stream << "node_value varchar2(1000)" << endl;
	*stream << ");" << endl;

	*stream << endl << endl;

	if (!root)
		return;

	root->export_sql(stream);
}