#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <csignal>
#include "Interpreter/Interpreter.h"

using namespace std;

class Script_parser
{
	char *f_name;
	char *p_name;
	fstream in;
	fstream prog;
public:
	Script_parser(const char *);
	~Script_parser();

	void pars();
};