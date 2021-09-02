#include "exp1.h"

int main(int argc, char** argv) {
	int fd;
	int ret;
	char buf[4];

	if (argc != 2) {
		printf("usage: %s [filename]\n", argv[0]);
		exit(-1);
	}

	fd = open(argv[1], O_RDONLY);
	printf("file descriptor = %d\n", fd);
	ret = read(fd, buf, 4);
	printf("read %d bytes\n", ret);
	int size = write(1, buf, ret);
	if ( size == -1 ) {
		perror("error in write: ");
	}
	close(fd);

	return 0;
}
