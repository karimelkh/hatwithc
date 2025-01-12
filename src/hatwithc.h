#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hname_to_ip(const char *hostname, char *ip_buffer) {
  struct addrinfo hints, *info, *p;
  int result;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  result = getaddrinfo(hostname, NULL, &hints, &info);
  if (result != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
    return -1;
  }

  for (p = info; p != NULL; p = p->ai_next) {
    struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
    inet_ntop(p->ai_family, &addr->sin_addr, ip_buffer, INET_ADDRSTRLEN);
    freeaddrinfo(info);
    return 0;
  }

  freeaddrinfo(info);
  return -1;
}
