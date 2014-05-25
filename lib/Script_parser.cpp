#include "Script_parser.h"

Script_parser::Script_parser(const char *f) {
	in.open(f, ifstream::in);

	f_name = new char [strlen(f)+32];
	sprintf(f_name, "%s.html", f);
	freopen(f_name, "w", stdout);
	// out.open(f_name, ofstream::out | ofstream::trunc);

	p_name = new char [strlen(f)+32];
	sprintf(p_name, "%s.mjs", f);
	prog.open(p_name, ofstream::out | ofstream::trunc);
}

Script_parser::~Script_parser() {
	in.close();
	prog.close();
	remove(p_name);
	delete [] f_name;
	delete [] p_name;
}

void Script_parser::pars() {
	string str;

	// copy everything before script
	while (!in.eof()) {
		in >> str;
		if (str == "<?")
			break;
		cout << str << ' ';
	}

	// copy script and execute
	while (!in.eof()) {
		in >> str;
		if (str == "?>")
			break;
		prog << str << ' ';
	}
	prog.close();

	try {
		Interpreter I(p_name);
		I.interpretation();
	}
	catch(char c) {
		cout << "unexpected symbol " << c << endl;
	}
	catch(Lex l) {
		cout << "unexpected lexeme " << l << endl;
	}
	catch(const char *source) {
		cout << source << endl;
	}

	// copy after script
	while (!in.eof()) {
		in >> str;
		cout << str << ' ';
	}
	in.close();
	fclose(stdout);
	stdout = fdopen(0, "w");
}