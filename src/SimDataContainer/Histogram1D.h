#ifndef __HISTOGRAM1D_H_
#define __HISTOGRAM1D_H_

#include <iostream>
#include "Histogram.h"

using namespace std;  
                                                         
class Histogram1D : public Histogram {
  public:
    Histogram1D(int n, double min, double max);
    friend  Histogram1D &operator<<(Histogram1D &hist, const double &value);
    friend ostream &operator<<(ostream &str, const Histogram1D &hist);
    enum OTUSTREAM_FLAGS
    {
        CHANNEL = 0x01,
        MINVALUE = 0x02,
        MAXVALUE = 0x04,
        CONTENT = 0x08
    };
    static int outstreamFilter;
  protected:
    double minValue;
    double maxValue;
};

#endif
