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

int
getinput(char *input, int size)
{
  int len;
  if ( fgets(input, size, stdin) == NULL ) {
    //
    // End of input
    //
    exit(0);
    return -1;
  }
  len = strlen(input);
  //
  // Get rid of newline..
  //
  if (input[len-1] == '\n') {
    input[len-1] = 0;
    len--;
  }
  return len;
}

void
check_and_exit(int ret, const char *func)
{
  if ( ret < 0 ) {
    perror(func);
    exit(1);
  }
}


//
// Set perl-mode in Emacs...
//
// Local Variables: ***
// mode: c++ ***
// End:
