//
//  Executer.h
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Executer__
#define __Interpreter__Executer__

#include "Parser.h"
#include "Stack.h"

class Executer {
	Lex pc_el;
	T func_args[10000];
	Stack <100000> args;
public:
	void execute(Poliz &);
};

#endif /* defined(__Interpreter__Executer__) */
