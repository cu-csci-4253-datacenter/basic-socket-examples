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
// Sample UDP relay agent that optionall drops packets and/or re-orders packets.
//
// usage:
//
// udp-relay in-port out-addr out-port [ drop probability ]
//
// Any message sent to the relay port that IS NOT from the out-addr/out-port is
// then sent to out-addr/out-port.
//
// Any message sent to the relay in-port that IS from the out-addr/out-port is
// then forwarded to the last addr/port that forwarded a relayed packet.
//
//

int
initSocket(struct sockaddr_in *addr, const char *netString, int port)
{

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if ( sockfd < 0 ) {
    perror("socket");
    return -1;
  }
  //
  // Initialize socket
  //
  memset(addr, 0, sizeof(struct sockaddr_in));
  addr-> sin_family = AF_INET;
  inet_pton(AF_INET, netString, &(addr -> sin_addr));
  addr -> sin_port = htons(port);

  int ret = bind(sockfd, (const struct sockaddr *) addr, sizeof(struct sockaddr_in));
  if ( ret < 0 ) {
    perror("bind");
    return -1;
  }
  return sockfd;
}

//
// Determine if we should drop a packet with probabilty 'prob'
//
int dropPacket(double prob)
{
  long basis = 10000;
  double sample = random() % basis;
  double samp = sample / basis;

  return  samp <= prob;
}

int
main(int argc, char **argv)
{
  if (argc < 4) {
    fprintf(stderr, "usage: udp-relay in-port out-addr out-port [ drop-probability [re-order-probability] ]\n");
    exit(1);
  }

  int inputPort = atoi(argv[1]);
  struct sockaddr_in inputAddr;
  int inputSocketFd;

  int ret;

  struct sockaddr_in outputAddr;
  char *outputNetString= argv[2];
  int outputPort = atoi(argv[3]);

  double dropProb = 0.0;
  if ( argc > 4) {
    dropProb = atof(argv[4]);
  }

  fprintf(stdout, "Specified input port is *%d*\n", inputPort);
  fprintf(stdout, "Specified network is *%s*\n", outputNetString);
  fprintf(stdout, "Specified port is *%d*\n", outputPort);
  fprintf(stdout, "Drop probability is *%f*\n", dropProb);

  //
  // Create input socket
  //

  inputSocketFd = initSocket(&inputAddr, "0.0.0.0", inputPort);
  if ( inputSocketFd < 0 ) {
    fprintf(stderr, "Unable to open input socket - exit\n");
    exit(1);
  }

  //
  // Prepare address for output
  //

  memset(&outputAddr, 0, sizeof(struct sockaddr_in));
  outputAddr.sin_family = AF_INET;
  inet_pton(AF_INET, outputNetString, &outputAddr.sin_addr);
  outputAddr.sin_port = htons(outputPort);

  //
  // Prepare address to record the originator (non-relay)
  //
  struct sockaddr_in nonRelayAddr;  
  memset(&nonRelayAddr, 0, sizeof(struct sockaddr_in));

  for(;;) {
    //
    // Receive and relay packets
    //
    const int size = 8192;
    char buffer[size];
    struct sockaddr_in fromAddr;
    socklen_t from_size = sizeof(fromAddr);
    char from_name[1024];

    char nonRelayNetString[1024];

    int recvSize = recvfrom(inputSocketFd, buffer, size, 0,
			    (struct sockaddr *) &fromAddr, &from_size);
    if ( recvSize < 0 ) {
      perror("recvfrom");
      exit(1);
    }

    inet_ntop(AF_INET,&fromAddr.sin_addr,
	      from_name, sizeof(from_name));

    inet_ntop(AF_INET,&nonRelayAddr.sin_addr,
	      nonRelayNetString, sizeof(nonRelayNetString));


    if (fromAddr.sin_addr.s_addr == outputAddr.sin_addr.s_addr
	&& fromAddr.sin_port == outputAddr.sin_port) {
      //
      // Relay packet from input to output. We drop the packet with some probability,
      // specified by a command line option
      //

      if ( dropPacket( dropProb ) ) {
	fprintf(stdout,"DROP PACKET: Server -> Relay -> Client %s:%d -> %s:%d \n", 
		from_name, ntohs(fromAddr.sin_port),
		nonRelayNetString, ntohs(nonRelayAddr.sin_port));
      } else {	

	ret = sendto(inputSocketFd, buffer, recvSize, 0,
		     (struct sockaddr *) &nonRelayAddr, sizeof(nonRelayAddr));

	if ( ret < 0 ) {
	  perror("sendto");
	  exit(1);
	}

	fprintf(stdout,"Server -> Relay -> Client %s:%d -> %s:%d \n", 
		from_name, ntohs(fromAddr.sin_port),
		nonRelayNetString, ntohs(nonRelayAddr.sin_port));
      }

    } else {
      //
      // Copy from addr to the nonRelay addr, then forward to end point
      //
      
      if ( dropPacket( dropProb ) ) {
	fprintf(stdout,"DROP PACKET: Client -> Relay -> Server %s:%d -> %s:%d \n", 
		from_name, ntohs(fromAddr.sin_port),
		outputNetString, ntohs(outputAddr.sin_port));

      } else {
	memcpy(&nonRelayAddr, &fromAddr, sizeof(nonRelayAddr));

	ret = sendto(inputSocketFd, buffer, recvSize, 0,
		     (struct sockaddr *) &outputAddr, sizeof(outputAddr));
	if ( ret < 0 ) {
	  perror("sendto");
	  exit(1);
	}
      
	fprintf(stdout,"Client -> Relay -> Server %s:%d -> %s:%d \n", 
		from_name, ntohs(fromAddr.sin_port),
		outputNetString, ntohs(outputAddr.sin_port));
      }
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
