
#ifndef hzura_objects_h
#define hzura_objects_h

#include "hzura_cfg.hh"

namespace hzura {

  class Particle {
    public:
    Particle(){
      index = -1;
    }
    
    virtual inline int Charge();

    int index;
    TLorentzVector tlv;

    std::vector<Particle*> mothers;
  };

  class Photon : public Particle {
    public:
    void Init(const int & i){
      tlv.SetPtEtaPhiM(hzura::glob::event->Photons_pt[index], hzura::glob::event->Photons_eta[index], hzura::glob::event->Photons_phi[index], 0 );
    }

    Float_t iso;
  };

  class Electron : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Electrons_pt[index], hzura::glob::event->Electrons_eta[index], hzura::glob::event->Electrons_phi[index], 0.005 );
    }
    inline int Charge() const { return hzura::glob::event->Electrons_charge[index]; }
  };

  class Muon : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Muons_pt[index], hzura::glob::event->Muons_eta[index], hzura::glob::event->Muons_phi[index], 0.105 );
    }
    inline int Charge() const { return hzura::glob::event->Muons_charge[index]; }
  };

  class Jet : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Jets_pt[index], hzura::glob::event->Jets_eta[index], hzura::glob::event->Jets_phi[index], hzura::glob::event->Jets_m[index] );
      btag_DeepCSV_val     = -1;
      btag_DeepFlavour_val = -1;
    }

    inline int Charge() const { return hzura::glob::event->Jets_charge[index]; }

    bool btag_DeepCSV_isLoose, btag_DeepCSV_isMedium, btag_DeepCSV_isTight;
    bool btag_DeepFlavour_isLoose, btag_DeepFlavour_isMedium, btag_DeepFlavour_isTight;

    Float_t btag_DeepCSV_val, btag_DeepFlavour_val;
  };

  class Met : public Particle {
    public:
  };


};

#endif

