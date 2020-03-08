#include "B1PrimaryGeneratorAction.hh"
#include <stdio.h>
#include <math.h>
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "time.h"
#include "G4GenericMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double theta_source = 0.5;
int nPrimeries = 1;

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  fMessenger = new G4GenericMessenger(this,"/ABALONE_Event/generator/","Primary generator controls");
  G4GenericMessenger::Command& angleCMD = fMessenger->DeclareProperty("angle",theta_source,"Angle of Event");
  angleCMD.SetParameterName("angle",true);
  angleCMD.SetRange("angle>0");
  angleCMD.SetDefaultValue("0.5");
  
  G4GenericMessenger::Command& particleCMD = fMessenger->DeclareProperty("number",nPrimeries);
  particleCMD.SetParameterName("number",true);
  particleCMD.SetRange("number<999");
  particleCMD.SetDefaultValue("1");
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="e-");

  fParticleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double r0 = 4.995 * cm; //4.99cm

  //theta_source = 0;  //18.6
  //std::cerr<<"generating event.."<<std::endl;
  //std::cerr<<theta_source<<std::endl;
  double phi_source = 0.0; // 2 * pi * G4UniformRand() <-> 45/360
  double theta_source_rad = theta_source/360.0*2*3.1415926;
  G4double height = 1.844 * cm;
  G4double x_particle = (r0 * sin(theta_source_rad) * cos(phi_source)), y_particle = (r0 * sin(theta_source_rad) * sin(phi_source)), z_particle = r0 * abs( cos(theta_source_rad))+ height; //abs
  G4ThreeVector particle_pos = G4ThreeVector(x_particle, y_particle, z_particle); // *** (x_particle, y_particle, z_particle) <-> (0,0,r0)

  G4double x = x_particle, y = y_particle, z = z_particle; //(x_particle, y_particle, z_particle) < -> (0, 0, r0)
  G4double x_dir = -1. * x / sqrt((std::pow(x, 2) + (std::pow(y, 2) + (std::pow(z, 2)))));
  G4double y_dir = -1. * y / sqrt((std::pow(x, 2) + (std::pow(y, 2) + (std::pow(z, 2)))));
  G4double z_dir = -1. * z / sqrt((std::pow(x, 2) + (std::pow(y, 2) + (std::pow(z, 2)))));
 
  G4double x_dir_prime;
  G4double y_dir_prime;
  G4double z_dir_prime;
  
  fParticleGun->SetParticleEnergy(0.25 * eV); // eqv to 10eV
  fParticleGun->SetParticlePosition(particle_pos); //particle_pos <-> temp_particle_pos


  double x_dir_test , y_dir_test , z_dir_test; // for testing the dot product
  double x_dir_prime_test, y_dir_prime_test, z_dir_prime_test;


  double theta_rot=0., phi_rot=0., psi_rot=0.; //rotation angle around the axes
  double dot_product=-1.; 
  
  //int nPrimeries = 1; // rand()%2+1;  // creates 1 - 2 photons // if you want to create 0 - 1 photon -> use rand()%2

  if (nPrimeries!=0){
	  for (int j = 0; j<nPrimeries;j++){

		x_dir_test = x_dir, y_dir_test = y_dir, z_dir_test = z_dir;

		while (dot_product < 0.5){ //around 84.26 degree angle


		  theta_rot = 3.141515926 * G4UniformRand(); //(3.141515926)*G4UniformRand();  //theta_source)/
		  phi_rot = (2. * 3.141515926) * G4UniformRand();//(2 * 3.141515926) * G4UniformRand(); 
		  psi_rot = (2. * 3.141515926) * G4UniformRand();//(2 * 3.141515926) * G4UniformRand();

		  x_dir_prime = (cos(psi_rot) * cos(phi_rot) - cos(theta_rot) * sin(phi_rot) * sin(psi_rot)) * x_dir + (cos(psi_rot) * sin(phi_rot) + cos(theta_rot) * cos(phi_rot) * sin(psi_rot)) * y_dir + (sin(psi_rot) * sin(theta_rot)) * z_dir;
		  y_dir_prime = (-sin(psi_rot) * cos(phi_rot) - cos(theta_rot) * sin(phi_rot) * cos(psi_rot)) * x_dir + (-sin(psi_rot) * sin(phi_rot) + cos(theta_rot) * cos(phi_rot) * cos(psi_rot)) * y_dir + (cos(psi_rot) * sin(theta_rot)) * z_dir;
		  z_dir_prime = sin(theta_rot) * sin(phi_rot) * x_dir - sin(theta_rot) * cos(phi_rot) * y_dir + cos(theta_rot) * z_dir;

		  x_dir_prime_test = x_dir_prime;
		  y_dir_prime_test = y_dir_prime;
		  z_dir_prime_test = z_dir_prime;

		  dot_product = x_dir_test * x_dir_prime_test + y_dir_test * y_dir_prime_test + z_dir_test * z_dir_prime_test; // prevent electrons going to the direction of photocathode
		  //G4cout << "The result of dot product is: " << dot_product << G4endl;
		}

		dot_product = -1.0;

		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_dir_prime, y_dir_prime, z_dir_prime)); // x_dir_prime, y_dir_prime, z_dir_prime<-> 0.,0.,-1 <-> x_dir, y_dir, z_dir
		fParticleGun->GeneratePrimaryVertex(anEvent);
	  }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

