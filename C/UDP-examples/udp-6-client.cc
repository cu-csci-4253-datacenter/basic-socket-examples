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
// Extend UDP-5 to specify the port options that allow us to
// reuse port numbers across different interfaces.
//

int
main(int argc, char **argv)
{
  struct sockaddr_in to_addr;
  struct sockaddr_in my_addr;
  int sockfd;
  int ret;

  fprintf(stdout, "Specified destination network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified destination port is *%s*\n", argv[2]);
  fprintf(stdout, "Specified source network is *%s*\n", argv[3]);
  fprintf(stdout, "Specified source port is *%s*\n", argv[4]);

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
  // Initialize my address
  //
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[3], &my_addr.sin_addr);
  my_addr.sin_port = htons(atoi(argv[4]));

  //
  // Set socket option to allow re-use
  //
  {
    int on = 1;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    check_and_exit(ret, "setsockopt");
  }

  //
  // bind and then connect my socket.
  //
  ret = bind(sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if ( ret < 0 ) {
    perror("bind");
    exit(1);
  }

  {
    struct sockaddr_in report;
    socklen_t len = sizeof(report);
    char name[1024];
    getsockname(sockfd, (struct sockaddr*) &report, &len);
    inet_ntop(AF_INET,&report.sin_addr,
	      name, sizeof(name));
    fprintf(stdout,"I see myself as  %s (port %d):\n", 
	    name, ntohs(report.sin_port));
  }


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


    from_size = sizeof(from_addr);
    ret = getpeername(sockfd, (struct sockaddr*) &from_addr, &from_size);
    check_and_exit(ret,"getpeername");

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
