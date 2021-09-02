#include "exp1.h"

int main(int argc, char** argv) {
	char buf[4];
	int ret;

	ret = read(0, buf, 4);
	printf("read %d bytes\n", ret);

	ret = write(1, buf, ret);
	printf("write %d bytes\n", ret);

	return 0;
}
