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
#include <deque>
#include <algorithm>
#include <sys/poll.h>
#include "support.h"

using namespace std;
//
// Sample TCP server. This example does not fork a child for concurrency.
// Instead, it uses the "poll" system call to wait for input
// on one of many possible sockets, including the listen socket.
//

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  int listenfd;

  if ( argc != 3 ) {
    fprintf(stderr,"Usage: %s network port\n",
	    argv[0]);
  }

  fprintf(stdout, "Specified network is *%s*\n", argv[1]);
  fprintf(stdout, "Specified port is *%s*\n", argv[2]);

  listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
  check_and_exit(listenfd, "socket");
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
  int ret = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
  check_and_exit(ret, "bind");

  //
  // Place socket in LISTEN state
  //
  ret = listen(listenfd, 100);
  check_and_exit(ret,"listen");

  //
  // Start the acceptance loop
  //

  typedef deque<int> intlist;
  intlist fd_list;
  fd_list.push_back(listenfd);

  for(;;) {
    int maxfd = fd_list.size();
    struct pollfd fds[maxfd];

    intlist::iterator i;
    int j = 0;
    for (i = fd_list.begin(); i != fd_list.end(); i++) {
      int fd = *i;
      fds[j].fd = fd;
      fds[j].events = POLLIN | POLLPRI | POLLERR | POLLHUP;
      fds[j].revents = 0;
      j++;
    }
    
    int n = poll(fds, maxfd, -1);

    fprintf(stderr,"Got %d from poll\n", n);

    for( int i = 0; i <= maxfd && n > 0; i++) {
      int myfd = fds[i].fd;

      if ( fds[i].revents & (POLLIN) ) {
	fprintf(stderr,"File descriptor %d is ready\n", myfd);
	n--;

	if ( myfd == listenfd ) {
	  struct sockaddr_in theiraddr;
	  socklen_t theiraddr_len = sizeof(theiraddr);
	  int acceptfd = accept(listenfd, 
				(struct sockaddr *)&theiraddr,
				&theiraddr_len);
	  check_and_exit(acceptfd, "accept");
    
	  fd_list.push_back(acceptfd);

	  fprintf(stderr,"Start connection with...\n");
	  print_socket_endpoints(stderr,"Start connection with ..", acceptfd);
	} else {
	  char line[1024];
	  ssize_t r = read(myfd, line, sizeof(line));

	  if ( r <= 0 ) {
	    if ( r < 0 ) {
	      perror("read");
	    } else {
	      fprintf(stderr,"** End of input, exit\n");
	    }
	    //
	    // Remove file descriptor for list
	    //
	    intlist::iterator x = find(fd_list.begin(),
				       fd_list.end(),
				       myfd);
	    if ( x != fd_list.end() ) {
	      fd_list.erase(x);
	    }
	    close(myfd);

	    break;
	  } else {
	    fprintf(stderr,"** %d Got %ld bytes: %s\n", getpid(), r, line);
	  }

	  int w = write(myfd, line, r);
	  if ( w < 0 ) {
	    perror("write");
	    break;
	  } else {
	    fprintf(stderr,"** %d sent %d bytes to socket\n", getpid(), w);
	  }

	}
      }
    }
  }
  fprintf(stderr,"Parent is exiting...\n");
}
