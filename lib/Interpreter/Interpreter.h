//
//  Interpreter.h
//  Interpreter
//
//  Created by Ayat Ospanov on 19.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#ifndef __Interpreter__Interpreter__
#define __Interpreter__Interpreter__

#include "Executer.h"
#include "Parser.h"

class Interpreter {
	Executer E;
	Parser pars;
public:
	Interpreter(const char*program):pars(program) {}
	void interpretation();
};

#endif /* defined(__Interpreter__Interpreter__) */
