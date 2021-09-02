#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
	int sock;
	char buf[1024];
	FILE* fp;
	int ret;

	if(argc != 3) {
		printf("usage: %s [ip address] [filename]\n", argv[0]);
		exit(-1);
	}

	sock = exp1_tcp_connect(argv[1], 11111);
	fp = fopen(argv[2], "r");
	ret = fread(buf, sizeof(char), 1024, fp);
	while(ret > 0) {
		send(sock, buf, ret, 0);
		ret = fread(buf, sizeof(char), 1024, fp);
	}
	close(sock);

	return 0;
}

