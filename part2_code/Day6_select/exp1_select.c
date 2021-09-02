#include "exp1.h"
#include "exp1lib.h"
 #include <stdio.h>
 #include <unistd.h>
 #include <time.h>
 #include <stdbool.h>
 #include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <string.h>

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
}

void exp1_check_file(exp1_info_type *info)
{
  struct stat s;//stat()の返り値がstat構造体
  int ret;
  char* pext;

  sprintf(info->real_path, "html%s", info->path);//real_pathにhtmlpathの情報を…？
  ret = stat(info->real_path, &s);//成功0、失敗-1 pathname,buf

  if((s.st_mode & S_IFMT) == S_IFDIR){
    //st_mode(アクセス保護)とS_IFMT(S_IFMT	0170000	ファイル種別を示すビット領域を表すビットマスク)がS_IFDIR	0040000	ディレクトリの時
    //このif文の書きかたはMANにある通常のファイルかどうかを検査する書きかた
    sprintf(info->real_path, "%s/index.html", info->real_path);//real_pathにreal_pathを・・?
  }

  ret = stat(info->real_path, &s);

  if(ret == -1){
    info->code = 404;//失敗
  }else{
    info->code = 200;//成功
    info->size = (int) s.st_size;
  }

  pext = strstr(info->path, ".");//info->pathの中から'.'を探す、あればアドレスを返し、無ければNULL。'.'があれば.から文末までをpextに格納
  if(pext != NULL && strcmp(pext, ".html") == 0){//.以下がhtmlと一致
    strcpy(info->type, "text/html");
  }else if(pext != NULL && strcmp(pext, ".jpg") == 0){//.以下がjpgと一致
    strcpy(info->type, "image/jpeg");
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
  while(len > 0)
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





#define MAXCHILD 1200
void acceptLoop(int sock) {

  // クライアント管理配列
  int childNum = 0;
  int child[MAXCHILD];
  int i = 0;
  for (i = 0; i < MAXCHILD; i++) {
    child[i] = -1;
  }

  while (true) {

    // select用マスクの初期化
    fd_set mask;
    FD_ZERO(&mask);
    FD_SET(sock, &mask);  // ソケットの設定
    int width = sock + 1;
    int i = 0;
    for (i = 0; i < childNum; i++) {
      if (child[i] != -1) {
        FD_SET(child[i], &mask);  // クライアントソケットの設定
        if ( width <= child[i] ) {
          width = child[i] + 1;
        }
      }
    }

    // マスクを設定
    fd_set ready = mask;

    // タイムアウト値のセット
    struct timeval timeout;
    timeout.tv_sec = 600;
    timeout.tv_usec = 0;

    switch (select(width, (fd_set *) &ready, NULL, NULL, &timeout)) {
    case -1:
      // エラー処理
      perror("select");
      break;
    case 0:
      // タイムアウト
      break;
    default:
      // I/Oレディあり

      if (FD_ISSET(sock, &ready)) {
        // サーバソケットレディの場合
        struct sockaddr_storage from;
        socklen_t len = sizeof(from);
        int acc = 0;
        if ((acc = accept(sock, (struct sockaddr *) &from, &len))
            == -1) {
          // エラー処理
          if (errno != EINTR) {
            perror("accept");
          }
        } else {
          // クライアントからの接続が行われた場合
          char hbuf[NI_MAXHOST];
          char sbuf[NI_MAXSERV];
          getnameinfo((struct sockaddr *) &from, len, hbuf,
              sizeof(hbuf), sbuf, sizeof(sbuf),
              NI_NUMERICHOST | NI_NUMERICSERV);
          fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);

          // クライアント管理配列に登録
          int pos = -1;
          int i = 0;
          for (i = 0; i < childNum; i++) {
            if (child[i] == -1) {
              pos = i;
              break;
            }
          }


          if (pos == -1) {
            if (childNum >= MAXCHILD) {
              // 並列数が上限に達している場合は切断する
              fprintf(stderr, "child is full.\n");
              close(acc);
            } else {
              pos = childNum;
              childNum = childNum + 1;
            }
          }

          if (pos != -1) {
            child[pos] = acc;
           
          }
        }
      }

      // アクセプトしたソケットがレディの場合を確認する
      int i = 0;
      for (i = 0; i < childNum; i++) {
        if (child[i] != -1 && FD_ISSET(child[i], &ready)) {
          // クライアントとの通信処理
            exp1_http_session(child[i]);

            shutdown(child[i], SHUT_RDWR);//成功したとき0、失敗-1、RDWRは以降の送受信禁止
            close(child[i]);
            child[i] = -1;
        }
      }
    }
  }
}

 int main(int argc, char** argv) {
         int sock_listen;

 
         sock_listen = exp1_tcp_listen(19088);
         //sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
         acceptLoop(sock_listen);

         close(sock_listen);
 
         return 0;
 }