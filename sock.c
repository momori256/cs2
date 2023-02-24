#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE (200112L) // gai Feature Test Macro.
#endif

#include "sock.h"

#include <errno.h> // errno.
#include <stdio.h> // NULL, stderr.
#include <stdlib.h> // exit.
#include <string.h> // strerror.
#include <sys/socket.h> // gai, socket, accept.
#include <sys/types.h> // gai.
#include <unistd.h> // close.

/**
 * @brief Print the current error and exit.
 */
void error(const char* const message) {
  perror(message);
  exit(1);
}

/**
 * @brief Create a listening socket.
 * @return The created socket file descriptor.
 */
int sock_create(const char* const port, int backlog) {
  typedef struct addrinfo addrinfo;

  addrinfo hints = {0};
  {
    hints.ai_family = AF_INET; // IPv4.
    hints.ai_socktype = SOCK_STREAM; // TCP.
    hints.ai_flags = AI_PASSIVE; // Server.
  }

  addrinfo* head;
  {
    const int result = getaddrinfo(NULL, port, &hints, &head);
    if (result) {
      fprintf(stderr, "getaddrinfo. err[%s]\n", gai_strerror(result));
      exit(1);
    }
  }

  int sfd = 0;
  for (addrinfo* p = head; p != NULL; p = p->ai_next) {
    sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sfd == -1) {
      continue;
    }

    int val = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) {
      error("setsockopt");
    }

    if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sfd);
      continue;
    }
    break;
  }
  freeaddrinfo(head);

  if (!sfd) {
    error("socket, bind");
  }

  if (listen(sfd, backlog) == -1) {
    error("listen");
  }
  return sfd;
}

/**
 * @brief Accept an incoming connection.
 */
int sock_accept(int listen_fd, struct sockaddr_in* const peer_addr) {
  socklen_t len = sizeof(struct sockaddr_in);
  const int fd = accept(listen_fd, (struct sockaddr*)&peer_addr, &len);
  if (fd == -1) {
    error("accept");
  }
  return fd;
}
