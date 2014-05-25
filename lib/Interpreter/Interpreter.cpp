//
//  Interpreter.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 19.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Interpreter.h"

void Interpreter::interpretation()
{
	pars.analyze();
	E.execute(pars.prog);
}