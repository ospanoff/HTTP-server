#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <csignal>

using namespace std;

class Script_parser
{
	bool is_omjs; // Is file OMJS(ospanoff's model JS) file?
	char *inf_name; // Input file name
	char *f_name; // Output file name
	char *p_name; // Output file for program code
	fstream inp; // input stream: from inf_name
	fstream outp; // output stream: to f_name
	fstream prog; // output stream: to p_name
public:
	Script_parser(const char *);
	~Script_parser();

	void setFlag();
	void pars();
	void execute_program();
};