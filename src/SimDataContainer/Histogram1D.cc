#include <stdlib.h>
#include "Histogram1D.h"

int Histogram1D::outstreamFilter = Histogram1D::CONTENT;

Histogram1D::Histogram1D(int n, double min, double max) {
  numberOfChannels = n;
  minValue = min;
  maxValue = max;
  array = new int[numberOfChannels];

  if (!array) {
    cerr << "Histogram1D: not enough memory!" << endl;
    exit(-1);
  }
  
  clear();
};

Histogram1D& operator<<(Histogram1D &hist, const double &value) {
  if ((value >= hist.minValue) && (value < hist.maxValue)) {
    int bin = int((1.0*hist.numberOfChannels * (value-1.0*hist.minValue)) / 
                  (hist.maxValue-1.0*hist.minValue));
    hist.array[bin]++;
    hist.numberOfEvents++;
  }
  
  return hist;
};

ostream &operator<<(ostream &str, const Histogram1D &hist) {
  double minValue = hist.minValue;
  double step = (hist.maxValue-hist.minValue)/hist.numberOfChannels;

  for (int i=0;i<hist.numberOfChannels;i++) {
      if (Histogram1D::outstreamFilter & Histogram1D::CHANNEL) 
	      str << i << " ";
      if (Histogram1D::outstreamFilter & Histogram1D::MINVALUE) 
	      str << minValue+i*step << " ";
      if (Histogram1D::outstreamFilter & Histogram1D::MAXVALUE) 
	      str << minValue+(i+1)*step << " ";
      if (Histogram1D::outstreamFilter & Histogram1D::CONTENT) 
	      str << hist.array[i];
      str << endl;
  }

  return str;
}
