#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/errno.h>

int
main(int argc, char **argv)
{
  struct sockaddr_in addr;
  char buffer[1024];

  fprintf(stdout, "Argument is *%s*\n", argv[1]);
  inet_pton(AF_INET, argv[1], &addr.sin_addr);
  //
  // The address is now stored in the socket.
  // Reverse translate
  //
  inet_ntop(AF_INET,&addr.sin_addr, buffer, sizeof(buffer));
  fprintf(stdout,"Reverse translated address is *%s*\n", buffer);
  return(0);
}
