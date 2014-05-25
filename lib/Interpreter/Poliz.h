//
//  Poliz.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Poliz__
#define __Interpreter__Poliz__

#ifndef __Interpreter__Lex_H__
#define __Interpreter__Lex_H__
#include "Lex.h"
#endif

// this class realizes Reverse Polish notation
class Poliz {
	Lex *p;
	int size;
	int free; // next free position
public:
	Poliz(int);
	~Poliz();
	void put_lex(Lex);
	void put_lex(Lex, int);
	void blank(); // skip one cell
	int get_free();
	Lex &operator[](int);
	friend ostream &operator<<(ostream &, Poliz &);
};

#endif /* defined(__Interpreter__Poliz__) */
