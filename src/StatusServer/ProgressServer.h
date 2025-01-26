/** ProgressServer sends datas of type double to a tcp/ip socket
  * (presently the datas should describe the percentage of progress)
  * the server must beware of numerical problems
  * the client needs a number >= 1.0 for closing the progress bar! 
  * progress bar server */

#ifndef __PROGRESSSERVER_H
#define __PROGRESSSERVER_H

extern "C" {
  double *initStatusServer(int port);
  int killStatusServer();
}

#include "StatusServer.h"

class ProgressServer : public StatusServer {
  public:
    ProgressServer(int p) {
      port = p;
    }
    double *start() {
      return initStatusServer(port);
    }
    int  kill() {
      return killStatusServer();
    }
};

#endif
