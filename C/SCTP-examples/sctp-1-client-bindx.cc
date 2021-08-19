#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/ip.h"
#include "arpa/inet.h"
#include <netinet/sctp.h>
#include <sys/errno.h>
#include <string.h>
#include <fcntl.h>


#define SCTP_ADD                        35
#define SCTP_ADD_IP                     36
#define SCTP_DEL_IP                     37

#include "support.h"

//
// Sample TCP server. This simply echos a reply
//

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

  fprintf(stdout, "Destination network is *%s*\n", argv[1]);
  fprintf(stdout, "Destination port is *%s*\n", argv[2]);

  sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
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
  struct sockaddr_in zero;
  memset(&zero,0,sizeof(zero));
  zero.sin_family = AF_INET;
  bind(sockfd, (const sockaddr *) &zero, sizeof(zero));
	 
  ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "connect");

  if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
    perror("fcntl");
    exit(-1);
  }

  int nodelay = 1;
  if (setsockopt(sockfd, SOL_SCTP, SCTP_NODELAY,
		 &nodelay, sizeof(nodelay)) < 0) {
    perror("setsockopt");
    exit(-1);
  }

  int sctp_add = 1;
  if (setsockopt(sockfd, SOL_SCTP, SCTP_ADD,
		 &sctp_add, sizeof(sctp_add)) < 0) {
    perror("setsockopt SCTP_ADD");
    exit(-1);
  }
  
  print_socket_endpoints(stderr,"socket", sockfd);
  //
  // Start the I/O loop
  //
  for(;;) {
    char line[1024];
    int len = getinput(line, sizeof(line));
    fprintf(stderr,"Read %d-byte line: %s\n", len, line);
    char addr[1000];
    if (sscanf(line,"add %100s", addr) == 1) {
      fprintf(stderr,"Add interface %s\n", addr);

      struct sockaddr_in sa; 
      sa.sin_family = AF_INET;
      sa.sin_port = htons(atoi(argv[2]));
      inet_pton(AF_INET, addr, &sa.sin_addr);
      
      if (setsockopt(sockfd, SOL_SCTP, SCTP_ADD_IP,
		     &sa, sizeof(sa)) < 0) {
	perror("setsockopt");
	exit(-1);
      }


      struct sockaddr_in *sap[1];
      sap[1] = &sa;
      int result = sctp_bindx(sockfd,
			      (struct sockaddr *)sap, 1,
			      SCTP_BINDX_ADD_ADDR);
      if ( result < 0) {
	perror("bindx");
      }
      
    } else if ( strcmp(line, ".") == 0 ) {
      fprintf(stderr, "User signals end of input, exit\n");
      exit(0);
    } else {
      int w = write(sockfd, line, len+1);
      //      int w = send(sockfd, line, len+1, 0);
      if ( w < 0 ) {
	perror("write");
	break;
      } else {
	fprintf(stderr,"And sent %d bytes to socket\n", w);
      }
    }
  }
}
