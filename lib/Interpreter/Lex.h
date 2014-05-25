//
//  Lex.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Lex__
#define __Interpreter__Lex__

#include "Table_Ident.h"

using namespace std;

//this class describes Lexemes
class Lex {
	type_of_lex t_lex;
	int v_lex;
	char *sv_lex; // value of Lex if string
	double dv_lex;
public:
	Lex(type_of_lex t = LEX_NULL, int v = 0, double dv = 0, const char *sv = NULL);
	type_of_lex get_type();
	int get_value();
	double get_dvalue();
	char *get_svalue();
	friend ostream &operator<<(ostream &, Lex);
};

#endif /* defined(__Interpreter__Lex__) */
