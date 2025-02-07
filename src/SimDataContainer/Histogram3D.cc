/************************************************************************

     Histogram3D.cpp
     Copyright (C) 2002, 2003, 2004 Juergen Giersch

     This file is part of ROSI.

     ROSI is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     ROSI is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
  
     You should have received a copy of the GNU General Public License
     along with ROSI; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
     02111-1307  USA
 
     Juergen Giersch
     Physikalisches Institut Abteilung 4
     Erwin-Rommel-Strasse 1
     91058 Erlangen
     Germany

     E-Mail: juergen.giersch@physik.uni-erlangen.de

************************************************************************/ 

#include "Histogram3D.h"
#include <stdlib.h>

int Histogram3D::outstreamFilter = Histogram3D::CONTENT;

Histogram3D::Histogram3D(int n1, double min1, double max1,
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
  array = new int[numberOfChannels];

  if (!array) {
      cerr << "Histogram3D: not enough memory!" << endl;
      exit(-1);
  }

  clear();
};

Histogram3D& operator<<(Histogram3D &hist, const double &value) {
  hist.coordinateIndex++;

  switch (hist.coordinateIndex) {
    case 1: 
      hist.value1=value; 
      break;
    case 2: 
      hist.value2=value; 
      break;
    case 3: 
      hist.value3=value;
      break;
  };

  hist.coordinateIndex %= 3;
  if (hist.coordinateIndex){
    return hist;
  }
  if ((hist.value1 >= hist.minValue1) && (hist.value1 < hist.maxValue1) &&
      (hist.value2 >= hist.minValue2) && (hist.value2 < hist.maxValue2) &&
      (hist.value3 >= hist.minValue3) && (hist.value3 < hist.maxValue3)) {
    int bin1 = int((1.0*hist.numberOfChannels1 * 
                   (hist.value1-1.0*hist.minValue1)) / 
		               (hist.maxValue1-hist.minValue1));
    int bin2 = int((1.0*hist.numberOfChannels2 * 
		               (hist.value2-1.0*hist.minValue2)) / 
		               (hist.maxValue2-hist.minValue2));
    int bin3 = int((1.0*hist.numberOfChannels3 * 
		               (hist.value3-1.0*hist.minValue3)) / 
		               (hist.maxValue3-hist.minValue3));
    int bin = bin1 + bin2*hist.numberOfChannels1 
                   + bin3*hist.numberOfChannels1*hist.numberOfChannels2;
    hist.array[bin]++;
    hist.numberOfEvents++;
  }

  return hist;
};

ostream &operator<<(ostream &str, const Histogram3D &hist) {
  double minValue1 = hist.minValue1;
  double minValue2 = hist.minValue2;
  double minValue3 = hist.minValue3;
  double step1 = (hist.maxValue1-hist.minValue1)/hist.numberOfChannels1;
  double step2 = (hist.maxValue2-hist.minValue2)/hist.numberOfChannels2;
  double step3 = (hist.maxValue3-hist.minValue3)/hist.numberOfChannels3;
  int bin;

  for (int i=0; i<hist.numberOfChannels1; i++) {
    for (int j=0; j<hist.numberOfChannels2; j++) {
      for (int k=0; k<hist.numberOfChannels3; k++) {
        if (Histogram3D::outstreamFilter & Histogram3D::CHANNEL1) 
          str << i << " ";
        if (Histogram3D::outstreamFilter & Histogram3D::MINVALUE1) 
		      str << minValue1 + i * step1 << " ";
        if (Histogram3D::outstreamFilter & Histogram3D::MAXVALUE1) 
		      str << minValue1 + (i+1) * step1 << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::CHANNEL2) 
		      str << j << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::MINVALUE2) 
		      str << minValue2 + j * step2 << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::MAXVALUE2) 
		      str << minValue2 + (j+1) * step2 << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::CHANNEL3) 
		      str << k << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::MINVALUE3) 
		      str << minValue3 + k * step3 << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::MAXVALUE3) 
		      str << minValue3 + (k+1) * step3 << " ";
	      if (Histogram3D::outstreamFilter & Histogram3D::CONTENT) {
		      bin = i + hist.numberOfChannels1 * j
                  + hist.numberOfChannels1 * hist.numberOfChannels2 * k;
          str << hist.array[bin];
        }
        str << endl;
      }
    }
  }

  return str;
}

