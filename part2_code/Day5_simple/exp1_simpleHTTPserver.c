// #include "../exp1.h"
#include "exp1lib.h"
#include "exp1.h"
#include <unistd.h>//403


typedef struct //タグ名省略
{
  char cmd[64];//HTTPrequest内のHTTPメソッド：GETやPOSTなど
  char path[256];//HTTPリクエスト内のURI情報用
  char real_path[256];//request対象の実体へのパス情報用
  char type[64];//HTTPリプライにおけるCpntent-Typeフィールド用：test/htmlやimage/jpegなど
  int code;//HTTPリプライにおけるHTTPステータスコード
  int size;//HTTPリプライにおけるContent-Lengthフィールド用
} exp1_info_type;//データ型

void exp1_parse_status(char* status, exp1_info_type *pinfo)
{
  char cmd[1024];
  char path[1024];
  char* pext;
  int i, j;

  enum state_type//列挙型、exp1_parse_headerと同じ
  {
    SEARCH_CMD,
    SEARCH_PATH,
    SEARCH_END
  }state;

  state = SEARCH_CMD;
  j = 0;
  for(i = 0; i < strlen(status); i++){//statusにはheaderで読み込んだ1行の情報が入ってる
    switch(state){
    case SEARCH_CMD:
      if(status[i] == ' '){
        cmd[j] = '\0';
        j = 0;
        state = SEARCH_PATH;
      }else{
        cmd[j] = status[i];//statusのうち、HTTPメソッド情報をcmdに
        j++;
      }
      break;
    case SEARCH_PATH:
      if(status[i] == ' '){
        path[j] = '\0';
        j = 0;
        state = SEARCH_END;
      }else{
        path[j] = status[i];//statusのうち、URI情報をpathに
        j++;
      }
      break;
    }
  }

  strcpy(pinfo->cmd, cmd);//構造体の同名変数に値をコピー？
  strcpy(pinfo->path, path);
  //printf("path:%s",path);
  //path:/401test.html
}

void exp1_check_file(exp1_info_type *info)
{
  struct stat s;//stat()の返り値がstat構造体
  int ret;
  char* pext;

  sprintf(info->real_path, "html%s", info->path);//real_pathにhtmlpathの情報を…？
  //printf("real_path:%s\n",info->real_path);
  //real_path:html/401test.html
  ret = stat(info->real_path, &s);//成功0、失敗-1 pathname,buf



  if((s.st_mode & S_IFMT) == S_IFDIR){
    //st_mode(アクセス保護)とS_IFMT(S_IFMT	0170000	ファイル種別を示すビット領域を表すビットマスク)がS_IFDIR	0040000	ディレクトリの時
    //このif文の書きかたはMANにある通常のファイルかどうかを検査する書きかた
    sprintf(info->real_path, "%s/index.html", info->real_path);//real_pathにreal_pathを・・?
  }

  ret = stat(info->real_path, &s);
  
  //  if (access (info->path, (R_OK | X_OK)))
  
  // printf("403:%d\n",access (info->real_path, (R_OK | X_OK)));
  // printf("403r:%d\n",access (info->real_path, R_OK));
  // printf("403w:%d\n",access (info->real_path,  X_OK));

  //access
  // ファイルが存在し、読み込み権限および実行権限があるかを確認
  // 権限の確認がすべてOKに場合には、0 がリターンされる
  // 権限がない、またはその他のエラーの場合は -1 がリターンされる
  //char cmp[200]={'/','t','e','a','p','o','t','t','e','s','t','.','h','t','m','l','\0'};
  //char cmp[] = "/teapottest.html";
  //char basic[] = "html/";

  char cmp[] = "/teapottest.html";
  if(strcmp(info->path,"/old.html") == 0){
	info->code = 301;
  }else if(strcmp(info->path,"/home.html") == 0){
	info->code = 302;
  }else if(strcmp(info->path,"/form.html") == 0){
	info->code = 303;
  }else if( strcmp(info->path,cmp) == 0){
    info->code = 418;
  }else if(ret == -1){
    info->code = 404;//失敗
  }else if((access (info->real_path,R_OK)) == -1){
    perror ("accesserror:");
    info->code = 403;
  }else{
    info->code = 200;//成功
    info->size = (int) s.st_size;
  }

  pext = strstr(info->path, ".");//info->pathの中から'.'を探す、あればアドレスを返し、無ければNULL。'.'があれば.から文末までをpextに格納
  if(pext != NULL && strcmp(pext, ".html") == 0){//.以下がhtmlと一致
    strcpy(info->type, "text/html");
  }else if(pext != NULL && strcmp(pext, ".jpg") == 0){//.以下がjpgと一致
    strcpy(info->type, "image/jpeg");
  }else if(pext != NULL && strcmp(pext,".mp4") == 0){
    strcpy(info->type, "video/mp4");
  }else if(pext != NULL && strcmp(pext, ".doc") == 0){
    strcpy(info->type, "application/msword");
  }else if(pext != NULL && strcmp(pext, ".pdf") == 0){
    strcpy(info->type, "application/pdf");
  }else if(pext != NULL && strcmp(pext, ".xls") == 0){
    strcpy(info->type, "application/vcd.ms-excel");
  }else if(pext != NULL && strcmp(pext, ".ppt") == 0){
    strcpy(info->type, "application/vcd.ms-powerpoint");
  }else if(pext != NULL && strcmp(pext, ".png") == 0){
    strcpy(info->type, "image/png");
  }else if(pext != NULL && strcmp(pext, ".gif") == 0){
    strcpy(info->type, "image/gif");
  }else if(pext != NULL && strcmp(pext, ".css") == 0){
    strcpy(info->type, "text/css");
  }else if(pext != NULL && strcmp(pext, ".txt") == 0){
    strcpy(info->type, "text/plain");
  }else if(pext != NULL && strcmp(pext, ".tsv") == 0){
    strcpy(info->type, "text/tab-separated-values");
  }
}


int exp1_parse_header(char* buf, int size, exp1_info_type* info)//HTTPリクエストの1行目を抽出
{
  char status[1024];
  int i, j;

  enum state_type //enumは列挙型、複数の変数に一連の整数値をつける必要がある際に便利、定数のリスト
  {
    PARSE_STATUS,
    PARSE_END
  }state; //タグ名：state_type, 定数リスト：{PARSE_STATUS,PARSE_END}, 変数：state
  //リストは上から順に0から1ずつ+されていく→PARSE_STATUS=0,PARSE_END=1 ?

  state = PARSE_STATUS;//(0?)
  j = 0;
  for(i = 0; i < size; i++){
    switch(state){
    case PARSE_STATUS:
      if(buf[i] == '\r'){//rは復帰
        status[j] = '\0';
        j = 0;
        state = PARSE_END;
        exp1_parse_status(status, info);//HTTPメソッドのcmdやURI情報をinfoへ格納
        exp1_check_file(info);//格納したinfoを渡して、request対象の実体へのパス情報やHTTPリプライで返す具体的な情報をinfoに格納
      }else{
        status[j] = buf[i];//1行分の受け取った情報をstatus[]に格納していく
        j++;
      }
      break;
    }

    if(state == PARSE_END){
      return 1;
    }
  }

  return 0;
}

void exp1_send_404(int sock)
{
  char buf[16384];
  int ret;

  sprintf(buf, "HTTP/1.0 404 Not Found\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_403(int sock)
{
  char buf[16384];
  int ret;

  sprintf(buf, "HTTP/1.0 403 Forbidden\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_418(int sock)
{
  char buf[16384];
  int len;
  int ret;
  len = sprintf(buf, "HTTP/1.0 418 teapot!\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_301(int sock)
{
  char buf[16384];
  int ret;
  int len;
  
  len = sprintf(buf, "HTTP/1.0 301 Moved Permanently\r\n");
  len += sprintf(buf + len, "Location: http://192.168.1.101:19088/new.html\r\n");
  len += sprintf(buf + len, "\r\n");
  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_302(int sock)
{
  char buf[16384];
  int ret;
  int len;
  
  len = sprintf(buf, "HTTP/1.0 302 Found\r\n");
  len += sprintf(buf + len, "Location: http://192.168.1.101:19088/temporary.html\r\n");
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_303(int sock)
{
  char buf[16384];
  int ret;
  int len;
  
  len = sprintf(buf, "HTTP/1.0 303 See Other\r\n");
  len += sprintf(buf + len, "Location: http://192.168.1.101:19088/top.html\r\n");
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, strlen(buf), 0);

  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}


void exp1_send_file(int sock, char* filename)
{
  FILE *fp;
  int len;
  char buf[16384];

  fp = fopen(filename, "r");
  if(fp == NULL){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  len = fread(buf, sizeof(char), 16384, fp);
  while(len > 0){
    int ret = send(sock, buf, len, 0);
    if(ret < 0){
      shutdown(sock, SHUT_RDWR);
      close(sock);
      break;
    }
    len = fread(buf, sizeof(char), 1460, fp);
  }

  fclose(fp);
}


void exp1_http_reply(int sock, exp1_info_type *info)
{

  char buf[16384];
  int len;
  int ret;

  if(info->code == 404){
    exp1_send_404(sock);
    printf("404 not found %s\n", info->path);
    return;
  }
  if(info->code == 403){
    exp1_send_403(sock);
    printf("403 Forbidden %s\n", info->path);
    return;
  }
   if(info->code == 418){
    exp1_send_418(sock);
    printf("418 teapot! %s\n", info->path);
    exp1_send_file(sock, info->real_path);
    return;
  }else if(info->code == 301){
	exp1_send_301(sock);
    printf("301 Moved Permanently %s\n", info->path);
    return;
  }else if(info->code == 302){
	exp1_send_302(sock);
    printf("302 Found %s\n", info->path);
    return;
  }else if(info->code == 303){
	exp1_send_303(sock);
    printf("303 See Other %s\n", info->path);
    return;
  }


  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
  len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  exp1_send_file(sock, info->real_path);
}


int exp1_http_session(int sock)
{
  char buf[2048];
  int recv_size = 0;
  exp1_info_type info;
  int ret = 0;

  while(ret == 0){
    int size = recv(sock, buf + recv_size, 2048, 0);

    if(size == -1){
      return -1;
    }

    recv_size += size;
    ret = exp1_parse_header(buf, recv_size, &info);//受信データのヘッダ部分を分解、変数infoへ格納
  }

  exp1_http_reply(sock, &info);

  return 0;
}


int main(int argc, char **argv)
{
  int sock_listen;

  sock_listen = exp1_tcp_listen(19088);

  while(1){
    struct sockaddr addr;
    int sock_client;
    int len;

    sock_client = accept(sock_listen, &addr, (socklen_t*) &len);//成功したとき非負、失敗-1
    //fprintf(stderr,"%d",sock_client);
    exp1_http_session(sock_client);

    shutdown(sock_client, SHUT_RDWR);//成功したとき0、失敗-1、RDWRは以降の送受信禁止
    close(sock_client);
  }
}