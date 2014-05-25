#include "http_ans.h"

#define SERVER "ospanoff/1.5"
#define PROTOCOL "HTTP/1.1"
#define TIME_FW "%a, %d %b %Y %H:%M:%S GMT"
#define ROOT_PATH "./root"
#define EXTEN ".html" // extension for cgi program output

void make_header(int f, int status, const char *title, const char *extra, const char *mime, int length, time_t date)
{
	time_t now;
	char timebuf[128];
	char buf[1024];

	sprintf(buf, "%s %d %s\r\n", PROTOCOL, status, title);
	send(f, buf, strlen(buf), 0);

	sprintf(buf, "Server: %s\r\n", SERVER);
	send(f, buf, strlen(buf), 0);
	
	now = time(NULL);
	strftime(timebuf, sizeof(timebuf), TIME_FW, gmtime(&now));
	
	sprintf(buf, "Date: %s\r\n", timebuf);
	send(f, buf, strlen(buf), 0);
	
	if (extra) {
		sprintf(buf, "%s\r\n", extra);
		send(f, buf, strlen(buf), 0);
	}
	if (mime) {
		sprintf(buf, "Content-Type: %s\r\n", mime);
		send(f, buf, strlen(buf), 0);
	}
	if (length >= 0) {
		sprintf(buf, "Content-Length: %d\r\n", length);
		send(f, buf, strlen(buf), 0);
	}
	if (date != -1) {
		strftime(timebuf, sizeof(timebuf), TIME_FW, gmtime(&date));
		sprintf(buf, "Last-Modified: %s\r\n", timebuf);
		send(f, buf, strlen(buf), 0);
	}
	sprintf(buf, "Connection: close\r\n");
	send(f, buf, strlen(buf), 0);
	
	sprintf(buf, "\r\n");
	send(f, buf, strlen(buf), 0);
}

const char *get_mime_type(const char *name)
{
	const char *ext = strrchr(name, '.');
	if (!ext) return NULL;
	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
	if (strcmp(ext, ".txt") == 0) return "text/plain";
	if (strcmp(ext, ".omjs") == 0) return "text/omjs";
	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
	if (strcmp(ext, ".gif") == 0) return "image/gif";
	if (strcmp(ext, ".png") == 0) return "image/png";
	if (strcmp(ext, ".ogg") == 0) return "audio/ogg";
	// if (strcmp(ext, ".wav") == 0) return "audio/wav";
	// if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
	// if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
	// if (strcmp(ext, ".mp4") == 0) return "video/mp4";
	// if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
	return NULL;
}

void send_error(int f, int status, const char *title, const char *extra, const char *text)
{
	char buf[1024];
	int size = 2 * strlen(title) + strlen(text) + 79;
	memset(buf, 0, sizeof(buf));
	make_header(f, status, title, extra, "text/html", size, -1);
	sprintf(buf, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\r\n", status, title);
	send(f, buf, strlen(buf), 0);
	
	sprintf(buf, "<BODY><H4>%d %s</H4>\r\n", status, title);
	send(f, buf, strlen(buf), 0);
	
	sprintf(buf, "%s\r\n", text);
	send(f, buf, strlen(buf), 0);
	
	sprintf(buf, "</BODY></HTML>\r\n");
	send(f, buf, strlen(buf), 0);
}

void send_file(int f, char *path, struct stat *statbuf)
{
	char data[1024];
	int n;
	bool isScript(false);
	memset(data, 0, sizeof(data));

	const char *m_type = get_mime_type(path); // mime type
	if (!strcmp(m_type,"text/html") || !strcmp(m_type,"text/plain") || !strcmp(m_type,"text/omjs")) {
		isScript = true;
		Script_parser parser(path);
		if (!strcmp(m_type,"text/omjs")) // if omjs, set omjs flag to true
			parser.setFlag();
		parser.pars();
		strcat(path, EXTEN);
		stat(path, statbuf);
	}

	FILE *file = fopen(path, "rb"); // open for reading in bin mode
	if (!file) { // if can't open
		perror("fopen");
		send_error(f, 403, "Forbidden", NULL, "Access denied.");
	} else {
		int length = S_ISREG(statbuf->st_mode) ? statbuf->st_size : -1; // size of file
		make_header(f, 200, "OK", NULL, get_mime_type(path), length, statbuf->st_mtime);
		while ((n = fread(data, 1, sizeof(data), file)) > 0) { // sending file
			if (send(f, data, n, 0) != n) { // if can't send all parts
				perror("send");
			}
		}
	}
	fclose(file);
	if (isScript) // if we executed script, delete secondary output file
		remove(path);
}

char **make_env(char *command, struct sockaddr_in cl_addr)
{
	char **env = new char* [4];
	if (command) {
		env[0] = new char [15+strlen(command)];
		sprintf(env[0], "QUERY_STRING=%s", command);
	} else {
		env[0] = new char [15];
		sprintf(env[0], "QUERY_STRING=");
	}

	env[1] = new char [15+strlen(inet_ntoa(cl_addr.sin_addr))];
	sprintf(env[1], "REMOTE_ADDR=%s", inet_ntoa(cl_addr.sin_addr));
	
	env[2] = new char [20];
	sprintf(env[2], "REMOTE_PORT=%d", ntohs(cl_addr.sin_port));

	env[3] = NULL;

	return env;
}

void do_cgi(int f, char *path, char *command, struct sockaddr_in cl_addr)
{
	struct stat statbuf;
	char out[strlen(path)+5]; // output file of proc
	
	sigset_t sigmask;
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGCHLD);

	sprintf(out, "%s%s", path, EXTEN); // init of the name of out file

	char **env = make_env(command, cl_addr);
	
	pid_t pid = fork();
	if (pid < 0) {
		perror("pid");
		exit(1);
	} else if (pid == 0) {
		// blocking sigchild because grandpa catches pid1's end, but not wait()
		sigprocmask(SIG_BLOCK, &sigmask, 0);

		pid_t pid1 = fork();
		if (pid1 < 0) {
			perror("pid");
			exit(1);
		} else if (pid1 == 0) {
			char **argv = new char* [2];
			argv[0] = path;
			argv[1] = NULL;
			
			FILE *fn = fopen(out, "w");
			dup2(fileno(fn), 1);
			dup2(fileno(fn), 2);
			fclose(fn);

			// setsid();
			execve(path, argv, env);
			perror("Exec error: ");
			exit(1);
		}
		wait(NULL);
		sigprocmask(SIG_UNBLOCK, &sigmask, 0);
		
		if (stat(out, &statbuf) < 0)
			perror("stat");

		send_file(f, out, &statbuf);
		exit(0); // zombie killed! =)
	}
	remove(out);
}

int answer_client(int f, char *inf, struct sockaddr_in cl_addr)
{
	char *method;
	char *uri;
	char *protocol;
	char *post_msg;
	struct stat statbuf;
	char *buf;
	char path[1024];
	int len;

	if (inf[strlen(inf)-4] == '\r') inf[strlen(inf)-4] = 0; // if no msg body from client (not POST)
	post_msg = strrchr(inf, '\n'); // prev line was done for doing this
	post_msg++; // delete '\n'

	buf = strtok(inf, "\r\n"); // cutting first line

	method = strtok(buf, " "); // cutting first word
	uri = strtok(NULL, " "); // cutting second word
	protocol = strtok(NULL, "\r"); // cutting third word
	if (!method || !uri || !protocol) return -1;
	
	char *f_path = strtok(uri, "?&\0"); // cutting first word from request
	sprintf(path, "%s%s", ROOT_PATH, f_path);
	char *command = strtok(NULL, "\0");
	
	if (!strcmp(method, "POST")) { // if post, just send request
		make_header(f, 200, "OK", NULL, "text/html", strlen(post_msg), -1);
		send(f, post_msg, strlen(post_msg), 0);
	
	} else if (strcmp(method, "GET") && strcmp(method, "HEAD")) // if not GET or not HEAD send error
		send_error(f, 501, "Not supported", "Allow: GET, HEAD", "Method is not supported.");
	
	else if (stat(path, &statbuf) < 0)
		send_error(f, 404, "Not Found", NULL, "File not found.");
	
	else if (S_ISDIR(statbuf.st_mode)) { // if path is dir, open index.html
		len = strlen(path);
		if (path[len - 1] != '/') {
			sprintf(path, "%s/", path);
		}
		sprintf(path, "%sindex.html", path);
		if (stat(path, &statbuf) >= 0) {
			send_file(f, path, &statbuf);
		} else {
			send_error(f, 404, "Not Found", NULL, "File not found.");
		}
	
	} else { // if it is file
		if (!strncmp(uri, "/cgi-bin", 8)) // if we try to run executable file
			do_cgi(f, path, command, cl_addr);
		else {
			send_file(f, path, &statbuf); // or just send file
		}
	}

	return 0;
}