#ifndef __SUM3D_H_
#define __SUM3D_H_

#include <iostream>
#include "Sum.h"

using namespace std;
                                                     
class Sum3D : public Sum {
  public:
    Sum3D(int n1, double min1, double max1,
          int n2, double min2, double max2,
          int n3, double min3, double max3);
    friend Sum3D &operator<<(Sum3D &sum, const double &value);
    friend ostream &operator<<(ostream &str, const Sum3D &sum);
    enum OUTSTREAM_FLAGS {
      CHANNEL1 = 0x001,
      CHANNEL2 = 0x002,
      CHANNEL3 = 0x004,
      MINVALUE1 = 0x008,
      MINVALUE2 = 0x010,
      MINVALUE3 = 0x020,
      MAXVALUE1 = 0x040,
      MAXVALUE2 = 0x080,
      MAXVALUE3 = 0x100,
      CONTENT = 0x200
    };
    static int outstreamFilter;

  protected:
    int numberOfChannels1;
    int numberOfChannels2;
    int numberOfChannels3;
    double value1,value2,value3,value4;
    double minValue1;
    double maxValue1;
    double minValue2;
    double maxValue2;
    double minValue3;
    double maxValue3;
    int coordinateIndex;
};

#endif



