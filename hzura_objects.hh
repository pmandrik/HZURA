
#ifndef hzura_objects_h
#define hzura_objects_h

#include "hzura_cfg.hh"

namespace hzura {

  struct Weight {
    Weight(){
      c = 0.f;
      u = 0.f;
      d = 0.f;
    }

    void Set(const Float_t & val){
      c = val;
      u = val;
      d = val;
    }

    void Mult(const Weight & item){
      c *= item.c;
      u *= item.u;
      d *= item.d;
    }

    Float_t c, u, d;
  };

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

    Weight sf;
    Float_t iso;
  };

  class Electron : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Electrons_pt[index], hzura::glob::event->Electrons_eta[index], hzura::glob::event->Electrons_phi[index], 0.005 );
    }

    Weight sf;
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
    }

    bool isLooseISO, isMediumISO, isTightISO;
    Weight sf_id, sf_iso;
    inline Float_t Charge() const override { return hzura::glob::event->Muons_charge[index]; }
  };

  class Jet : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->Jets_pt[index], hzura::glob::event->Jets_eta[index], hzura::glob::event->Jets_phi[index], hzura::glob::event->Jets_m[index] );
      btag_DeepCSV_val     = -1;
      btag_DeepFlavour_val = -1;

      eff_btag.Set( 0.5)  ; // FIXME
    }

    inline Float_t Charge() const override { return hzura::glob::event->Jets_charge[index]; }

    bool btag_DeepCSV_isLoose, btag_DeepCSV_isMedium, btag_DeepCSV_isTight;
    bool btag_DeepFlavour_isLoose, btag_DeepFlavour_isMedium, btag_DeepFlavour_isTight;

    Float_t btag_DeepCSV_val, btag_DeepFlavour_val;

    Weight sf_btag, sf_id;
    Weight eff_btag;
  };

  class MET : public Particle {
    public:
    void Init(){
      pt              = hzura::glob::event->pt;
      eta             = hzura::glob::event->eta;
      phi             = hzura::glob::event->phi;
      gen_pt          = hzura::glob::event->gen_pt;
      gen_phi         = hzura::glob::event->gen_phi;
      significance    = hzura::glob::event->significance;

      filter_flag = false;
    }

    Bool_t          filter_flag;
    Float_t         pt, eta, phi;
    Float_t         gen_pt, gen_phi;
    Float_t         significance;
  };

  class HzuraEvent {
    public:
      HzuraEvent (){
        photon_candidates = nullptr;
        electron_candidates = nullptr;
        muon_candidates = nullptr;
        ljet_candidates = nullptr;
        bjet_candidates = nullptr;
      }

      ~HzuraEvent(){
        delete photon_candidates ;
        delete electron_candidates ;
        delete muon_candidates ;
        delete ljet_candidates ;
        delete bjet_candidates ;
      }

      std::vector<hzura::Photon>   * photon_candidates;
      std::vector<hzura::Electron> * electron_candidates;
      std::vector<hzura::Muon>     * muon_candidates;
      std::vector<hzura::Jet>      * ljet_candidates;
      std::vector<hzura::Jet>      * bjet_candidates;

      hzura::MET met;
  };
};

#endif

