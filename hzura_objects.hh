
#ifndef hzura_objects_h
#define hzura_objects_h

#include "Events.hh"

namespace hzura {

  class Photon(){
    Set(Events * event, const int & index){
      tlz.SetPtEtaPhiM(event->Photon_pt[index], event->Photon_eta[index], event->Photon_phi[index], 0 );
    }

    TLorentzVector tlz;
  };

  class Electron(){
    Set(Events * event, const int & index){
      tlz.SetPtEtaPhiM(events->Electron_pt[index], events->Electron_eta[index], events->Electron_phi[index], 0.005 );
    }

    TLorentzVector tlz;
  };

  class Muon(){
    Set(Events * event, const int & index){
      tlz.SetPtEtaPhiM(events->Muon_pt[index], events->Muon_eta[index], events->Muon_phi[index], 0.105 );
    }

    TLorentzVector tlz;
  };

  class Jet(){
    Set(Events * event, const int & index){
      tlz.SetPtEtaPhiM(events->Jet_pt[index], events->Jet_eta[index], events->Jet_phi[index], events->Jet_m[index] );
    }

    TLorentzVector tlz;
  };

  class Met(){
  };
};

#endif
