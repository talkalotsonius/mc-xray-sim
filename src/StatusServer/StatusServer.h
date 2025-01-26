/** StatusServer returns a value as a pointer to status information.
  * A more flexible solution would be a dedicated status information type. */

#ifndef __STATUSSERVER_H
#define __STATUSSERVER_H

class StatusServer {
  public:
  virtual double *start() {
    return 0;
  }
  virtual int  kill() {
    return 0;
  }
  protected:
    int port;
};

#endif
