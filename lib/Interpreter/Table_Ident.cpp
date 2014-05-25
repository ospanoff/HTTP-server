//
//  Table_Ident.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Table_Ident.h"

Table_ident::Table_ident(int max_size)
{
	p = new Ident[size = max_size];
	top = 1;
}
Table_ident::~Table_ident()
{
	delete [] p;
}

Ident &Table_ident::operator[](int k)
{
	return p[k];
}

int Table_ident::put(const char *buf)
{
	for(int j = 1; j < top; j++)
		if(!strcmp(buf, p[j].get_name()))
			return j;
	p[top].put_name(buf);
	++top;
	return top-1;
}

Table_ident TID(100);