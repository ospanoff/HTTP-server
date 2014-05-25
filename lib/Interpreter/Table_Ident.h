//
//  Table_Ident.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Table_Ident__
#define __Interpreter__Table_Ident__

#include "Ident.h"

// Table of IDs
class Table_ident {
	Ident *p;
	int size;
	int top;
public:
	Table_ident(int);
	~Table_ident();
	Ident &operator[](int);
	int put(const char *);
};

extern Table_ident TID;

#endif /* defined(__Interpreter__Table_Ident__) */
