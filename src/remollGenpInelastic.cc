#include "remollGenpInelastic.hh"

#include "christy_bosted_inelastic.h"

#include "CLHEP/Random/RandFlat.h"

#include "remollEvent.hh"
#include "remollVertex.hh"
#include "G4Material.hh"

#include "remolltypes.hh"

remollGenpInelastic::remollGenpInelastic(){
    fTh_min =     0.1*deg;
    fTh_max =     5.0*deg;

    fApplyMultScatt = true;
}

remollGenpInelastic::~remollGenpInelastic(){
}

void remollGenpInelastic::SamplePhysics(remollVertex *vert, remollEvent *evt){
    // Generate inelastic event

    double beamE = vert->GetBeamE();
    double mp    = proton_mass_c2;

    double th = acos(CLHEP::RandFlat::shoot(cos(fTh_max), cos(fTh_min)));
    double ph = CLHEP::RandFlat::shoot(0.0, 2.0*pi);
    double efmax = mp*beamE/(mp + beamE*(1.0-cos(th)));;
    double ef = CLHEP::RandFlat::shoot(0.0, beamE);

    double thissigma_p = sigma_p( beamE/GeV, th, ef/GeV )*microbarn;
    double thissigma_n = sigma_n( beamE/GeV, th, ef/GeV )*microbarn;

    double sigmatot = thissigma_p*vert->GetMaterial()->GetZ() +
	//  Effective neutron number...  I don't like it either  SPR 2/14/2013
	thissigma_n*(vert->GetMaterial()->GetA()*mole/g - vert->GetMaterial()->GetZ());

    double V = 2.0*pi*(cos(fTh_min) - cos(fTh_max))*efmax;

    evt->SetEffCrossSection(sigmatot*V);

    if( vert->GetMaterial()->GetNumberOfElements() != 1 ){
	G4cerr << __FILE__ << " line " << __LINE__ << 
	    ": Error!  Some lazy programmer didn't account for complex materials in the moller process!" << G4endl;
	exit(1);
    }

//    G4double APV = electron_mass_c2*beamE*GF*4.0*sin(thcom)*sin(thcom)*QeW/(sqrt(2.0)*pi*alpha*pow(3.0+cos(thcom)*cos(thcom),2.0));
//    FIXME
    G4double APV = 0.0;

    evt->SetAsymmetry(APV);

    double Q2 = 2.0*beamE*ef*(1.0-cos(th));
    evt->SetQ2( Q2 );
    evt->SetW2( mp*mp + 2.0*mp*(beamE-ef) - Q2 );

    evt->ProduceNewParticle( G4ThreeVector(0.0, 0.0, 0.0), 
	                     G4ThreeVector(ef*sin(th)*cos(ph), ef*sin(th)*sin(ph), ef*cos(th) ), 
			     "e-" );

    return;

}