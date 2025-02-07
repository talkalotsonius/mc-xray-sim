#ifndef __TREEDETECTOR_H
#define __TREEDETECTOR_H

#include "gismo/Detector.h"
#include "gismo/MCParticle.h"
#include "lscat/LSCATData.h"
#include "InteractionType.h"
#define maxNOChildren 10

class TreeDetector : public Detector {
  public:
    TreeDetector(int maxNOI);
    ~TreeDetector();
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
    double getTheta(int noOfInteraction);     // polar angle of step direction
    double getPhi(int noOfInteraction);       // azimuthal angle of step direction
    double getThetaAMS(int noOfInteraction);  // polar angle multiple scattering
    double getPhiAMS(int noOfInteraction);    // azimuthal angle after mult. scatt.
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

    // daughter particles
    int getNumberOfChildren(int noOfInteraction);
    long getChildID(int noOfInteraction, int noOfChildren);
    double getChildEnergy(int noOfInteraction, int noOfChildren);
    double getChildKineticEnergy(int noOfInteraction, int noOfChildren);
    double getChildMass(int noOfInteraction, int noOfChildren);
    double getChildStartX(int noOfInteraction, int noOfChildren);
    double getChildStartY(int noOfInteraction, int noOfChildren);
    double getChildStartZ(int noOfInteraction, int noOfChildren);
    double getChildTheta(int noOfInteraction, int noOfChildren);
    double getChildPhi(int noOfInteraction, int noOfChildren);
    
    const char * getParticleName(int noOfInteraction);
    
  private:
    long noOfInteraction, maxNOI;
    double *startX,*startY,*startZ;
    double *endX,*endY,*endZ;
    double *e,*energyLoss;
    double *iAEnergyLoss,*missingEnergy;
    double *MOLLEREnergyLoss;
    double *highestMOLLERe;
    double *phi,*phiAMS;
    double *theta,*thetaAMS; 
    double *stepLength;
    long *particleID;
    InteractionType *iType;
    const char **particleName;
    
    int *numOfChildren;
    long (*ChildID)[maxNOChildren];
    double (*ChildEnergy)[maxNOChildren];
    double (*ChildMass)[maxNOChildren];
    double (*ChildStartX)[maxNOChildren], (*ChildStartY)[maxNOChildren], (*ChildStartZ)[maxNOChildren];
    double (*ChildTheta)[maxNOChildren], (*ChildPhi)[maxNOChildren];
};

inline long TreeDetector::getNumberOfInteractions() {
  return noOfInteraction;
}
inline double TreeDetector::getStartX(int i) {
  return startX[i];
}
inline double TreeDetector::getStartY(int i) {
  return startY[i];
}
inline double TreeDetector::getStartZ(int i) {
  return startZ[i];
}
inline double TreeDetector::getEndX(int i) {
  return endX[i];
}
inline double TreeDetector::getEndY(int i) {
  return endY[i];
}
inline double TreeDetector::getEndZ(int i) {
  return endZ[i];
}
inline double TreeDetector::getTheta(int i) {
  return theta[i];
}
inline double TreeDetector::getThetaAMS(int i) {
  return thetaAMS[i];
}
inline double TreeDetector::getPhi(int i) {
  return phi[i];
}
inline double TreeDetector::getPhiAMS(int i) {
  return phiAMS[i];
}
inline double TreeDetector::getEnergy(int i) {
  return e[i];
}
inline double TreeDetector::getIAEnergyLoss(int i) {
  return iAEnergyLoss[i];
}
inline double TreeDetector::getMissingEnergy(int i) {
  return missingEnergy[i];
}
inline double TreeDetector::getEnergyLoss(int i) {
  return energyLoss[i];
}
inline double TreeDetector::getStepLength(int i) {
  return stepLength[i];
}
inline long TreeDetector::getParticleID(int i) {
  return particleID[i];
}
inline InteractionType TreeDetector::getInteractionType(int i) {
  return iType[i];
}
inline const char *TreeDetector::getParticleName(int i) {
  return particleName[i];
}

inline int TreeDetector::getNumberOfChildren(int i) {
  return numOfChildren[i];
}
inline long TreeDetector::getChildID(int i, int c) {
  return ChildID[i][c];
}
inline double TreeDetector::getChildKineticEnergy(int i, int c) {
  return ChildEnergy[i][c] - ChildMass[i][c];
}
inline double TreeDetector::getChildMass(int i, int c) {
  return ChildMass[i][c];
}
inline double TreeDetector::getChildEnergy(int i, int c) {
  return ChildEnergy[i][c];
}
inline double TreeDetector::getChildStartX(int i, int c) {
  return ChildStartX[i][c];
}
inline double TreeDetector::getChildStartY(int i, int c) {
  return ChildStartY[i][c];
}
inline double TreeDetector::getChildStartZ(int i, int c) {
  return ChildStartZ[i][c];
}
inline double TreeDetector::getChildTheta(int i, int c) {
  return ChildTheta[i][c];
}
inline double TreeDetector::getChildPhi(int i, int c) {
  return ChildPhi[i][c];
}

#endif
