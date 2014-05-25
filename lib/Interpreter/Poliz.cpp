//
//  Poliz.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Poliz.h"

Poliz::Poliz(int max_size)
{
	p = new Lex[size = max_size];
	free = 0;
}

Poliz::~Poliz()
{
	delete [] p;
}

void Poliz::put_lex(Lex l)
{
	p[free++] = l;
}

void Poliz::put_lex(Lex l, int place)
{
	p[place] = l;
}

void Poliz::blank()
{
	free++;
}

int Poliz::get_free()
{
	return free;
}

Lex &Poliz::operator[](int index)
{
	if (index > size)
		throw "POLIZ: out of array";
	else if (index > free)
		throw "POLIZ: indefinite element of array";
	else
		return p[index];
}

ostream &operator<<(ostream &s, Poliz &p)
{
	for (int i = 0; i < p.get_free(); i++) {
		s << p[i];
	}
	return s;
}