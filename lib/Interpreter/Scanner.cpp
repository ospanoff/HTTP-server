//
//  Scanner.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Scanner.h"

Scanner::Scanner(const char *program)
{
	buf = new char [BUF_SIZE];
	if ((fp = fopen(program, "r")) == NULL) {
		char *s = new char [128];
		sprintf(s, "No such file: %s", program);
		throw s;
	}
		
	CS = H;
	clear();
	gc();
}

Scanner::~Scanner()
{
	delete [] buf;
	fclose(fp);
}

const char *Scanner::TW [] = { // Words
	"", "{", "}",
	"var", "^bool", "^string", "^int", "^double",
	"if", "then", "else", "do", "while", "for",
	"true", "false", "and", "or", "not",
	"function", "break", "continue", "return", "in",
	"typeof", NULL
};

type_of_lex Scanner::words [] = {
	LEX_NULL, LEX_BEGIN, LEX_END,
	LEX_VAR, LEX_BOOL, LEX_STR, LEX_INT, LEX_DOUBLE,
	LEX_IF, LEX_THEN, LEX_ELSE, LEX_DO, LEX_WHILE, LEX_FOR,
	LEX_TRUE, LEX_FALSE, LEX_AND, LEX_OR, LEX_NOT,
	LEX_FUNC, LEX_BREAK, LEX_CONT, LEX_RET, LEX_IN,
	LEX_TYPEOF, LEX_NULL
};

const char *Scanner::TD [] = { // Delims
	"", "EOF", ".",
	";", ",", "=", "(", ")", "[", "]", "==", "<",
	">", "+", "-", "*", "/", "<=", "!=", ">=", "===", "!==",
	"++", "+=", "--", "-=", "\"", "%", "%=", "*=", "/=",
	"+#", "&&", "||", "!", NULL
};

type_of_lex Scanner::dlms [] = {
	LEX_NULL, LEX_FIN, LEX_DOT,
	LEX_SEMICOLON, LEX_COMMA, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_LSQBR, LEX_RSQBR, LEX_EQ, LEX_LSS,
	LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_MULT, LEX_DIV, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_ABEQ, LEX_ABNEQ,
	LEX_PLPL, LEX_PLEQ, LEX_MINMIN, LEX_MINEQ, LEX_QUOT, LEX_MOD, LEX_MODEQ, LEX_MULEQ, LEX_DIVEQ,
	LEX_POSTPP, LEX_AND, LEX_OR, LEX_NOT, LEX_NULL
};

void Scanner::clear()
{
	buf_top = 0;
	for (int i = 0; i < BUF_SIZE; i++)
		buf[i] = '\0';
}

int Scanner::look(const char *buf, const char **list)
{
	int i = 0;
	while(list[i]) {
		if (!strcmp(buf, list[i]))
			return i;
		++i;
	}
	return 0;
}

Lex Scanner::get_lex()
{
	int d(0), j, dd(0), len(0);
	bool bs(0), sign(0);
	
	CS = H;
	do {
		switch(CS) {
			case H:
				if (c ==' ' || c == '\n' || c== '\r' || c == '\t')
					gc();
				else if (isalpha(c) || c == '{' || c == '}' || c == '_') {
					clear();
					add();
					gc();
					CS = IDENT;
				} else if (isdigit(c)) {
					d = c - '0';
					dd = 0;
					len = 0;
					gc();
					CS = NUMB;
				} else if (c == '/') {
					gc();
					if (c == '*') {
						CS = COM;
						gc();
					} else if (c == '=') { // /=
						gc();
						j = look("/=", TD);
						return Lex(LEX_DIVEQ, j);
					} else {
						c = '/';
						fseek(fp, -1, SEEK_CUR);
						CS = DELIM;
					}
				} else if (c == '=' || c == '<' || c == '>') {
					clear();
					add();
					gc();
					CS = ALE;
				} else if (c == EOF)
					return Lex(LEX_FIN, 0);
				else if (c == '!') {
					clear();
					add();
					gc();
					CS = NEQ;
				} else if (c == '+') {
					gc();
					if (c == '+') { // ++
						gc();
						j = look("++", TD);
						return Lex(LEX_PLPL, j);
					} else if (c == '=') { // +=
						gc();
						j = look("+=", TD);
						return Lex(LEX_PLEQ, j);
					} else {
						c = '+';
						fseek(fp, -1, SEEK_CUR);
						CS = DELIM;
					}
				} else if (c == '-') {
					gc();
					if (c == '-') { // --
						gc();
						j = look("--", TD);
						return Lex(LEX_MINMIN, j);
					} else if (c == '=') { // -=
						gc();
						j = look("-=", TD);
						return Lex(LEX_MINEQ, j);
					} else if (isdigit(c)) {
						sign = true;
						d = c - '0';
						dd = 0;
						len = 0;
						gc();
						CS = NUMB;
					} /*else {
						c = '-';
						fseek(fp, -1, SEEK_CUR);
						CS = DELIM;
					}*/
				} else if (c == '*') {
					gc();
					if (c == '=') { // *=
						gc();
						j = look("*=", TD);
						return Lex(LEX_MULEQ, j);
					} else {
						c = '*';
						fseek(fp, -1, SEEK_CUR);
						CS = DELIM;
					}
				} else if (c == '%') {
					gc();
					if (c == '=') { // %=
						gc();
						j = look("%=", TD);
						return Lex(LEX_MODEQ, j);
					} else {
						c = '%';
						fseek(fp, -1, SEEK_CUR);
						CS = DELIM;
					}
				} else if (c == '"') {
					clear();
					gc();
					CS = STR;
				} else
					CS = DELIM;
				break;
			case IDENT:
				if (isalpha(c) || isdigit(c) || c == '_') {
					add();
					gc();
				} else if ((j = look(buf, TW)))
					return Lex(words[j], j);
				else {
					j = TID.put(buf);
					return Lex(LEX_ID, j);
				}
				break;
			case NUMB:
				if (isdigit(c)) {
					d = d * 10 + (c - '0');
					gc();
				} else if (c == '.') {
					CS = DOUB;
					gc();
				} else {
					if (sign)
						d = -d;
					return Lex(LEX_NUM, d);
				}
				break;
			case DOUB:
				if (isdigit(c)) {
					dd = dd * 10 + (c - '0');
					len++;
					gc();
				} else {
					double num = dd / pow(10,len);
					num += d;
					if (sign)
						num = -num;
					return Lex(LEX_DOUBLE, 0, num);
				}
				break;
			case STR:
				if (c == EOF) {
					throw c;
				} else if (c == '\\') {
					if (bs) {
						bs = false;
						c = '\\';
						add();
					} else {
						bs = true;
					}
					gc();
				} else if (bs) {
					switch (c) {
						case '"':
							break;
						case '\\':
							c = '\\';
							break;
						case 'n':
							c = '\n';
							break;
						case '\'':
							c = '\'';
							break;
						default:
							throw c;
							break;
					}
					add();
					bs = false;
					gc();
				} else if (!bs && c == '"') {
					gc();
					return Lex(LEX_STR, 0, 0, buf);
				} else {
					add();
					gc();
				}
				break;
			case COM:
				if (c == '*') {
					gc();
					if (c == '/') {
						gc();
						CS = H;
					}
				} else if (c == EOF)
					throw c;
				else
					gc();
				break;
			case ALE:
				if (c == '=') {
					add();
					gc();
					if (c == '=') {
						add();
						gc();
					}
					j = look(buf, TD);
					return Lex(dlms[j], j);
				} else {
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}
				break;
			case NEQ:
				if (c == '=') {
					add();
					gc();
					if (c == '=') {
						add();
						gc();
					}
					j = look(buf, TD);
					return Lex(dlms[j], j);
				} else {
					j = look(buf, TD);
					return Lex(LEX_NOT, j);
				}
				break;
			case DELIM:
				clear();
				add();
				if (c == '&') {
					gc();
					add();
				}
				if (c == '|') {
					gc();
					add();
				}
				if ((j = look(buf, TD))) {
					gc();
					return Lex(dlms[j], j);
				} else
					throw c;
				break;
		} //end switch
	} while (true);
}