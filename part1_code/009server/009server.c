#include "exp1.h"
#include "exp1lib.h"

int main(int argc, char** argv) {
	int sock_listen;
	int sock_client;
	struct sockaddr addr;
	int len;
	int ret;
	char buf[1024];
	FILE* fp;

	sock_listen = exp1_tcp_listen(11111);
	sock_client = accept(sock_listen, &addr, (socklen_t*) &len);

	fp = fopen("tmp.txt", "w");
	ret = recv(sock_client, buf, 1024, 0);
	while (ret > 0) {
		fwrite(buf, sizeof(char), ret, fp);
		ret = recv(sock_client, buf, 1024, 0);
	}
	close(sock_client);
	close(sock_listen);

	return 0;
}

