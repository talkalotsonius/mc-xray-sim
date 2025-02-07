#ifndef __HISTOGRAM2D_H_
#define __HISTOGRAM2D_H_

#include <iostream>
#include "Histogram.h" 
 
using namespace std;  
                                                        
class Histogram2D : public Histogram {
  public:
    Histogram2D(int n1, double min1, double max1,
                int n2, double min2, double max2);
    friend Histogram2D &operator<<(Histogram2D &hist, const double &value);
    friend ostream &operator<<(ostream &str, const Histogram2D &hist);
    enum OUTSTREAM_FLAGS {
      CHANNEL1 = 0x01,
      CHANNEL2 = 0x02,
      MINVALUE1 = 0x04,
      MINVALUE2 = 0x08,
      MAXVALUE1 = 0x10,
      MAXVALUE2 = 0x20,
      CONTENT = 0x40
    };
    static int outstreamFilter;
    
  protected:
    int numberOfChannels1;
    int numberOfChannels2;
    double value1,value2;
    double minValue1;
    double maxValue1;
    double minValue2;
    double maxValue2;
    int coordinateIndex;
};

#endif
