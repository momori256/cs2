#pragma once

#include <netdb.h> // gai, sockaddr_in.

void error(const char* const message);

int sock_create(const char* const port, int backlog);
int sock_accept(int listen_fd, struct sockaddr_in* const peer_addr);
