#ifndef __SUM1D_H_
#define __SUM1D_H_

#include <iostream>
#include "Sum.h"

using namespace std;  
                                                        
class Sum1D : public Sum {
  public:
    Sum1D(int n, double min, double max);
    friend Sum1D &operator<<(Sum1D &sum, const double &value);
    friend ostream &operator<<(ostream &str, const Sum1D &sum);
    enum OUTSTREAM_FLAGS {
      CHANNEL = 0x01,
      MINVALUE = 0x02,
      MAXVALUE = 0x04,
      CONTENT = 0x08
    };
    static int outstreamFilter;
    
  protected:
    double value1;
    double value2;
    double minValue;
    double maxValue;  
    int coordinateIndex;
};

#endif
