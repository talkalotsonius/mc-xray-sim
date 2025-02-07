/** (Only a gtk progress bar!)
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
#include <gtk/gtk.h>
#include <unistd.h> // for nice and sleep
#include <sys/types.h>
#include <sys/socket.h>

int sock;
int child_pid;
double *percentagePtr;
int shm_id;

typedef struct {
  GtkWidget *progressbar;
  GtkWidget *window;
  int bProgressUp;
  int nLastPct;
} typProgressData;

typProgressData *pdata = (typProgressData *) NULL;

GtkWidget *table;
GtkWidget *window;
GtkAdjustment *adj;
GtkWidget *label;

void *allocateShm(int size);
int freeShm(void *memPtr);
void catchInt(int sig_num);
void socketProcess(char *ipAddress);
int splitIpAdress(char *ipAddress,char **ipUrl, u_short *port);
gint closeWindow();
gint upDateProgressbar(gpointer data);
int gtkconfig(int argc, char* argv[]);

int main(int argc, char *argv[]) {
  int i;

  if (argc < 2) {
    printf("Usage: %s url:port [further gtk options]\n",argv[0]);
    exit(1);
  }
  nice(19);
  percentagePtr = (double*)allocateShm(sizeof(percentagePtr));
  *percentagePtr = 1;

  signal(SIGINT,catchInt); // handle ctrl-z by catchInt 
 

  //socketProcess will be forked, ipc with parent process by shared memory
  child_pid = fork();
  if (child_pid < 0) {
    perror("fork");
    exit(1);
  }
  if (!child_pid) {
    socketProcess(argv[1]);
  }
  
  // parent process is doing gtk all the time
  // takes 100% CPU time if not connected!!!!!!
  for (i=2; i < argc; i++) {
    argv[i-1] = argv[i]; // correct args for gtk
  }
  gtkconfig(argc,argv);
  gtk_main();

  return 0;
}


void *allocateShm(int size) {
  void *memPtr;
  shm_id = shmget(IPC_PRIVATE,size,IPC_CREAT|IPC_EXCL|0600);
  if (shm_id == -1) {
    perror("shmget"); exit(1);
  }
  memPtr = shmat(shm_id, NULL, 0);
  if (!memPtr) {
    perror("shmat"); exit(1);
  }
  return memPtr;
}

int freeShm(void *memPtr) {
  struct shmid_ds shm_desc;
  if (shmdt((char*)memPtr) == 1) {
    perror("shmdt");
  }
  if (shmctl(shm_id, IPC_RMID, &shm_desc) == -1) {
    perror("shmctl");
  }
  return 0;
}

void catchInt(int sig_num) {
  // "CRTL-Z" occurs; make a clean termination of current process group
  if (!child_pid) {
    close(sock);
    freeShm(percentagePtr);
    kill(0,SIGTERM);
  }
}

void socketProcess(char *ipAddress) {
  //socketProcess; collects percentage value over tcpip 
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

  while(1) { 
    // waiting for server loop; breaks by connection
    printf("waiting 4 server\n");
    while(1) {
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
      
      if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == 0) {
        	break; // if connected, leave loop
      }
      sleep(1); // try next connection after one second
      close(sock);
    }
    printf("connection 2 server ...\n");
    *percentagePtr = 0; // gtk knows now: the progress window can be opened
    clientLen = sizeof(client);

    if (getsockname(sock, (struct sockaddr *) &client, &clientLen)) {
      perror("Getting socket name");
      exit(1);
    }
    
    if (clientLen != sizeof(client)) {
      printf("getsockname() overwrote name structure\n");
      exit(1);
    }
    
    while(1) {
      if (write(sock, &tag, sizeof(tag)) < 0) {
        // sendig tag to server
        perror("Writing on stream socket");
        exit(1);
      }

      if (read(sock, percentagePtr, sizeof(double)) < 0) {
        break; // if server disappears leave loop
      }

      if (*percentagePtr > 1) {
        *percentagePtr = 1; // cliping for progressbar
      }
      if (*percentagePtr < 0) {
        *percentagePtr = 0; //
      }
    }
    close(sock);
    *percentagePtr = 1;// gtk will believe the progressbar is full and close it
  }
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

int gtkconfig(int argc, char* argv[]) {// setup the progress window
  pdata = (typProgressData *) g_malloc (sizeof(typProgressData));
  pdata -> nLastPct = -1;
  pdata -> bProgressUp = TRUE;

  gtk_init (&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "Progress bar");

  gtk_signal_connect(GTK_OBJECT (window), "delete_event",
                      GTK_SIGNAL_FUNC (closeWindow), NULL);

  gtk_container_border_width (GTK_CONTAINER (window), 10);

  // the progress bar is a element of a table
  table = gtk_table_new (2, 1, TRUE);
  gtk_container_add (GTK_CONTAINER(window), table);
  adj = (GtkAdjustment *) gtk_adjustment_new (0, 0, 400, 0, 0, 0);
  label = gtk_label_new ("");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_FILL);
  pdata->progressbar = gtk_progress_bar_new_with_adjustment(adj);
  gtk_table_attach_defaults(GTK_TABLE(table), 
                             pdata -> progressbar, 0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table), 
                             label, 0,1,1,2);

  gtk_widget_show(pdata->progressbar);
  gtk_widget_show(label);

  // the progress bar runs as a idle task in gtk
  gtk_idle_add(upDateProgressbar,NULL);

  gtk_widget_show (table);

  return 0;
}

gint closeWindow() {
  pid_t pid;
  pid = getpid();
  gtk_widget_destroy(window);
  gtk_main_quit();
  kill(0, SIGINT); // give the process group a "ctrl-c"
  return (0);
}

gint upDateProgressbar(gpointer data) {
  char string[100];
  static int windowVisible = 0; // flag for visibility state of progress window

  // freeze percentage it could change during upDateProgressbar
  gdouble frozenPercentage = *percentagePtr; 

  usleep(1000); //necessary, otherwise 100% CPU is catched!!!
  if ((frozenPercentage < 1) && (!windowVisible)) {
    gtk_widget_show(window); 
    windowVisible = 1;
  }
  if (windowVisible) {
    gtk_progress_set_percentage(GTK_PROGRESS(pdata -> progressbar), frozenPercentage);
    sprintf(string, "%9.8f%%", 100.0 * frozenPercentage);
    gtk_label_set_text(GTK_LABEL(label), string);
    while(g_main_iteration(FALSE));
    usleep(250000); // wait 250 ms, means update-time for progressbar
  }  
  if ((frozenPercentage == 1) && (windowVisible)) {
    gtk_widget_hide(window); 
    windowVisible = 0;
  }

  return (1);
}
