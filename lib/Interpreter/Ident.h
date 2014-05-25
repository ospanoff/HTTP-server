//
//  Ident.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Ident__
#define __Interpreter__Ident__

#include "lexems.h"

// this class describes Identifier (variable or func. name)
class Ident {
	char *name; // name of ID
	bool isFuncName;
//	bool declare;
	type_of_lex type; // type of ID
	bool assign;
	int value; // value of ID if int or bool
	double dvalue; // value of ID if number with floating point
	char *svalue; // val of ID if string
	int placeInPoliz;
public:
	Ident();
	void set_plInPoliz(int);
	int get_plInPoliz();
	void set_isFuncName();
	bool get_isFuncName();
	char *get_name();
	void put_name(const char *);
//	bool get_declare();
//	void put_declare();
	type_of_lex get_type();
	void put_type(type_of_lex);
//	bool get_assign();
//	void put_assign();
	int get_value();
	void put_value(int);
	double get_dvalue();
	void put_dvalue(double);
	char *get_svalue();
	void put_svalue(const char *);
};

#endif /* defined(__Interpreter__Ident__) */
