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
//

int
main(int argc, char **argv)
{
  struct sockaddr_in to_addr; int sockfd; int ret;

  fprintf(stdout, "Specified destination network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified destination port is *%s*\n", argv[2]);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if ( sockfd < 0 ) {
    perror("socket");
    exit(1);
  }

  //
  // Initialize destination address
  //
  memset(&to_addr, 0, sizeof(to_addr));
  to_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &to_addr.sin_addr);
  to_addr.sin_port = htons(atoi(argv[2]));

  //
  // connect my socket.
  //
  ret = connect(sockfd, (struct sockaddr*) &to_addr, sizeof(to_addr));
  if ( ret < 0 ) {
    perror("connect");
    exit(1);
  }


  //
  // Set socket option to to joint mcast
  //
  {
    u_char ttl = 16;

    ret = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,
		     &ttl, sizeof(ttl));
    check_and_exit(ret, "setsockopt");
  }

  while (!feof(stdin)) {
    const int size = 1024;
    char input[size];
    int len;

    len = getinput(input, size);

    ret = write(sockfd, input, len+1);
    check_and_exit(ret,"write");
  }

  return(0);
}


//
// Set perl-mode in Emacs...
//
// Local Variables: ***
// mode: c++ ***
// End:
