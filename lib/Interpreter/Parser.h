//
//  Parser.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Parser__
#define __Interpreter__Parser__

#include "Scanner.h"
#include "Poliz.h"

// this class realizes Syntax analyze
class Parser {
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
	bool isFuncName;
	bool funcReturn;
	bool inFunc;
	bool inCycle;
	int br_pl, con_pl;
	Scanner scan; // returns Lexemes
	// next functions, which realizes the grammar (look grammar.txt)
	void H();
	void A();
	void O();
	void V();
	void P();
	void B();
	void I();
	void F();
	void J();
	void E();
	void N();
	void N1();
	void T();
	void F1();
	void F2();
	void Nam();
	void TO();
	void gl() // getting lexemes from Scanner
	{
		curr_lex = scan.get_lex();
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
	}
public:
	Poliz prog;
	Parser(const char *program)
		:scan(program),prog(1000),br_pl(0),con_pl(0),
		isFuncName(false),funcReturn(false),inFunc(false),inCycle(false) {}
	void analyze();
};

#endif /* defined(__Interpreter__Parser__) */
