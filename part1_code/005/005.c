#include "exp1.h"

int main(int argc, char** argv) {
	FILE* fp;
	char* p;
	char buf[1024];

	if(argc != 2) {
		printf("usage: %s [filename]\n", argv[0]);
		exit(-1);
	}

	fp = fopen(argv[1], "r");
	p = fgets(buf, 1024, fp);
	fputs(p, stdout);
	fclose(fp);

	return 0;
}
