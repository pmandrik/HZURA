
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

    void Set(const Float_t & valx, const Float_t & valy, const Float_t & valz){
      u = valx;
      c = valy;
      d = valz;
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

  class GenParticle : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM( hzura::glob::event->GetGenParticlePt(index), hzura::glob::event->GetGenParticleEta(index), hzura::glob::event->GetGenParticlePhi(index), hzura::glob::event->GetGenParticleM(index) );
      pdg_id = hzura::glob::event->GetGenParticlePdgId(index);
      status = hzura::glob::event->GetGenParticleStatus(index);
    }

    Int_t pdg_id, status;
  };

  class Photon : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->GetPhotonPt(index), hzura::glob::event->GetPhotonEta(index), hzura::glob::event->GetPhotonPhi(index), 0 );
      mva_value = hzura::glob::event->GetPhotonMvaValue(index);
    }

    Float_t mva_value;
    Weight sf;
    Float_t iso;
  };

  class Electron : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->GetElectronPt(index), hzura::glob::event->GetElectronEta(index), hzura::glob::event->GetElectronPhi(index), 0.005 );
    }

    Weight sf;
    inline Float_t Charge() const override { return hzura::glob::event->GetElectronCharge(index); }
  };

  class Muon : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->GetMuonPt(index), hzura::glob::event->GetMuonEta(index), hzura::glob::event->GetMuonPhi(index), 0.105 );
      isLooseISO  = false;
      isMediumISO = false;
      isTightISO  = false;
    }

    bool isLooseISO, isMediumISO, isTightISO;
    Weight sf_id, sf_iso;
    inline Float_t Charge() const override { return hzura::glob::event->GetMuonCharge(index); }
  };

  class Jet : public Particle {
    public:
    void Init(const int & i){
      index = i;
      tlv.SetPtEtaPhiM(hzura::glob::event->GetJetPt(index), hzura::glob::event->GetJetEta(index), hzura::glob::event->GetJetPhi(index), hzura::glob::event->GetJetM(index) );
      btag_DeepCSV_val     = -1;
      btag_DeepFlavour_val = -1;

      hadronFlavour = hzura::glob::event->GetJetHadronFlavour( index );

      btag_eff = -1;
    }

    inline Float_t Charge() const override { return hzura::glob::event->GetJetCharge(index); }

    bool btag_DeepCSV_isLoose, btag_DeepCSV_isMedium, btag_DeepCSV_isTight;
    bool btag_DeepFlavour_isLoose, btag_DeepFlavour_isMedium, btag_DeepFlavour_isTight;

    Float_t btag_DeepCSV_val, btag_DeepFlavour_val;
    Int_t hadronFlavour;

    Weight sf_btag, sf_id;
    Double_t btag_eff;
  };

  class MET : public Particle {
    public:
    void Init(){
      pt              = hzura::glob::event->GetMETPt();
      eta             = hzura::glob::event->GetMETEta();
      phi             = hzura::glob::event->GetMETPhi();
      gen_pt          = hzura::glob::event->GetMETGenPt();
      gen_phi         = hzura::glob::event->GetMETGenPhi();
      significance    = hzura::glob::event->GetMETSignificance();

      filter_flag = false;
    }

    Bool_t          filter_flag;
    Float_t         pt, eta, phi;
    Float_t         gen_pt, gen_phi;
    Float_t         significance;
  };

  class HzuraEvent {
    public:
      HzuraEvent (){ verbose_lvl = 5; }

      ~HzuraEvent(){}

      void Print() const {
        MSG_INFO("Info about events, before/after basic selections:");
        MSG_INFO("  photons: ", hzura::glob::event->GetPhotonsN(), photon_candidates->size() );
        MSG_INFO("  electrons: ", hzura::glob::event->GetElectronsN(), electron_candidates->size() );
        MSG_INFO("  muons: ", hzura::glob::event->GetMuonsN(), muon_candidates->size() );
        MSG_INFO("  jets: ", hzura::glob::event->GetJetsN(), ljet_candidates->size() + bjet_candidates->size() );
        MSG_INFO("  genparticles: ", hzura::glob::event->GetGenParticlesN(), genparticles->size() );
      }

      std::shared_ptr< std::vector<hzura::Photon>      > photon_candidates;
      std::shared_ptr< std::vector<hzura::Electron>    > electron_candidates;
      std::shared_ptr< std::vector<hzura::Muon>        > muon_candidates;
      std::shared_ptr< std::vector<hzura::Jet>         > ljet_candidates;
      std::shared_ptr< std::vector<hzura::Jet>         > bjet_candidates;
      std::shared_ptr< std::vector<hzura::GenParticle> > genparticles;

      hzura::MET met;
      int verbose_lvl;
  };
};

#endif

