#ifndef B1StackingAction_H
#define B1StackingAction_H 1

#include "globals.hh"
#include "B1RunAction.hh"
#include "G4UserStackingAction.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

extern std::vector<int> photon_count_array;

class B1StackingAction : public G4UserStackingAction
{
 
public:
	B1StackingAction();
	~B1StackingAction();

public:
	G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	void NewStage();
	void PrepareNewEvent();
	
private:
	
	G4int gammaCounter;	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
