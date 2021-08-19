//-*-c++-*-
#ifndef _support_h_
#define _support_h_

#include <stdio.h>
#include <sys/select.h>

extern int getinput(char *, int);
extern int fgetinput(int fd, char *input, int size);
extern void check_and_exit(int, const char*);
extern void print_socket_endpoints(FILE* file, const char *name, int sockfd);
extern void fd_copy(fd_set *to, fd_set *from, int n);

#endif
