#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <string.h>
#include "support.h"

//
// Sample TCP server. This example extends tcp-1-server by reading
// and then writing a reply.
//

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int listenfd;
  int ret;

  fprintf(stdout, "Specified network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified port is *%s*\n", argv[2]);

  ret = listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
  check_and_exit(ret, "socket");
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
  ret = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "bind");

  //
  // Place socket in LISTEN state
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
	fprintf(stderr,"** End of input, exit\n");
	break;
      } else {
	fprintf(stderr,"** Got %ld bytes: %s\n", r, line);
      }

      int w = write(acceptfd, line, r);
      if ( w < 0 ) {
	perror("write");
	break;
      } else {
	fprintf(stderr,"And sent %d bytes to socket\n", w);
      }

    }
    //
    // Close socket, wait for another connection
    //
    close(acceptfd);
  }
}
