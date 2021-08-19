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
// Sample UDP client that expects echoed messages. This example
// shows how you can use "connect" to connect the remote endpoint
// of a socket to a specific IP/port combination. The local endpoint
// is still assigned by the local O/S and uses an empheral port.
//
// Since the remote endpoint is assigned, we can use the write or send
// system calls rather than sendto.
//
// Furthermore, we do not need to use recvfrom, because the ONLY datagrams
// accepted by a connected socket are those that come from the specified
// remote endpoint; however, those messages can arrive on any
// interface and will be accepted, since the socket has not been bound
// a specific interface/port on this system.
//
// Lastly, errors are returned to the process for a connected UDP socket.
//
// This example also shows the use of "getpeername" to retrieve information
// about the "peer" (remote endpoint in this case), including the IP
// adddress and port.
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

  while (!feof(stdin)) {
    const int size = 1024;
    char input[size];
    char buffer[size];
    int len;
    struct sockaddr_in from_addr;
    socklen_t from_size = sizeof(from_addr);
    char from_name[1024];

    len = getinput(input, size);

    ret = write(sockfd, input, len+1);
    check_and_exit(ret,"write");

    ret = read(sockfd, buffer, size);
    check_and_exit(ret,"read");


    buffer[size-1] = 0;

    from_size = sizeof(from_addr);
    ret = getpeername(sockfd, (struct sockaddr*) &from_addr, &from_size);
    check_and_exit(ret,"getpeername");

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
