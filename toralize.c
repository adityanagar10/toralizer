/* toralize.c */

#include "toralize.h"
#include <string.h>

Req *request(const char *dstip, const int dstport) {
  Req *req;

  req = malloc(REQ_SIZE);

  req->vn = 4;
  req->cd = 1;
  req->dstport = htons(dstport);
  req->dstip = inet_addr(dstip);
  strncpy(req->userid, USERNAME, 8);

  return req;
}

int main(int argc, char *argv[]) {
  char *host;
  int port, sock_file_descriptor;
  struct sockaddr_in sock;
  Req *req;
  Res *res;
  char buf[RES_SIZE];
  int success;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
    return -1;
  }

  host = argv[1];
  port = atoi(argv[2]);

  sock_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_file_descriptor < 0) {
    perror("socket");
    return -1;
  }

  sock.sin_family = AF_INET;
  sock.sin_port = htons(PROXYPORT);
  sock.sin_addr.s_addr = inet_addr(PROXY);

  if (connect(sock_file_descriptor, (struct sockaddr *)&sock, sizeof(sock))) {
    perror("connect failed");
    return -1;
  };

  printf("connected to proxy\n");
  req = request(host, port);
  write(sock_file_descriptor, req, REQ_SIZE);

  memset(buf, 0, RES_SIZE);

  if (read(sock_file_descriptor, buf, RES_SIZE) < 1) {
    perror("read failed");
    free(req);
    close(sock_file_descriptor);
    return -1;
  };

  res = (Res *)buf;
  success = (res->cd == 90);

  if (!success) {
    fprintf(stderr,
            "unable to traverse"
            "the proxy, error code: %d\n",
            res->cd);
    free(req);
    close(sock_file_descriptor);
    return -1;
  }

  printf("successfully traversed the proxy");
  close(sock_file_descriptor);
  free(req);
  return 0;
}
