#include <stdlib.h>
#include "Histogram2D.h"


int Histogram2D::outstreamFilter = Histogram2D::CONTENT;

Histogram2D::Histogram2D(int n1, double min1, double max1,
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
  array = new int[numberOfChannels];

  if (!array) {
      cerr << "Histogram2D: not enough memory!" << endl;
      exit(-1);
  }
  
  clear();
};

Histogram2D& operator<<(Histogram2D &hist, const double &value) {
  hist.coordinateIndex++;

  switch (hist.coordinateIndex) {
    case 1: 
      hist.value1=value; 
      break;
    case 2: 
      hist.value2=value;
      break;
  };

  hist.coordinateIndex %= 2;
  if (hist.coordinateIndex){
    return hist;
  } 
  if ((hist.value1 >= hist.minValue1) && (hist.value1 < hist.maxValue1) &&
      (hist.value2 >= hist.minValue2) && (hist.value2 < hist.maxValue2)) {
    int bin1 = int((1.0 * hist.numberOfChannels1 * 
                   (hist.value1 - 1.0 * hist.minValue1)) / 
                   (hist.maxValue1 - hist.minValue1));
    int bin2 = int((1.0 * hist.numberOfChannels2 * 
                   (hist.value2 - 1.0 * hist.minValue2)) / 
                   (hist.maxValue2 - hist.minValue2));
    int bin = bin1 + hist.numberOfChannels1 * bin2;

    hist.array[bin]++;
    hist.numberOfEvents++;
  }

  return hist;
};

ostream &operator<<(ostream &str, const Histogram2D &hist) {
  double minValue1 = hist.minValue1;
  double minValue2 = hist.minValue2;
  double step1 = (hist.maxValue1-hist.minValue1) / hist.numberOfChannels1;
  double step2 = (hist.maxValue2-hist.minValue2) / hist.numberOfChannels2;

  for (int i=0; i < hist.numberOfChannels1; i++) {
    for (int j=0; j < hist.numberOfChannels2; j++) {
      if (Histogram2D::outstreamFilter & Histogram2D::CHANNEL1) 
	      str << i << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::MINVALUE1) 
	      str << minValue1 + i * step1 << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::MAXVALUE1) 
	      str << minValue1 + (i+1) * step1 << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::CHANNEL2) 
	      str << j << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::MINVALUE2) 
	      str << minValue2 + j * step2 << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::MAXVALUE2) 
	      str << minValue2 + (j+1) * step2 << " ";
      if (Histogram2D::outstreamFilter & Histogram2D::CONTENT) 
	      str << hist.array[i + hist.numberOfChannels1 * j];
      str << endl;
    }
  }
  
  return str;
}
