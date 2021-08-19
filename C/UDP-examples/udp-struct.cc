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
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


//
// Sample UDP client that expects echod messages
//

struct ExampleStruct {
  char packet[256];
  int seq;
};

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

  int packetNumber;
  for(packetNumber = 0; packetNumber < 100; packetNumber++) {

    struct sockaddr_in from_addr;
    unsigned int from_size = sizeof(from_addr);
    char from_name[1024];


    //
    // Create some useless message in a structure and send it
    //
    struct ExampleStruct example;
    example.seq = packetNumber;
    sprintf(example.packet, "This is packet %d\n", example.seq);

    ret = sendto(sockfd, &example, sizeof(example), 0,
		 ((struct sockaddr*) &(to_addr)), sizeof(to_addr));

    if ( ret < 0 ) {
      perror("sendto");
      exit(1);
    }

    //
    // Add in a call to select - first, create a new
    // file descriptor set and set the one file descriptor
    // we actually care about (sockfd).
    //
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);
    FD_SET(sockfd, &read_fd_set);

    //
    // then arrange for 3 second time out
    //
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    //
    // Now wait for input on sockfd or timeout
    //
    int n = select(sockfd+1,
		   &read_fd_set,
		   NULL,
		   NULL,
		   &timeout);

    if ( n == 0 ) {
      fprintf(stderr,"Looks like a timeout!\n");
    } else {
      struct ExampleStruct received;
      ret = recvfrom(sockfd, &received, sizeof(received), 0,
		     (struct sockaddr*) &from_addr, &from_size);
      if ( ret < 0 ) {
	perror("recvfrom");
	exit(1);
      }

      fprintf(stderr,"Got message of %d bytes, which is packet #%d and content %s\n",
	      ret, received.seq, received.packet);
    }

  }
  return (0);
}
