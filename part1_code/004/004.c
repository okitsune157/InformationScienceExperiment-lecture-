#include "exp1.h"

int main(int argc, char** argv) {
	FILE* fp;
	int ret;
	char buf[1024];

	if(argc != 2) {
		printf("usage: %s [filename]\n", argv[0]);
		exit(-1);
	}

	fp = fopen(argv[1], "r");
	ret = fread(buf, sizeof(char), 1024, fp);
	fwrite(buf, sizeof(char), ret, stdout);
	fclose(fp);

	return 0;
}
