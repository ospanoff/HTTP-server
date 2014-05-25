//
//  lexems.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__lexems__
#define __Interpreter__lexems__

#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <vector>

enum type_of_lex {
	LEX_NULL,
	LEX_BEGIN, LEX_END,
	LEX_VAR, LEX_BOOL, LEX_STR, LEX_INT, LEX_DOUBLE,
	LEX_IF, LEX_THEN, LEX_ELSE, LEX_DO, LEX_WHILE, LEX_FOR,
	LEX_TRUE, LEX_FALSE, LEX_AND, LEX_OR, LEX_NOT,
	LEX_FUNC, LEX_BREAK, LEX_CONT, LEX_RET, LEX_IN,
	LEX_FIN,
	LEX_SEMICOLON, LEX_COMMA, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_LSQBR, LEX_RSQBR,
	LEX_EQ, LEX_LSS, LEX_GTR, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_ABEQ, LEX_ABNEQ,
	LEX_PLUS, LEX_MINUS, LEX_MULT, LEX_DIV, LEX_PLPL, LEX_MINMIN, LEX_MOD,
	LEX_PLEQ, LEX_MINEQ, LEX_MODEQ, LEX_MULEQ, LEX_DIVEQ,
	LEX_QUOT, LEX_POSTPP, LEX_DOT, LEX_UNDEF,
	LEX_TYPEOF,
	LEX_ID, LEX_NUM,
	POLIZ_LABEL, POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO
};

struct T {
	int i;
	double d;
	char *s;
	type_of_lex tl;
};

extern const char *lex_names[];

#endif /* defined(__Interpreter__lexems__) */
