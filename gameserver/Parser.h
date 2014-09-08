#pragma once

#include <iostream>
#include <list>
#include <conio.h>
#include <exception>

#include "global.h"

class SERVER_CORE_API PARSE_ERROR : public std::exception
{
public:
	virtual const char* what() const
	{
		return "Неверный формат данных";
	}
};

#ifdef QT_VERSION
class QIODevice;
#endif

class PItem;
class SERVER_CORE_API Parser
{
	friend class PItem;
public:
	Parser(void);
	~Parser(void);

#ifndef QT_VERSION
	void attach(std::istream *stream);
#elif
	void attach(QIODevice *stream);
#endif
	void add_escape_symbol(const char &ch);

	PItem *parse();

	void export_sql(std::ostream *stream);

private:
	inline int stream_pos();
	bool stream_eof();
	char read_symbol();
	void stream_seek(int pos);
	void read_symbol(char &ch);
#ifndef QT_VERSION
	void read_string(std::string &value);
#elif
	void read_string(QString &value);
#endif
	void escape_symbols();

	PItem *root;
	std::list<char> _escape;
#ifndef QT_VERSION
	std::istream *_stream;
#elif
	QIODevice *_stream;
#endif
};

