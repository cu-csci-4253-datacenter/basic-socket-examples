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
#include <memory.h>
#include "support.h"

//
//
// Extend UDP-5 to specify the port options that allow us to
// reuse port numbers across different interfaces.
//

int
main(int argc, char **argv)
{
  struct sockaddr_in from_addr;
  int sockfd;
  int ret;

  fprintf(stdout, "Specified from network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified from port is *%s*\n", argv[2]);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if ( sockfd < 0 ) {
    perror("socket");
    exit(1);
  }

  //
  // Initialize destination address
  //
  memset(&from_addr, 0, sizeof(from_addr));
  from_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &from_addr.sin_addr);
  from_addr.sin_port = htons(atoi(argv[2]));

  //
  // Set socket option to to joint mcast
  //
  {
    struct ip_mreq mreq;
    memcpy(&mreq.imr_multiaddr, 
	   &from_addr.sin_addr.s_addr,
	   sizeof(struct in_addr));
	    
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    ret = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		     &mreq, sizeof(mreq));
    check_and_exit(ret, "setsockopt");
  }


  //
  // bind and then connect my socket.
  //
  ret = bind(sockfd, (struct sockaddr*) &from_addr, sizeof(from_addr));
  check_and_exit(ret, "setsockopt");

  for(;;) {
    const int size = 1024;
    char buffer[size];
    struct sockaddr_in from_addr;
    socklen_t from_size = sizeof(from_addr);
    char from_name[1024];

    ret = recvfrom(sockfd, buffer, size, 0,
		   (struct sockaddr*) &from_addr, &from_size);
    check_and_exit(ret,"recvfrom");

    buffer[size-1] = 0;
    inet_ntop(AF_INET,&from_addr.sin_addr,
	      from_name, sizeof(from_name));
    fprintf(stdout,"Received messsage from %s (port %d): *%s*\n", 
	    from_name, ntohs(from_addr.sin_port), buffer);

  }

  return(0);
}


//
// Set perl-mode in Emacs...
//
// Local Variables: ***
// mode: c++ ***
// End:
