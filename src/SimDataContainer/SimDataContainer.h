#ifndef __SIMULATIONDATA_H
#define __SIMULATIONDATA_H

class SimDataContainer {
  public:
    virtual char *getPointer() {return 0;};
    virtual int getSize() {return 0;};
    virtual void clear() {} ;
    virtual void operator<<(const void*) {}; 
};

#endif
