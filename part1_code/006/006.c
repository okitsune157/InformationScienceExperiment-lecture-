#include "exp1.h"

int main(int argc, char** argv) {
	FILE* fpr;
	FILE* fpw;
	int ret;
	char buf[4];

	if(argc != 2) {
		printf("usage: %s [filename]\n", argv[0]);
		exit(-1);
	}

	fpr = fopen(argv[1], "r");
	fpw = fopen("tmp.txt", "w");

	ret = fread(buf, sizeof(char), 4, fpr);
	while (ret > 0) {
		fwrite(buf, sizeof(char), ret, fpw);
		ret = fread(buf, sizeof(char), 4, fpr);
	}
	fclose(fpr);
	fclose(fpw);

	return 0;
}
