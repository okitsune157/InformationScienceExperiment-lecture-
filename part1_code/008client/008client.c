#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
	int sock;
	char* p;
	char buf[1024];

	if(argc != 2) {
		printf("usage: %s [ip address]\n", argv[0]);
		exit(-1);
	}

	sock = exp1_tcp_connect(argv[1], 11111);
	p = fgets(buf, 1024, stdin);
	send(sock, p, strlen(p), 0);
	close(sock);

	return 0;
}

