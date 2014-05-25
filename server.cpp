// Server @ version 1.6, name .ospanoff
// copyright (c) .ospanoff
// Ayat ".ospanoff" Ospanov
// CMC, KB MSU, 2013 - 2014

#include <set>
#include <map>
#include <algorithm>
// #include <fcntl.h>

#include "lib/sock_create.h"
#include "lib/http_ans.h"

#define MAX_CLIENT 5

int listener;

using namespace std;

void sig_chld(int o)
{
	cout << "Child: ";
	cout << waitpid(0, NULL, WNOHANG) << '\n';
	fflush(stdout);
}

void sig_int(int o)
{
	cout << "\nServer turned off";
	fflush(stdout);
	shutdown(listener, 2);
	close(listener);
	signal(SIGINT, SIG_DFL);
	raise(SIGINT);
}

int main(int argc, char **argv)
{
	signal(SIGCHLD, sig_chld);
	signal(SIGINT, sig_int);

	int sock;
	int bytes_read = 1024;
	int max_fd;
	struct sockaddr_in cl_ad;
	fd_set fd_s;

	// creating socket
	listener = sock_create(argc, argv);
	// --endof creating socket

	// listening
	if (listen(listener, MAX_CLIENT) < 0) {
		perror("listen");
	}
	// --endof listening

	set <int> clients; // here is clients descriptors
	clients.clear();

	map <int, struct sockaddr_in> cl_addr;
	cl_addr.clear();

	// setting up blocking signals
	sigset_t sigmask;
	sigemptyset (&sigmask);
	sigaddset (&sigmask, SIGCHLD);
	// --endof setting up 

	// receiving data from clients
	while (7) {
		// setting up select
		FD_ZERO(&fd_s);
		FD_SET(listener, &fd_s);
		for (set<int>::iterator i = clients.begin(); i != clients.end(); i++) {
			FD_SET(*i, &fd_s);
		}
		max_fd = max(listener, *max_element(clients.begin(), clients.end()));
		
		sigprocmask (SIG_BLOCK, &sigmask, 0);
		if (select(max_fd+1, &fd_s, NULL, NULL, NULL) <= 0) {
			perror("select");
			exit(1);
		}
		sigprocmask (SIG_UNBLOCK, &sigmask, 0);
		// endof

		// connecting and reading data
		if (FD_ISSET(listener, &fd_s)) { // if new client, accepting
			unsigned int len = sizeof(cl_ad);
			sock = accept(listener, (struct sockaddr *) &cl_ad, &len);
			if (sock < 0) {
				perror("accept");
				exit(1);
			}
			// fcntl(sock, F_SETFL, O_NONBLOCK);
			clients.insert(sock);
			cl_addr[sock] = cl_ad;
		}
		for (set<int>::iterator i = clients.begin(); i != clients.end(); i++) {
			if (FD_ISSET(*i, &fd_s)) { // looking for ready client
				char buf[1024];
				// memset(buf, 0, bytes_read);
				bytes_read = recv(*i, buf, sizeof(buf), 0); //buf.size();
				
				if (bytes_read == 0) { // if client closed sock
					close(*i);
					cl_addr.erase(cl_addr.find(*i));
					clients.erase(*i);
					if (clients.size() == 0)
						break;
				} else { // if we recieved inf
					// cout << buf;
					answer_client(*i, buf, cl_addr[*i]);
				}
			}
		}
	}
	shutdown(listener, 2);
	close(listener);
	return 0;
}