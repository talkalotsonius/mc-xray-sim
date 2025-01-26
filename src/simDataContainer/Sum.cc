#include "Sum.h"
#include <stdlib.h>

Sum::~Sum() {
  delete [] array;
};

char *Sum::getPointer() {
  return (char*) array;
};

int Sum::getSize() {
  return numberOfChannels * sizeof(double);
};

void Sum::clear() {
  for (int bin = 0; bin < numberOfChannels; bin++) 
    array[bin]=0;
};

void Sum::normalise() {
  double max=0;

  for (int bin=0; bin < numberOfChannels; bin++) {
    if (array[bin] > max) {
      max=array[bin];
    } 
  }
    
  for (int bin=0; bin < numberOfChannels; bin++) {
    array[bin] /= max;
  }
}

void Sum::operator<<(const void *a) {
  int eventsOfArray=0;

  for (int bin = 0; bin<numberOfChannels; bin++) {
    array[bin] += ((double *)a)[bin];
    eventsOfArray += ((int *)a)[bin];
  }
  numberOfEvents += eventsOfArray;
};

Sum &operator<<(Sum &sum1, const Sum &sum2) {
  if (sum1.numberOfChannels == sum2.numberOfChannels) {
    for (int bin = 0; bin < sum1.numberOfChannels; bin++) {
      sum1.array[bin] += sum2.array[bin];
    }
    sum1.numberOfEvents += sum2.numberOfEvents;
  } else {
    cerr << "streaming of sums: ";
    cerr << " different number of channels doesn't make sense!" << endl;
  }

  return sum1;
};
