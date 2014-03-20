#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

void make_header(int f, int status, const char *title, const char *extra, const char *mime, int length, time_t date);
const char *get_mime_type(const char *name);
void send_error(int f, int status, const char *title, const char *extra, const char *text);
void send_file(int f, const char *path, struct stat *statbuf);
char **make_env(char *command, struct sockaddr_in cl_addr);
void do_cgi(int f, char *path, char *command, struct sockaddr_in cl_addr);
int answer_client(int f, char *buf, struct sockaddr_in cl_addr);