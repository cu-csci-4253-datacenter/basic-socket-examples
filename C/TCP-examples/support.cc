#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <string.h>
#include <assert.h>
#include "support.h"

extern char *me; // either 'client' or 'server' - used to prefix messages

//
// Copy a set of file descriptors
//
void
fd_copy(fd_set *to, fd_set *from, int n)
{
  int sock;

  FD_ZERO(to);
  for(sock = 0; sock <= n; sock++) {
    if ( FD_ISSET(sock, from) ) {
      FD_SET(sock, to);
    }
  }

}

//
// Read a line of input from stdin
//
int
getinput(char *input, int size)
{
  int len = 0;
  if ( fgets(input, size, stdin) == NULL ) {
    //
    // End of input
    //
    exit(0);
    return -1;
  }
  len = strlen(input);
  //
  // Get rid of newline..
  //
  if (input[len-1] == '\n') {
    input[len-1] = 0;
    len--;
  }
  return len;
}

//
// get input from a file descriptor
//
int
fgetinput(int fd, char *input, int size)
{
  int len = 0;
  len = read(fd, input, size);
  if ( len < 0 ) {
    return -1;
  }
  len = strlen(input);
  //
  // Get rid of newline..
  //
  if (input[len-1] == '\n') {
    input[len-1] = 0;
    len--;
  }
  return len;
}

//
// simply check error message, print it (using perror) & exit
// if error has occured.
//
void
check_and_exit(int ret, const char *func)
{
  
  char msg[strlen(func) + 1000];
  sprintf(msg, "%s: %s\n", me, func);

  if ( ret < 0 ) {
    perror(msg);
    exit(1);
  }
}

//
// Use 'getsockname' calls to find out information about the
// socket end-points in human readable form.
//
void
print_socket_endpoints(FILE* file, const char *name, int sockfd)
{
  //
  // Get my end of the socket
  //
  struct sockaddr_in myaddr;
  socklen_t myaddr_len = sizeof(myaddr);
  int r1 = getsockname(sockfd,
		       (struct sockaddr *) &myaddr,
		       &myaddr_len);
  char myname[512];
  if ( r1 < 0 ) {
    sprintf(myname, "<UNKNOWN>");
  } else {
    char net[512];
    inet_ntop(AF_INET, &myaddr.sin_addr, &net[0], sizeof(net));
    sprintf(myname, "%s:%d",
	    net, htons(myaddr.sin_port));
  }

  //
  // Get their end of the socket (the "peer")
  //
  struct sockaddr_in theiraddr;
  socklen_t theiraddr_len = sizeof(theiraddr);
  int r2 = getpeername(sockfd,
		       (struct sockaddr *) &theiraddr,
		       &theiraddr_len);
  char theirname[512];
  if ( r2 < 0 ) {
    sprintf(theirname, "<UNKNOWN>");
  } else {
    char net[512];
    inet_ntop(AF_INET,&theiraddr.sin_addr, &net[0], sizeof(net));
    sprintf(theirname, "%s:%d",
	    net, htons(theiraddr.sin_port));
  }
  fprintf(file, "%s: Socket %s (%d) is connecting %s(me) and %s(them)\n",
	  me, name, sockfd, myname, theirname);
}

//
// Set perl-mode in Emacs...
//
// Local Variables: ***
// mode: c++ ***
// End:
