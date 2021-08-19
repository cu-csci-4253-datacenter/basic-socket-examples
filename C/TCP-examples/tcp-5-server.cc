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
#include "support.h"

//
// Sample TCP server. This example does not fork a child for concurrency.
// Instead, it uses the "select" system call to wait for input
// on one of many possible sockets, including the listen socket.
// This allows a single threaded server to handle multiple clients concurrently.
//

const char *me = "SERVER";

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int listenfd;

  if ( argc != 3 ) {
    fprintf(stderr,"Usage: %s network port\n",
	    argv[0]);
  }

  fprintf(stdout, "%s: Specified network is *%s*\n", me, argv[1]);
  fprintf(stdout, "%s: Specified port is *%s*\n", me, argv[2]);


  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  check_and_exit(listenfd, "socket");
  //
  // Initialize socket
  //
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &addr.sin_addr);
  addr.sin_port = htons(atoi(argv[2]));

  //
  // Bind socket to specified address
  //
  int ret = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "bind");

  //
  // Place socket in LISTEN state
  //
  ret = listen(listenfd, 100);
  check_and_exit(ret,"listen");

  //
  // Start the acceptance loop
  //

  int maxfd = -1;
  fd_set readfd;
  FD_ZERO(&readfd);
  FD_SET(listenfd, &readfd);
  maxfd = listenfd; 

  for(;;) {
    //
    fd_set tempfd;

    fd_copy(&tempfd, &readfd, maxfd+1);

    int n = select(maxfd + 1, &tempfd,
		   NULL, NULL, NULL);

    fprintf(stderr,"Got %d from select\n", n);

    for( int i = 0; i <= maxfd && n > 0; i++) {
      if ( FD_ISSET(i, &tempfd) ) {
	fprintf(stderr,"%s: File descriptor %d is ready\n", me, i);
	n--;
	if ( i == listenfd ) {
	  struct sockaddr_in theiraddr;
	  socklen_t theiraddr_len = sizeof(theiraddr);
	  int acceptfd = accept(listenfd, 
				(struct sockaddr *)&theiraddr,
				&theiraddr_len);
	  check_and_exit(acceptfd, "accept");
    
	  FD_SET(acceptfd, &readfd);
	  if ( acceptfd > maxfd ) {
	    maxfd = acceptfd;
	  }

	  fprintf(stderr,"%s: Start connection with...\n", me);
	  print_socket_endpoints(stderr,"Start connection with ..", acceptfd);
	} else {
	  int myfd = i;
	  char line[1024];
	  ssize_t r = read(myfd, line, sizeof(line));
	  if ( r < 0 ) {
	    perror("read");
	    FD_CLR(myfd, &readfd);
	    close(myfd);
	    break;
	  } else  if ( r ==  0 ) {
	    fprintf(stderr,"%s: ** End of input, exit\n", me);
	    FD_CLR(myfd, &readfd);
	    close(myfd);
	    break;
	  } else {
	    fprintf(stderr,"%s: ** %d Got %d bytes: %s\n", me, getpid(), (int) r, line);
	  }

	  int w = write(myfd, line, r);
	  if ( w < 0 ) {
	    perror("write");
	    break;
	  } else {
	    fprintf(stderr,"%s: ** %d sent %d bytes to socket\n", me, getpid(), w);
	  }

	}
      }
    }
  }
  fprintf(stderr,"%s: Parent is exiting...\n", me);
}
