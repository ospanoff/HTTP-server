//
//  Scanner.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Scanner__
#define __Interpreter__Scanner__

#ifndef __Interpreter__Lex_H__
#define __Interpreter__Lex_H__
#include "Lex.h"
#endif

// this class realizes Lexical analyze
class Scanner {
	enum state { H, IDENT, NUMB, DOUB, STR, COM, ALE, DELIM, NEQ };
	static const char *TW [];
	static type_of_lex words [];
	static const char *TD [];
	static type_of_lex dlms [];
	static const int BUF_SIZE = 100;
	state CS;
	FILE *fp;
	char c;
	char *buf; // for keeping string or ID name
	int buf_top;
	void clear();
	void add() { buf[buf_top++] = c; }
	int look(const char *, const char **);
	void gc() { c = fgetc(fp); }
public:
	Scanner(const char *);
	~Scanner();
	Lex get_lex();
};

#endif /* defined(__Interpreter__Scanner__) */
