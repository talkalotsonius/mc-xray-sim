/** (Only a text version, good if no XWindows is available!)
  * expect datas of type double at a tcp/ip socket 
  * (presently the datas should describe the percentage of progress) 
  * the server must beware of numerical problems
  * the client needs a number >= 1.0 for closing the progress bar!  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/shm.h> // shared memory functions and structs
//#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h> // for nice and sleep
#include <sys/types.h>
#include <sys/socket.h>

int sock;
int child_pid;
double *percentagePtr;

int socketProcess(char *ipAddress);
int splitIpAdress(char *ipAddress,char **ipUrl, u_short *port);

int main(int argc, char *argv[])
{
  if (argc < 2){
    printf("Usage: %s url:port\n",argv[0]);
    exit(1);
  }
  nice(19);
  percentagePtr=(double*)calloc(1,sizeof(double));
  socketProcess(argv[1]);
  
  return 0;
}


int socketProcess(char *ipAddress)
{//socketProcess; collects percentage value over tcpip 
  struct sockaddr_in server;   
  struct sockaddr_in client;   
  unsigned int clientLen;   
  struct hostent *hp;   
  char *ipUrl;
  u_short port;

  int tag = 0; // a tag; for sending to server; 
  // the server-code defines the meaning of tag, look there ...
  // if there are no special tags defined, set it to 0 

  // split ipAddress into url and port
  splitIpAdress(ipAddress,&ipUrl,&port);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Opening stream socket");
    exit(1);
  }
  
  bzero((char *) &server, sizeof(server));
  server.sin_family = AF_INET;
  if ((hp = gethostbyname(ipUrl)) == NULL) {
    printf("%s: unknown host\n", ipUrl);
    exit(1);
  }
  
  bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
  server.sin_port = htons(port);
  
  if (connect(sock, (struct sockaddr *) &server, sizeof(server)) != 0){
    printf("Cannot connect to server! Stopping...\n");
    return -1;
  }
  
  printf("connection 2 server ...\n");
  //  *percentagePtr = 0; // gtk knows now: the progress window can be opened
  clientLen = sizeof(client);
  if (getsockname(sock, (struct sockaddr *) &client, &clientLen)) {
    perror("Getting socket name");
    exit(1);
  }
  
  if (clientLen != sizeof(client)) {
    printf("getsockname() overwrote name structure\n");
    exit(1);
  }
  
  if (write(sock, &tag, sizeof(tag)) < 0) // sendig tag to server
  {perror("Writing on stream socket");exit(1);}
  
  if (read(sock, percentagePtr, sizeof(double)) < 0){
    printf("Cannot read data from server! Stopping...\n");
    return -1;
  }
  printf("\n%9.8f%%\n",100.0*(*percentagePtr));
  
  return 0; 
}

int splitIpAdress(char *ipAddress,char **ipUrl, u_short *port) {
  char *doubleDotLocation,*ipPort;
  doubleDotLocation = strpbrk(ipAddress,":");

  if (!doubleDotLocation) {
    fprintf(stderr,"invalid ip address\n");
    kill(0,SIGINT);
  }

  *doubleDotLocation = 0;
  *ipUrl = ipAddress;
  ipPort = doubleDotLocation + 1;
  *port = (u_short) atoi(ipPort);

  if (!*port) {
    fprintf(stderr,"invalid ip port\n");
    kill(0,SIGINT);
  }

  return 0;
}
