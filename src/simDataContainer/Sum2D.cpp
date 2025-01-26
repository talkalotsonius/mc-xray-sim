#include <stdlib.h>
#include "Sum2D.h"

int Sum2D::outstreamFilter = Sum2D::CONTENT;

Sum2D::Sum2D(int n1, double min1, double max1,
             int n2, double min2, double max2) {
  numberOfEvents = 0;
  coordinateIndex = 0;
  numberOfChannels1 = n1;
  numberOfChannels2 = n2;
  numberOfChannels = n1*n2;
  minValue1 = min1;
  maxValue1 = max1;
  minValue2 = min2;
  maxValue2 = max2;
  array = new double[numberOfChannels];

  if (!array) {
    cerr << "Sum2D: not enough memory!" << endl;
    exit(-1);
  }

  clear();
};

Sum2D& operator<<(Sum2D &sum, const double &value){
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
  };

  sum.coordinateIndex %= 3;
  if (sum.coordinateIndex) {
    return sum;
  }
  if ((sum.value1 >= sum.minValue1) && (sum.value1 < sum.maxValue1) &&
      (sum.value2 >= sum.minValue2) && (sum.value2 < sum.maxValue2)) {
    int bin1 = int((1.0*sum.numberOfChannels1 * 
                   (sum.value1-1.0*sum.minValue1)) / 
                   (sum.maxValue1-sum.minValue1));
    int bin2 = int((1.0*sum.numberOfChannels2 * 
                   (sum.value2-1.0*sum.minValue2)) / 
                   (sum.maxValue2-sum.minValue2));
    int bin = bin1+sum.numberOfChannels1*bin2;
    sum.array[bin] += sum.value3;
    sum.numberOfEvents++;
  }

  return sum;
};

ostream &operator<<(ostream &str, const Sum2D &sum) {
  double minValue1 = sum.minValue1;
  double minValue2 = sum.minValue2;
  double step1 = (sum.maxValue1-sum.minValue1) / sum.numberOfChannels1;
  double step2 = (sum.maxValue2-sum.minValue2) / sum.numberOfChannels2;
  for (int i=0; i < sum.numberOfChannels1; i++) {
    for (int j=0; j < sum.numberOfChannels2; j++) {
      if (Sum2D::outstreamFilter & Sum2D::CHANNEL1) 
	      str << i << " ";
      if (Sum2D::outstreamFilter & Sum2D::MINVALUE1) 
	      str << minValue1 + i * step1 << " ";
      if (Sum2D::outstreamFilter & Sum2D::MAXVALUE1) 
	      str << minValue1 + (i+1) * step1 << " ";
      if (Sum2D::outstreamFilter & Sum2D::CHANNEL2) 
	      str << j << " ";
      if (Sum2D::outstreamFilter & Sum2D::MINVALUE2) 
	      str << minValue2 + j * step2 << " ";
      if (Sum2D::outstreamFilter & Sum2D::MAXVALUE2) 
	      str << minValue2 + (j+1) * step2 << " ";
      if (Sum2D::outstreamFilter & Sum2D::CONTENT) 
	      str << sum.array[i + sum.numberOfChannels1 * j];
      str << endl;
    }
  }
  return str;
}
