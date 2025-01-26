#include <stdlib.h>
#include "Histogram.h"

Histogram::~Histogram() {
  delete [] array;
};

char *Histogram::getPointer() {
  return (char*) array;
};

int Histogram::getSize() {
  return numberOfChannels * sizeof(int);
};

void Histogram::clear() {
  for (int bin = 0; bin < numberOfChannels; bin++) {
    array[bin]=0;
  }
};

void Histogram::operator<<(const void *a) {
  int eventsOfArray=0;
  for (int bin = 0; bin < numberOfChannels; bin++) {
    array[bin]+=((int *)a)[bin];
    eventsOfArray += ((int *)a)[bin];
  }
  numberOfEvents += eventsOfArray;
};

Histogram &operator<<(Histogram &hist1, const Histogram &hist2) {
  if (hist1.numberOfChannels == hist2.numberOfChannels) {
    for (int bin = 0; bin < hist1.numberOfChannels; bin++) {
      hist1.array[bin]+=hist2.array[bin];
    }
    hist1.numberOfEvents += hist2.numberOfEvents;
  } else {
    cerr << "streaming of Histograms: ";
    cerr << " different number of channels makes no sense!" << endl;
  }

  return hist1;
};
