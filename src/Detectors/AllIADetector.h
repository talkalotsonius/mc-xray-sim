#ifndef __ALLIADETECTOR_H
#define __ALLIADETECTOR_H

#include "gismo/Detector.h"
#include "gismo/MCParticle.h"
#include "lscat/LSCATData.h"
#include "InteractionType.h"

class AllIADetector : public Detector {
  public:
    AllIADetector(int maxNOI);
    ~AllIADetector();
    void clear();
    void score(MCParticle* particle);
    void generateResponse();
    long getNumberOfInteractions();
    double getStartX(int noOfInteraction);
    double getStartY(int noOfInteraction);
    double getStartZ(int noOfInteraction);
    double getEndX(int noOfInteraction);
    double getEndY(int noOfInteraction);
    double getEndZ(int noOfInteraction);
    double getTheta(int noOfInteraction);    // polar angle of step direction
    double getPhi(int noOfInteraction);      // azimuthal angle of step direction
    double getThetaAMS(int noOfInteraction); // polar angle multiple scattering
    double getPhiAMS(int noOfInteraction);   // azimuthal angle after mult. scatt.
    double getEnergy(int noOfInteraction);
    /** getIAEnergyLoss
      * Energy lost during an interaction to the daughter particles 
      * which are not of same type as the mother particle */
    double getIAEnergyLoss(int noOfInteraction); 
    double getMissingEnergy(int noOfInteraction); 
    /** getEnergyLoss
      * Energy lost during step (only charged particles) */
    double getEnergyLoss(int noOfInteraction);
    double getStepLength(int noOfInteraction);
    long getParticleID(int noOfInteraction);
    InteractionType getInteractionType(int noOfInteraction);
    const char * getParticleName(int noOfInteraction);
  private:
    long noOfInteraction,maxNOI;
    double *startX,*startY,*startZ;
    double *endX,*endY,*endZ;
    double *e,*energyLoss;
    double *iAEnergyLoss,*missingEnergy;
    double *phi,*phiAMS;
    double *theta,*thetaAMS; 
    double *stepLength;
    long *particleID;
    InteractionType *iType;
    const char **particleName;
};

inline long AllIADetector::getNumberOfInteractions() {
  return noOfInteraction;
}
inline double AllIADetector::getStartX(int i) {
  return startX[i];
}
inline double AllIADetector::getStartY(int i) {
  return startY[i];
}
inline double AllIADetector::getStartZ(int i) {
  return startZ[i];
}
inline double AllIADetector::getEndX(int i) {
  return endX[i];
}
inline double AllIADetector::getEndY(int i) {
  return endY[i];
}
inline double AllIADetector::getEndZ(int i) {
  return endZ[i];
}
inline double AllIADetector::getTheta(int i) {
  return theta[i];
}
inline double AllIADetector::getThetaAMS(int i) {
  return thetaAMS[i];
}
inline double AllIADetector::getPhi(int i) {
  return phi[i];
}
inline double AllIADetector::getPhiAMS(int i) {
  return phiAMS[i];
}
inline double AllIADetector::getEnergy(int i) {
  return e[i];
}
inline double AllIADetector::getIAEnergyLoss(int i) {
  return iAEnergyLoss[i];
}
inline double AllIADetector::getMissingEnergy(int i) {
  return missingEnergy[i];
}
inline double AllIADetector::getEnergyLoss(int i) {
  return energyLoss[i];
}
inline double AllIADetector::getStepLength(int i) {
  return stepLength[i];
}
inline long AllIADetector::getParticleID(int i) {
  return particleID[i];
}
inline InteractionType AllIADetector::getInteractionType(int i) {
  return iType[i];
}
inline const char *AllIADetector::getParticleName(int i) {
  return particleName[i];
}

#endif
