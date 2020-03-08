/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "B1RunAction.hh"
//#include "B1StackingAction.hh"

#include <G4HCofThisEvent.hh>
#include <G4DCofThisEvent.hh>
#include <G4VHitsCollection.hh>
#include <G4VDigiCollection.hh>
#include <G4Event.hh>



class B1EventAction : public G4UserEventAction
{

B1StackingAction B1Stackingvariables_event;

public:
	B1EventAction();
	virtual ~B1EventAction();

	virtual void  BeginOfEventAction(const G4Event* event);
	virtual void    EndOfEventAction(const G4Event* event);

private:
	// methods
	//B1CalorHitsCollection* GetHitsCollection(G4int hcID,const G4Event* event) const;
	void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength) const;

	// data members                   
	G4int  fAbsHCID;
	G4int  fCoatingHCID;
	G4int  fringHCID;
	G4int  fbaseHCID;
	G4int fallHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
