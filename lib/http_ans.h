#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>
#include "Script_parser.h"

using namespace std;

void make_header(int f, int status, const char *title, const char *extra, const char *mime, long length, time_t date);
const char *get_mime_type(const char *name);
void send_error(int f, int status, const char *title, const char *extra, const char *text);
char *pars_script(int f, const char *path, struct stat *statbuf);
void send_file(int f, char *path, struct stat *statbuf);
char **make_env(char *command, struct sockaddr_in cl_addr);
void do_cgi(int f, char *path, char *command, struct sockaddr_in cl_addr);
void urldecode2(char *dst, const char *src);
void pars_post(int f, char *post_msg);
int answer_client(int f, char *buf, struct sockaddr_in cl_addr);