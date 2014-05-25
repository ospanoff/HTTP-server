//
//  lexems.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "lexems.h"

const char *lex_names[] = {
	"NULL",
	"{", "}",
	"var", "bool", "string", "int", "double",
	"if", "then", "else", "do", "while", "for",
	"true", "false", "&&", "||", "!",
	"function", "break", "continue", "return", "in",
	"EOF",
	";", ",", "=", "(", ")", "[", "]",
	"==", "<", ">", "<=", "!=", ">=", "===", "!==",
	"+", "-", "*", "/", "++", "--", "%",
	"+=", "-=", "%=", "*=", "/=",
	"\"", "+#", ".", "undefined",
	"typeof",
	"id", "num",
	"label", "address", "go", "fgo"
};