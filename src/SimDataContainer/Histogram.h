#ifndef __HISTOGRAM_H_
#define __HISTOGRAM_H_

#include <iostream>
#include "SimDataContainer.h"    

using namespace std;  

class Histogram : public SimDataContainer
{
  public:
    ~Histogram();
    char *getPointer();
    int getSize();
    void clear();
    int getNumberOfEvents() {return numberOfEvents;};
    void operator<<(const void *a);
    friend Histogram &operator<<(Histogram &hist1, const Histogram &hist2);
    int *array;
    
  protected:
    int numberOfChannels;
    int numberOfEvents;
};

#endif
