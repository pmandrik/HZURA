
#ifndef hzura_objects_h
#define hzura_objects_h

#include "hzura_cfg.hh"

namespace hzura {

  class Photon {
    public:
    void Init(const int & i){
      tlv.SetPtEtaPhiM(hzura::glob::event->Photons_pt[index], hzura::glob::event->Photons_eta[index], hzura::glob::event->Photons_phi[index], 0 );
    }

    Float_t iso;
    int index;
    TLorentzVector tlv;
  };

  class Electron {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Electrons_pt[index], hzura::glob::event->Electrons_eta[index], hzura::glob::event->Electrons_phi[index], 0.005 );
    }

    int index;
    TLorentzVector tlv;
  };

  class Muon {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Muons_pt[index], hzura::glob::event->Muons_eta[index], hzura::glob::event->Muons_phi[index], 0.105 );
    }

    int index;
    TLorentzVector tlv;
  };

  class Jet {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Jets_pt[index], hzura::glob::event->Jets_eta[index], hzura::glob::event->Jets_phi[index], hzura::glob::event->Jets_m[index] );
      btag_DeepCSV_val     = -1;
      btag_DeepFlavour_val = -1;
    }

    int index;
    bool btag_DeepCSV_isLoose, btag_DeepCSV_isMedium, btag_DeepCSV_isTight;
    bool btag_DeepFlavour_isLoose, btag_DeepFlavour_isMedium, btag_DeepFlavour_isTight;

    TLorentzVector tlv;
    Float_t btag_DeepCSV_val, btag_DeepFlavour_val;
  };

  class Met {
    public:
  };
};

#endif

