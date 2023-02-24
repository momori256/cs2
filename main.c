#include <stdio.h> // perror.
#include <stdlib.h> // exit.
#include <unistd.h> // read, write, close.

#include "sock.h"

const int BACKLOG = 10;
const int NBUF = 256;

#define CRLF "\r\n"

static void handle_request(int fd);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s <PORT>\n", argv[0]);
    return 0;
  }
  const char* const port = argv[1];
  const int lfd = sock_create(port, BACKLOG);
  while (1) {
    const int pfd = sock_accept(lfd, NULL);
    handle_request(pfd);
  }
  return 0;
}

static void handle_request(int fd) {
  char request[NBUF];
  const size_t nread = read(fd, request, sizeof(request));
  if (nread == -1) {
    error("read");
  }

  char response[NBUF];
  const int nres = snprintf(
    response,
    sizeof(response),
    "HTTP/1.0 200 OK" CRLF
    "Content-Length: %lu" CRLF
    CRLF
    "%s",
    nread,
    request);
  const size_t nwrite = write(fd, response, nres);
  if (nwrite == -1) {
    error("write");
  }
  if (close(fd) == -1) {
    error("close");
  }
}
