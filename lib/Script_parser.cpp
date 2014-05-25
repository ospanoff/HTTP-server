#include "Script_parser.h"

Script_parser::Script_parser(const char *f)
{
	inf_name = new char [strlen(f)+1];
	sprintf(inf_name, "%s", f);
	inp.open(f, ifstream::in);

	f_name = new char [strlen(f)+32];
	sprintf(f_name, "%s.html", f);
	outp.open(f_name, ofstream::out | ofstream::trunc);

	p_name = new char [strlen(f)+32];
	sprintf(p_name, "%s.mjs", f);
	prog.open(p_name, ofstream::out | ofstream::trunc);
}

Script_parser::~Script_parser()
{
	inp.close();
	prog.close();
	outp.close();
	
	if (is_omjs)
		remove(strcat(p_name, ".mjs"));
	else
		remove(p_name);
	
	delete [] f_name;
	delete [] p_name;
}

void Script_parser::setFlag()
{
	is_omjs = true;
}

void Script_parser::pars()
{
	string str;

	if (is_omjs) {
		p_name = inf_name;
		inp.close();
	} else {
		// copy everything before script
		while (!inp.eof()) {
			inp >> str;
			if (str == "<?")
				break;
			outp << str << ' ';
		}

		// copy script and execute
		while (!inp.eof()) {
			inp >> str;
			if (str == "?>")
				break;
			prog << str << ' ';
		}
		prog.close();
		outp.close();
	}

	sigset_t sigmask;
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGCHLD);
	
	sigprocmask(SIG_BLOCK, &sigmask, 0);

	pid_t pid1 = fork();
	if (pid1 < 0) {
		perror("pid");
		exit(1);
	} else if (pid1 == 0) {
		char **argv = new char* [3];
		argv[0] = new char [strlen("./lib/Interpreter")+1];
		strcpy(argv[0], "./lib/Interpreter");
		argv[1] = p_name;
		argv[2] = NULL;

		FILE *fn = fopen(f_name, "a");
		dup2(fileno(fn), 1);
		dup2(fileno(fn), 2);
		fclose(fn);

		// setsid();
		execv(argv[0], argv);
		perror("Exec error: ");
		exit(1);
	}
	wait(NULL);
	sigprocmask(SIG_UNBLOCK, &sigmask, 0);

	if (is_omjs)
		return;

	outp.open(f_name, ifstream::app);
	// copy after script
	while (!inp.eof()) {
		inp >> str;
		outp << str << ' ';
	}
	inp.close();
	outp.close();
}