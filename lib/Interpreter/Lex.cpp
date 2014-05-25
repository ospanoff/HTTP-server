//
//  Lex.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Lex.h"

Lex::Lex(type_of_lex t, int v, double dv, const char *sv):t_lex(t),v_lex(v),dv_lex(dv)
{
	if (sv) {
		sv_lex = new char [strlen(sv)+1];
		strcpy(sv_lex, sv);
	} else
		sv_lex = NULL;
}

type_of_lex Lex::get_type()
{
	return t_lex;
}

int Lex::get_value()
{
	return v_lex;
}

double Lex::get_dvalue()
{
	return dv_lex;
}

char *Lex::get_svalue()
{
	return sv_lex;
}

ostream &operator<<(ostream &s, Lex l)
{
	s << lex_names[l.t_lex];
//	l.sv_lex ? (s << l.sv_lex) : (s << l.v_lex);
//	s << " ";
	if (l.t_lex == LEX_ID && TID[l.v_lex].get_isFuncName())
		s << '(' << TID[l.v_lex].get_isFuncName() << ')';
	if (l.t_lex == LEX_ID || l.t_lex == POLIZ_ADDRESS || l.t_lex == POLIZ_LABEL)
		s << l.v_lex;
	s << " ";
//	s << endl;
	return s;
}