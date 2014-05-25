//
//  Ident.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Ident.h"

Ident::Ident()
{
	type = LEX_NULL;
//	assign = false;
	isFuncName = false;
	value = 0;
}

void Ident::set_plInPoliz(int i)
{
	placeInPoliz = i;
}

int Ident::get_plInPoliz()
{
	return placeInPoliz;
}

void Ident::set_isFuncName()
{
	isFuncName = true;
}

bool Ident::get_isFuncName()
{
	return isFuncName;
}

char *Ident::get_name()
{
	return name;
}

void Ident::put_name(const char *n)
{
	name = new char [strlen(n)+1];
	strcpy(name, n);
}

//bool Ident::get_declare()
//{
//	return declare;
//}

//void Ident::put_declare()
//{
//	declare = true;
//}

type_of_lex Ident::get_type()
{
	return type;
}

void Ident::put_type(type_of_lex t)
{
	type = t;
}

//bool Ident::get_assign()
//{
//	return assign;
//}
//
//void Ident::put_assign()
//{
//	assign = true;
//}

int Ident::get_value()
{
	return value;
}

void Ident::put_value(int v)
{
	value = v;
	delete [] svalue;
}

double Ident::get_dvalue()
{
	return dvalue;
}

void Ident::put_dvalue(double v)
{
	dvalue = v;
	delete [] svalue;
}

char *Ident::get_svalue()
{
	return svalue;
}

void Ident::put_svalue(const char *v)
{
	svalue = new char [strlen(v)+1];
	strcpy(svalue, v);
}