
#ifndef hzura_objects_h
#define hzura_objects_h

#include "hzura_cfg.hh"

namespace hzura {

  class Particle {
    public:
    Particle(){
      index = -1;
    }
    
    virtual inline Float_t Charge() const { return 0; };

    int index;
    TLorentzVector tlv;

    std::vector<Particle*> mothers;
  };

  class Photon : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Photons_pt[index], hzura::glob::event->Photons_eta[index], hzura::glob::event->Photons_phi[index], 0 );
    }

    Float_t iso;
  };

  class Electron : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Electrons_pt[index], hzura::glob::event->Electrons_eta[index], hzura::glob::event->Electrons_phi[index], 0.005 );

      sf = 0;
      sf_up = 0;
      sf_down = 0;
    }

    Float_t sf, sf_up, sf_down;
    inline Float_t Charge() const override { return hzura::glob::event->Electrons_charge[index]; }
  };

  class Muon : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Muons_pt[index], hzura::glob::event->Muons_eta[index], hzura::glob::event->Muons_phi[index], 0.105 );
      isLooseISO  = false;
      isMediumISO = false;
      isTightISO  = false;

      sf_id = 0;
      sf_id_up = 0; 
      sf_id_down = 0;
      sf_iso = 0;
      sf_iso_up = 0;
      sf_iso_down = 0;
    }

    bool isLooseISO, isMediumISO, isTightISO;
    Float_t sf_id,   sf_id_up, sf_id_down;
    Float_t sf_iso, sf_iso_up, sf_iso_down;
    inline Float_t Charge() const override { return hzura::glob::event->Muons_charge[index]; }
  };

  class Jet : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Jets_pt[index], hzura::glob::event->Jets_eta[index], hzura::glob::event->Jets_phi[index], hzura::glob::event->Jets_m[index] );
      btag_DeepCSV_val     = -1;
      btag_DeepFlavour_val = -1;
    }

    inline Float_t Charge() const override { return hzura::glob::event->Jets_charge[index]; }

    bool btag_DeepCSV_isLoose, btag_DeepCSV_isMedium, btag_DeepCSV_isTight;
    bool btag_DeepFlavour_isLoose, btag_DeepFlavour_isMedium, btag_DeepFlavour_isTight;

    Float_t btag_DeepCSV_val, btag_DeepFlavour_val;
  };

  class Met : public Particle {
    public:
  };


};

#endif

