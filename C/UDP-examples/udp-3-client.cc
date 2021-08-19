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
// Sample UDP client that expects echod messages
//

int
main(int argc, char **argv)
{
  struct sockaddr_in to_addr;
  int sockfd;
  int ret;

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

  while (!feof(stdin)) {
    const int size = 1024;
    char input[size];
    char buffer[size];
    int len;
    struct sockaddr_in from_addr;
    socklen_t from_size = sizeof(from_addr);
    char from_name[1024];

    if ( fgets(input, sizeof(input), stdin) == NULL ) {
      //
      // End of input
      //
      break;
    }
    len = strlen(input);
    //
    // Get rid of newline..
    //
    if (input[len-1] == '\n') {
      input[len-1] = 0;
      len--;
    }

    ret = sendto(sockfd, input, len+1, 0,
		 (struct sockaddr*) &to_addr, sizeof(to_addr));

    if ( ret < 0 ) {
      perror("sendto");
      exit(1);
    }

    ret = recvfrom(sockfd, buffer, size, 0,
		   (struct sockaddr*) &from_addr, &from_size);
    if ( ret < 0 ) {
      perror("recvfrom");
      exit(1);
    }

    buffer[size-1] = 0;
    inet_ntop(AF_INET,&from_addr.sin_addr,
	      from_name, sizeof(from_name));
    fprintf(stdout,"Received messsage from %s (port %d): *%s*\n", 
	    from_name, ntohs(from_addr.sin_port), buffer);

  }

  return(0);
}
