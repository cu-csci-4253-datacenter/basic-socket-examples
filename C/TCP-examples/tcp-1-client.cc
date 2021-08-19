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
// Sample TCP server. This simply echos a reply
//

const char *me = "CLIENT";

int
main(int argc, char **argv)
{
  //
  // Sockets come in many flavours - this is the 
  // socket address for an IPv4 interface.
  //
  struct sockaddr_in addr;
  int sockfd;
  int ret;

  fprintf(stdout, "%s: Destination network is *%s*\n", me, argv[1]);
  fprintf(stdout, "%s: Destination port is *%s*\n", me, argv[2]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  check_and_exit(sockfd, "socket");

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
  ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "connect");
  
  print_socket_endpoints(stderr,"socket", sockfd);
  //
  // Start the I/O loop
  //
  for(;;) {
    char line[1024];
    int len = getinput(line, sizeof(line));
    fprintf(stderr,"%s: Read %d-byte line: %s\n", me, len, line);
    if ( strcmp(line, ".") == 0 ) {
      fprintf(stderr, "%s: User signals end of input, exit\n", me);
      exit(0);
    } else {
      int w = write(sockfd, line, len+1);
      if ( w < 0 ) {
	perror("write");
	break;
      } else {
	fprintf(stderr,"%s: And sent %d bytes to socket\n", me, w);
      }
    }
  }
}
