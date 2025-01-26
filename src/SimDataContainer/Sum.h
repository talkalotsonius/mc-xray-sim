#ifndef __SUM_H_
#define __SUM_H_

#include <iostream>
#include "SimDataContainer.h"

using namespace std;  
                                                         
class Sum : public SimDataContainer {
  public:
    ~Sum();
    char *getPointer();
    int getSize();
    void clear();
    int getNumberOfEvents() {return numberOfEvents;};
    void normalise();
    void operator<<(const void *a);
    friend Sum &operator<<(Sum &sum1, const Sum &sum2);
    double *array;
    
  protected:
    int numberOfChannels;
    int numberOfEvents;
};

#endif

