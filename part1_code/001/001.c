#include "exp1.h"

int main(int argc, char** argv) {
	printf("The number of arguments is %d\n", argc);
	for(int i=0; i<argc; i++) {
		printf("argument %d is %s\n", i, argv[i]);
	}
	return 0;
}
