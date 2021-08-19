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
  const int size = 32000;
  const int bufsize = size * 2;


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

  //
  // bind and then connect my socket.
  //
  ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
  if ( ret < 0 ) {
    perror("setsockopt");
    exit(1);
  }
  ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
  if ( ret < 0 ) {
    perror("setsockopt");
    exit(1);
  }


  ret = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
  if ( ret < 0 ) {
    perror("bind");
    exit(1);
  }

  for(;;) {
    char buffer[size];
    struct sockaddr_in from_addr;
    socklen_t from_size = sizeof(from_addr);
    char from_name[1024];
    int gotsize;

    gotsize = recvfrom(sockfd, buffer, size, 0,
		       (struct sockaddr*) &from_addr, &from_size);
    if ( gotsize < 0 ) {
      perror("recvfrom");
      exit(1);
    }

    inet_ntop(AF_INET,&from_addr.sin_addr,
	      from_name, sizeof(from_name));

    fprintf(stdout,"Received %d byte messsage from %s (port %d)\n", 
	    gotsize, from_name, ntohs(from_addr.sin_port));

    ret = sendto(sockfd, buffer, gotsize, 0,
		 (struct sockaddr*) &from_addr, sizeof(from_addr));

    if ( ret < 0 ) {
      perror("sendto");
      exit(1);
    }
  }

  return(0);
}
