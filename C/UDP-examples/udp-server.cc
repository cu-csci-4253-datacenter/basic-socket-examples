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

//
// Sample UDP server that echos messages.
//

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int sockfd;
  int ret;

  fprintf(stdout, "Specified network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified port is *%s*\n", argv[2]);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if ( sockfd < 0 ) {
    perror("socket");
    exit(1);
  }
  //
  // Initialize socket
  //
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &addr.sin_addr);
  addr.sin_port = htons(atoi(argv[2]));

  ret = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
  if ( ret < 0 ) {
    perror("bind");
    exit(1);
  }

  for(;;) {
    const int size = 1024;
    char buffer[size];
    int len;
    struct sockaddr_in from_addr;
    socklen_t from_size = sizeof(from_addr);
    char from_name[1024];

    ret = recvfrom(sockfd, buffer, size, 0,
		   (struct sockaddr*) &from_addr, &from_size);
    if ( ret < 0 ) {
      perror("recvfrom");
      exit(1);
    }

    //
    // Null terminate in case sender didn't or
    // buffer overflow
    //
    buffer[size-1] = 0;
    len = strlen(buffer);
    inet_ntop(AF_INET,&from_addr.sin_addr,
	      from_name, sizeof(from_name));

    fprintf(stdout,"Received messsage from %s[0x%x] (port %d): *%s*\n", 
	    from_name, ntohl(from_addr.sin_addr.s_addr),
	    ntohs(from_addr.sin_port), buffer);

    ret = sendto(sockfd, buffer, len+1, 0,
		 (struct sockaddr*) &from_addr, sizeof(from_addr));

    if ( ret < 0 ) {
      perror("sendto");
      exit(1);
    }
  }

  return(0);
}

//
// Set perl-mode in Emacs...
//
// Local Variables: ***
// mode: c++ ***
// End:
