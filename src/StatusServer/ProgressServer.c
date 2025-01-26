#ifndef WIN32
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/shm.h> // shared memory functions and structs
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#endif

double *initStatusServer(int port);
int killStatusServer();

static double *percentagePtr;

#ifndef WIN32
static int listeningSocket, connectedSocket;  
static pid_t childPid;
static int shm_id;

static void serverProcess(int port);
static void catchInt(int sig_num);
static void catchTerm(int sig_num);
static void *allocateShm(int size);
static int freeShm(void *memPtr);
static void cleanTermination();
#endif

double *initStatusServer(int port) {
#ifndef WIN32
  percentagePtr = (double*)(allocateShm(sizeof(percentagePtr)));

  // socketProcess will be forked, ipc with parent process by shared memory
  childPid = fork();
  if (childPid < 0) {
    perror("fork");
    exit(1);
  }

  if (childPid) {
    signal(SIGINT,catchInt); // handle "CRTL-C" via catchIntChild
    signal(SIGTERM,catchTerm); // handle SIGTERM via catchTermChild
  }
  
  if (!childPid) {
    serverProcess(port);
  }
#endif

  return percentagePtr;
}

int killStatusServer() {
#ifndef WIN32
  printf("terminating statusserver: %i\n",childPid);
  cleanTermination();
  printf("killing statusserver: %i\n",childPid);
  kill(childPid,SIGTERM);
#endif

  return 0;
}

static void *allocateShm(int size) {
  void *memPtr;
#ifndef WIN32
  shm_id = shmget(IPC_PRIVATE,size,IPC_CREAT|IPC_EXCL|0600);
  if (shm_id == -1) {
    perror("shmget"); 
    exit(1);
  }
  memPtr = shmat(shm_id, NULL, 0);
  if (!memPtr) {
    perror("shmat"); 
    exit(1);
  }
#endif

  return memPtr;
}

static int freeShm(void *memPtr) {
#ifndef WIN32
  struct shmid_ds shm_desc;
  if (shmdt((char*)(memPtr)) == 1) {
    perror("shmdt");
  }
  if (shmctl(shm_id, IPC_RMID, &shm_desc) == -1) {
    perror("shmctl");
  }
#endif

  return 0;
}

static void catchTerm(int sig_num) {
// When SIGTERM occurs, make a clean termination of current process group 
#ifndef WIN32
  if (childPid) {
    killStatusServer();
    kill(0,SIGKILL);
  }
#endif
}

static void catchInt(int sig_num) {
// When "CRTL-C" occurs, make a clean termination of current process group 
#ifndef WIN32
  if (childPid) {
    killStatusServer();
    kill(0,SIGKILL);
  }
#endif
}

static void cleanTermination() {
#ifndef WIN32
  if (childPid) {
    close(listeningSocket);
    close(connectedSocket);
    freeShm(percentagePtr);
  }
#endif
}

static void serverProcess(int port) {
#ifndef WIN32
  struct sockaddr_in server;  
  struct sockaddr_in client;  
  unsigned int clientLen;  
  int tag; // a tag; recieved by client

  if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("opening socket"); 
    exit(1);
  }
  
  bzero((char *) &server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(listeningSocket, (struct sockaddr *) &server, sizeof(server))) {
    perror("binding socket");
    exit(1);
  }
  
  listen(listeningSocket, 5);
  
  while (1) {
    clientLen = sizeof(client);
    connectedSocket = accept(listeningSocket, 
                             (struct sockaddr *) &client, 
                             &clientLen);

    if (connectedSocket == -1) {
      perror("accept");
      exit(1);
    }
    
    if (clientLen != sizeof(client)) {
      perror("accept");
      exit(1);
    }

    while (1) {
      if (read(connectedSocket, &tag, sizeof(tag)) <= 0) {
        break; //client disappears or has sent an EOF; leave loop
      }
      if (write(connectedSocket, percentagePtr, sizeof(double)) < 0) {
        perror("writing on socket");
        exit(1);
      }
      usleep(250000); // next percentage message after at least 250 ms
    }
  }
#endif
}
