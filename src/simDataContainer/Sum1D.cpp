#include "Sum1D.h"
#include <stdlib.h>

int Sum1D::outstreamFilter = Sum1D::CONTENT;

Sum1D::Sum1D(int n, double min, double max) {
    numberOfEvents = 0;
    numberOfChannels = n;
    coordinateIndex = 0;
    minValue = min;
    maxValue = max;
    array = new double[numberOfChannels];

    if (!array) {
      cerr << "Sum1D: not enough memory!" << endl;
      exit(-1);
    }

    clear();
};

Sum1D& operator<<(Sum1D &sum, const double &value) {
  sum.coordinateIndex++;

  switch (sum.coordinateIndex) {
    case 1: 
      sum.value1=value; 
      break;
    case 2: 
      sum.value2=value; 
      break;
  };

  sum.coordinateIndex %= 2;
  if (sum.coordinateIndex) {
    return sum;
  }
  if ((sum.value1 >= sum.minValue) && (sum.value1 < sum.maxValue)) { 
    int bin = int((1.0*sum.numberOfChannels * 
                  (sum.value1-1.0*sum.minValue)) / 
                  (sum.maxValue-sum.minValue));
    sum.array[bin] += sum.value2;
	  sum.numberOfEvents++;
  }

  return sum;
};

ostream &operator<<(ostream &str, const Sum1D &sum) {
  double minValue = sum.minValue;
  double step = (sum.maxValue - sum.minValue) / sum.numberOfChannels;

  for (int i=0; i < sum.numberOfChannels; i++) {
    if (Sum1D::outstreamFilter & Sum1D::CHANNEL) 
      str << i << " ";
    if (Sum1D::outstreamFilter & Sum1D::MINVALUE) 
	    str << minValue + i * step << " ";
    if (Sum1D::outstreamFilter & Sum1D::MAXVALUE) 
	    str << minValue + (i+1) * step << " ";
    if (Sum1D::outstreamFilter & Sum1D::CONTENT) 
	    str << sum.array[i];
    str << endl;
  }

  return str;
}
