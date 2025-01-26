#include <stdlib.h>
#include "Sum3D.h"

int Sum3D::outstreamFilter = Sum3D::CONTENT;

Sum3D::Sum3D(int n1, double min1, double max1,
             int n2, double min2, double max2,
             int n3, double min3, double max3) {
  numberOfEvents = 0;
  coordinateIndex = 0;
  numberOfChannels1 = n1;
  numberOfChannels2 = n2;
  numberOfChannels3 = n3;
  numberOfChannels = n1*n2*n3;
  minValue1 = min1;
  maxValue1 = max1;
  minValue2 = min2;
  maxValue2 = max2;
  minValue3 = min3;
  maxValue3 = max3;
  array = new double[numberOfChannels];

  if (!array) {
    cerr << "Sum3D: not enough memory!" << endl;
    exit(-1);
  }

  clear();
};

Sum3D& operator<<(Sum3D &sum, const double &value) {
  sum.coordinateIndex++;

  switch (sum.coordinateIndex) {
    case 1:
      sum.value1=value;
      break;
    case 2:
      sum.value2=value; 
      break;
    case 3:
      sum.value3=value; 
      break;
    case 4:
      sum.value4=value; 
      break;
  };

  sum.coordinateIndex %= 4;
  if (sum.coordinateIndex) {
     return sum;
  }
  if ((sum.value1 >= sum.minValue1) && (sum.value1 < sum.maxValue1) &&
      (sum.value2 >= sum.minValue2) && (sum.value2 < sum.maxValue2) &&
      (sum.value3 >= sum.minValue3) && (sum.value3 < sum.maxValue3)) {
    int bin1 = int((1.0 * sum.numberOfChannels1 * 
                   (sum.value1 - 1.0 * sum.minValue1)) / 
                   (sum.maxValue1 - sum.minValue1));
    int bin2 = int((1.0 * sum.numberOfChannels2 * 
                   (sum.value2 - 1.0 * sum.minValue2)) / 
                   (sum.maxValue2 - sum.minValue2));
    int bin3 = int((1.0 * sum.numberOfChannels3 * 
                   (sum.value3 - 1.0 * sum.minValue3)) / 
                   (sum.maxValue3 - sum.minValue3));
    int bin = bin1 + 
              bin2 * sum.numberOfChannels1 + 
              bin3 * sum.numberOfChannels1 * sum.numberOfChannels2;
    sum.array[bin] += sum.value4; 
    sum.numberOfEvents++;
  }

  return sum;
};

ostream &operator<<(ostream &str, const Sum3D &sum) {
  double value1 = sum.minValue1;
  double value2 = sum.minValue2;
  double value3 = sum.minValue3;
  double step1 = (sum.maxValue1 - sum.minValue1) / sum.numberOfChannels1;
  double step2 = (sum.maxValue2 - sum.minValue2) / sum.numberOfChannels2;
  double step3 = (sum.maxValue3 - sum.minValue3) / sum.numberOfChannels3;
  int bin;

  for (int i=0; i < sum.numberOfChannels1; i++) {
    for (int j=0; j < sum.numberOfChannels2; j++) {
      for (int k=0; k < sum.numberOfChannels3; k++) {
        if (Sum3D::outstreamFilter & Sum3D::CHANNEL1) 
		      str << i << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MINVALUE1) 
		      str << value1 + i * step1 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MAXVALUE1) 
		      str << value1 + (i+1) * step1 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::CHANNEL2) 
		      str << j << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MINVALUE2) 
		      str << value2 + j * step2 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MAXVALUE2) 
		      str << value2 + (j+1) * step2 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::CHANNEL3) 
		      str << k << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MINVALUE3) 
		      str << value3 + k * step3 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::MAXVALUE3) 
		      str << value3 + (k+1) * step3 << " ";
	      if (Sum3D::outstreamFilter & Sum3D::CONTENT) {
		      bin = i + sum.numberOfChannels1 * j +
                sum.numberOfChannels1 * sum.numberOfChannels2 * k;
          str << sum.array[bin];
        }
        str << endl;
      }
    }
  }

  return str;
}
