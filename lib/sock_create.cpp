#include "sock_create.h"

int sock_create(int argc, char **argv) {
	int port, listener, n;
	struct sockaddr_in addr;

	if (argc != 2) {
		printf("Input port number\n");
		exit(1);
	}
	if (sscanf(argv[1], "%d%n", &port, &n) != 1|| argv[1][n] || port <= 1023 || port > 65535) { // converting port from char to int
		fprintf(stderr, "Bad port number: %s\n", argv[1]);
		exit(1);
	}

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("socket");
		exit(1);
	}

	int opt = 1;
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(1);
	}
	return listener;
}