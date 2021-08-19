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
// Sample TCP server. This simply reads lines of text from the remote
// connection and prints them out in the server.
//

const char *me = "SERVER";

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int listenfd;

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
  // Place socket in LISTEN state - this is how the O/S know's
  // we're a server.
  //
  ret = listen(listenfd, 100);
  check_and_exit(ret,"listen");

  //
  // Start the acceptance loop
  //
  for(;;) {
    struct sockaddr_in theiraddr;
    socklen_t theiraddr_len = sizeof(theiraddr);
    int acceptfd = accept(listenfd, 
			  (struct sockaddr *)&theiraddr,
			  &theiraddr_len);
    check_and_exit(acceptfd, "accept");
    
    //
    // Describe the end points
    //
    print_socket_endpoints(stderr,"listen", listenfd);
    print_socket_endpoints(stderr,"accept", acceptfd);
   
    //
    // Begin file I/O on this socket
    //
    for(;;) {
      char line[1024];
      ssize_t r = read(acceptfd, line, sizeof(line));
      if ( r < 0 ) {
	perror("read");
	break;
      } else  if ( r ==  0 ) {
	fprintf(stderr,"%s: ** End of input, exit\n", me);
	break;
      } else {
	fprintf(stderr,"%s:** Got %d bytes: %s\n", me, (int) r, line);
      }
    }
    //
    // Close socket, wait for another connection
    //
    close(acceptfd);
  }
}
