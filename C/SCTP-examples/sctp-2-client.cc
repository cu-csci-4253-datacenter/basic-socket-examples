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
// Sample TCP server. This simply echos a reply
//

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int sockfd;
  int ret;

  fprintf(stdout, "Destinatio network is *%s*\n", argv[1]);
  fprintf(stdout, "Destination port is *%s*\n", argv[2]);

  sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
  //  check_and_exit(ret, "socket");


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
  struct sockaddr_in zero;
  memset(&zero,0,sizeof(zero));
  zero.sin_family = AF_INET;
  bind(sockfd, (const sockaddr *) &zero, sizeof(zero));
	 
  ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "connect");
  
  print_socket_endpoints(stderr,"socket", sockfd);
  //
  // Start the I/O loop
  //
  for(;;) {
    char line[1024];
    int len = getinput(line, sizeof(line));
    fprintf(stderr,"Read %d-byte line: %s\n", len, line);
    if ( strcmp(line, ".") == 0 ) {
      fprintf(stderr, "User signals end of input, exit\n");
      exit(0);
    } else {
      int w = write(sockfd, line, len+1);
      if ( w < 0 ) {
	perror("write");
	break;
      } else {
	fprintf(stderr,"And sent %d bytes to socket\n", w);
      }
    }

    //
    // Now, wait for input from the socket
    //
    len = fgetinput(sockfd, line, sizeof(line));
    fprintf(stderr,"Got %d-byte response: %s\n",
	    len, line);
  }
}
