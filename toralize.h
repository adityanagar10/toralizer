/* toralize.h */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define REQ_SIZE sizeof(struct proxy_request)
#define RES_SIZE sizeof(struct proxy_response)
#define USERNAME "torlizer"

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/*
  ˇ+----+----+----+----+----+----+----+----+----+----+....+----+
   | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
   +----+----+----+----+----+----+----+----+----+----+....+----+
#:    1    1      2              4           variable       1
*/

/*
 * NOTE: even though the userid is a variable size we are using a 7 char userid.
 * We went with the length of 8 so that when writing the user id the last
 * character is NULL
 */

struct proxy_request {
  int8 vn;
  int8 cd;
  int16 dstport;
  int32 dstip;
  unsigned char userid[8];
};

typedef struct proxy_request Req;

/*
                +----+----+----+----+----+----+----+----+
                | VN | CD | DSTPORT |      DSTIP        |
                +----+----+----+----+----+----+----+----+
#:	   1    1      2              4
*/

struct proxy_response {
  int8 vn;
  int8 cd;
  int16 dstport;
  int32 dstip;
};

typedef struct proxy_response Res;

/* function declarations */

Req *request(const char *, const int);
int main(int, char **);
