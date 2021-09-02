#include "../exp1.h"
#include "../exp1lib.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>

double get_current_timecount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec*1e-9;
}


double read_write(char* ip, char* filename, int buf_size){
		int sock;
		FILE* fp;
		int ret;
		//char* p;
		char buf[buf_size];

		sock = exp1_tcp_connect(ip, 11111);
    double t1 = get_current_timecount();//計測開始
		fp = fopen(filename, "r");

		ret = fread(buf,sizeof(char),buf_size,fp);
		while(ret > 0){
			//fwrite(buf,sizeof(char),ret,fp);
      write(sock,buf,ret);
			ret = fread(buf,sizeof(char),buf_size,fp);
		}
		//p = fgets(buf, 1024, stdin);
		//write(sock, p, strlen(p));
		close(sock);
		//return 0;
    double t2 = get_current_timecount();//計測終了
    return (t2-t1);
}

double recv_send(char* ip, char* filename, int buf_size){
    int sock;
    char buf[buf_size];
    FILE* fp;
    int ret;

    sock = exp1_tcp_connect(ip, 11111);

    double t1 = get_current_timecount();//計測開始

    fp = fopen(filename, "r");
    ret = fread(buf, sizeof(char), buf_size, fp);
		while(ret>0){
			send(sock, buf, ret, 0);
			ret = fread(buf, sizeof(char), buf_size, fp);
		}

    close(sock);



    double t2 = get_current_timecount();//計測終了

    return (t2-t1);
}

int main(int argc, char** argv) {
    int i;//index
    double result = 0;

    if(argc != 3) {
	  printf("usage: %s [ip address] [filename]\n", argv[0]);
	  exit(-1);
    }
    //
		printf("send_recieve\n");
		for(i = 1; i < 8000000; i = i * 2){
			result = recv_send(argv[1], argv[2], i);
			sleep(5);
			printf("%d,%f\n",i , result);
		}

    // printf("read_write\n");
    //   for(i = 1; i < 8000000; i = i * 2){
		//  	    result = read_write(argv[1],argv[2], i);
    //       sleep(5);
		//  	    printf("%d,%f\n",i , result);
    //   }


    return 0;
}
