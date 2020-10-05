
#ifndef hzura_objects_helpers_h
#define hzura_objects_helpers_h 1

namespace hzura {

  // EVENTS  ================================================================================================
  struct EventWeights {
    EventWeights(){
      sf_photons.Set( 1.f );
      sf_electrons.Set( 1.f );
      sf_muons_id.Set( 1.f );
      sf_muons_iso.Set( 1.f );
      sf_ljets_id.Set( 1.f );
      sf_ljets_btag.Set( 1.f );
      sf_bjets_id.Set( 1.f );
      sf_bjets_btag.Set( 1.f );
      sf_pileup.Set( 1.f );

      sf_PUJID_tag.Set( 1.f );
      sf_PUJID_mistag.Set( 1.f );

      sf_FracRV.Set( 1.f ); 
      sf_LooseMvaSF.Set( 1.f ); 
      sf_PreselSF.Set( 1.f ); 
      sf_Trigger.Set( 1.f ); 
      sf_electronVetoSF.Set( 1.f );
      sf_PrefireSF.Set( 1.f );

      combined_weights_names      = {"sf_photons", "sf_electrons", "sf_muons_id", "sf_muons_iso", "sf_ljets_btag", "sf_bjets_btag", "sf_pileup", "sf_PUJID_tag", "sf_PUJID_mistag", "FracRV", "LooseMvaSF", "PreselSF", "Trigger",  "electronVetoSF", "PrefireSF"};
      combined_weight = 1.f;
      for(int i = 0, wn = combined_weights_names.size(); i < wn; i++){
        combined_weights_up.push_back( 1.f );
        combined_weights_down.push_back( 1.f );
      }
    }

    void CombineWeights(){
      vector<Weight*>     weights = {&sf_photons, &sf_electrons, &sf_muons_id, &sf_muons_iso, &sf_ljets_btag, &sf_bjets_btag, &sf_pileup, &sf_PUJID_tag, &sf_PUJID_mistag,
                                     &sf_FracRV, &sf_LooseMvaSF, &sf_PreselSF, &sf_Trigger, &sf_electronVetoSF, &sf_PrefireSF };
      combined_weight             = 1.f;
      for(int i = 0, wn = combined_weights_names.size(); i < wn; i++){
        combined_weight *= weights[i]->c;
        // msg( combined_weights_names[i], weights[i]->c );

        Double_t w_up = 1.f, w_down = 1.f;
        for(int j = 0; j < wn; j++){
          w_up   *= weights[j]->c;
          w_down *= weights[j]->c;
        }
        if(weights[i]->c < 0.0000000001){
          combined_weights_up[i]   = 0;
          combined_weights_down[i] = 0;
          return;
        }
        combined_weights_up[i]   = w_up   * weights[i]->u / weights[i]->c;
        combined_weights_down[i] = w_down * weights[i]->d / weights[i]->c;
      }
    }

    std::vector<Weight*> GetWeights() {
      std::vector<Weight*>     weights  = { &sf_photons, &sf_electrons, &sf_muons_id, &sf_muons_iso, &sf_ljets_btag, &sf_bjets_btag, &sf_pileup, &sf_PUJID_tag, &sf_PUJID_mistag, &sf_FracRV, &sf_LooseMvaSF, &sf_PreselSF, &sf_Trigger, &sf_electronVetoSF, &sf_PrefireSF };
      return weights;
    }

    void Print() {
      msg("Central weight", combined_weight);
      vector<Weight*> weights = GetWeights();
      for(int i = 0, wn = combined_weights_names.size(); i < wn; i++){
        Weight* w = weights.at(i);
        msg( combined_weights_names[i], " +- ", combined_weights_up[i], combined_weights_down[i], " from ", w->c, " +- ", w->u, w->d );
      }
    }

    Double_t             combined_weight;
    vector<Double_t>     combined_weights_up;
    vector<Double_t>     combined_weights_down;
    vector<std::string>  combined_weights_names;

    Weight sf_photons, sf_electrons, sf_muons_id, sf_muons_iso;
    Weight sf_ljets_id, sf_ljets_btag;
    Weight sf_bjets_id, sf_bjets_btag;
    Weight sf_pileup;
    Weight sf_PUJID_tag, sf_PUJID_mistag;

    Weight sf_FracRV, sf_LooseMvaSF, sf_PreselSF, sf_Trigger, sf_electronVetoSF, sf_PrefireSF;
  };

  EventWeights calc_event_weight(const vector<Photon> & selected_photons, const vector<Electron> & selected_electrons, const vector<Muon> & selected_muons,
                                 const vector<Jet> & selected_ljets,  const vector<Jet> & selected_bjets, const PileUpSFReader & pileup_sf_calculator){
    EventWeights ews;
    // apply photons SFs
    // ID+ISO
    for(const Photon & obj : selected_photons)
      ews.sf_photons.Mult( obj.sf );

    // apply di-photons SFs, systematics --- flashgg
    /* OLD INDEXES "Central", "FracRVWeightCentral", "FracRVWeightDown01sigma", "FracRVWeightUp01sigma", "LooseMvaSFCentral", "LooseMvaSFDown01sigma", "LooseMvaSFUp01sigma", "PreselSFCentral",       "PreselSFDown01sigma",       "PreselSFUp01sigma", "TriggerWeightCentral",  "TriggerWeightDown01sigma",  "TriggerWeightUp01sigma", "electronVetoSFCentral", "electronVetoSFDown01sigma", "electronVetoSFUp01sigma"
    const std::vector<Float_t> & diphoton_weights = hzura::glob::event->GetEventFlashggDiphotonWeights();
    ews.sf_FracRV.Set        ( diphoton_weights[2],  diphoton_weights[1], diphoton_weights[3] );
    ews.sf_LooseMvaSF.Set    ( diphoton_weights[5],  diphoton_weights[4], diphoton_weights[6] );
    ews.sf_PreselSF.Set      ( diphoton_weights[8],  diphoton_weights[7], diphoton_weights[9] );
    ews.sf_Trigger.Set       ( diphoton_weights[11], diphoton_weights[10], diphoton_weights[12] );
    ews.sf_electronVetoSF.Set( diphoton_weights[14], diphoton_weights[13], diphoton_weights[15] );
    */

    /* NEW INDEXES Central LooseMvaSFCentral LooseMvaSFDown01sigma LooseMvaSFUp01sigma
                           PreselSFCentral   PreselSFDown01sigma   PreselSFUp01sigma
                           TriggerWeightCentral TriggerWeightDown01sigma TriggerWeightUp01sigma
                           electronVetoSFCentral electronVetoSFDown01sigma electronVetoSFUp01sigma
                           prefireWeight prefireWeightDown01sigma prefireWeightUp01sigma
    */

    const std::vector<Float_t> & diphoton_weights = hzura::glob::event->GetEventFlashggDiphotonWeights();
    ews.sf_FracRV.Set        ( 1.,  1., 1. );
    ews.sf_LooseMvaSF.Set    ( diphoton_weights[2],  diphoton_weights[1], diphoton_weights[3] );
    ews.sf_PreselSF.Set      ( diphoton_weights[5],  diphoton_weights[4], diphoton_weights[6] );
    ews.sf_Trigger.Set       ( diphoton_weights[8],  diphoton_weights[7], diphoton_weights[9] );
    ews.sf_electronVetoSF.Set( diphoton_weights[11], diphoton_weights[10], diphoton_weights[12] );
    ews.sf_PrefireSF.Set     ( diphoton_weights[14], diphoton_weights[13], diphoton_weights[15] );

    if(diphoton_weights[0] < 0.0000001){
      cout << "!!!===============" << endl;
      cout << diphoton_weights.size() << " " <<  diphoton_weights[0] << endl;
      cout << diphoton_weights[2] << " " << diphoton_weights[1] << " " << diphoton_weights[3] << endl;
      cout << diphoton_weights[5] << " " << diphoton_weights[4] << " " << diphoton_weights[6] << endl;
      cout << diphoton_weights[8] << " " << diphoton_weights[7] << " " << diphoton_weights[9] << endl;
      cout << diphoton_weights[11] << " " << diphoton_weights[10] << " " << diphoton_weights[12] << endl;
      cout << diphoton_weights[14] << " " << diphoton_weights[13] << " " << diphoton_weights[15] << endl;
      cout << diphoton_weights[17] << " " << diphoton_weights[16] << " " << diphoton_weights[18] << endl;
      cout << "!!!===============" << endl;
    }

    /*
    cout << diphoton_weights.size() << " " <<  diphoton_weights[0] << endl;
    cout << diphoton_weights[2] << " " << diphoton_weights[1] << " " << diphoton_weights[3] << endl;
    cout << diphoton_weights[5] << " " << diphoton_weights[4] << " " << diphoton_weights[6] << endl;
    cout << diphoton_weights[8] << " " << diphoton_weights[7] << " " << diphoton_weights[9] << endl;
    cout << diphoton_weights[11] << " " << diphoton_weights[10] << " " << diphoton_weights[12] << endl;
    cout << diphoton_weights[14] << " " << diphoton_weights[13] << " " << diphoton_weights[15] << endl;
    */

    // apply electrons SFs
    // ID+ISO
    for(const Electron & obj : selected_electrons){
      ews.sf_electrons.Mult( obj.sf );
    }

    // apply muons SFs
    // ID+ISO
    for(const Muon & obj : selected_muons){
      ews.sf_muons_id.Mult( obj.sf_id );
      ews.sf_muons_iso.Mult( obj.sf_iso );
    }

    // apply bjets SFs for ljets
    // ID + btag
    // https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods
    Weight eff(1., 1., 1.);
    for(const Jet & obj : selected_ljets){
      ews.sf_ljets_id;
      ews.sf_ljets_btag.c *= 1. - obj.sf_btag.c * obj.btag_eff; // obj.eff_btag.c;
      ews.sf_ljets_btag.u *= 1. - obj.sf_btag.u * obj.btag_eff; // obj.eff_btag.c;
      ews.sf_ljets_btag.d *= 1. - obj.sf_btag.d * obj.btag_eff; // obj.eff_btag.c;

      eff.c *= 1. - obj.btag_eff; // obj.eff_btag.c;
      eff.u *= 1. - obj.btag_eff; // obj.eff_btag.c;
      eff.d *= 1. - obj.btag_eff; // obj.eff_btag.c;
    }
    // ews.sf_ljets_btag.Print();
    // eff.Print(); msg("===");
    ews.sf_ljets_btag.Div( eff );
    // ews.sf_ljets_btag.Print();

    // apply bjets SFs for bjets
    // ID + btag
    for(const Jet & obj : selected_bjets){
      ews.sf_bjets_id;

      ews.sf_bjets_btag.c *= obj.sf_btag.c; // obj.eff_btag.c;
      ews.sf_bjets_btag.u *= obj.sf_btag.u; // obj.eff_btag.c;
      ews.sf_bjets_btag.d *= obj.sf_btag.d; // obj.eff_btag.c;
    }

    // apply SFs from pileup NPV reweighting
    int nPVs = 30;
    if( not hzura::glob::is_data ){
      nPVs = hzura::glob::event->GetEventTrueMCNumInteractions();
      pileup_sf_calculator.CalcSFs(nPVs, ews.sf_pileup.c, ews.sf_pileup.u, ews.sf_pileup.d);
    }

    // apply SFs from PUJID --- assume, only tagged jets are going to be used in the analyses !!! ---
    for(const Jet & obj : selected_ljets){
      if( obj.PUJID_tagged ){
        ews.sf_PUJID_tag.c *= obj.sf_pujid.c;
        ews.sf_PUJID_tag.u *= obj.sf_pujid.u;
        ews.sf_PUJID_tag.d *= obj.sf_pujid.d;
      } else {  
        ews.sf_PUJID_mistag.c *= obj.sf_pujid.c;
        ews.sf_PUJID_mistag.u *= obj.sf_pujid.u;
        ews.sf_PUJID_mistag.d *= obj.sf_pujid.d;
      }
    }
    for(const Jet & obj : selected_bjets){
      if( obj.PUJID_tagged ){
        ews.sf_PUJID_tag.c *= obj.sf_pujid.c;
        ews.sf_PUJID_tag.u *= obj.sf_pujid.u;
        ews.sf_PUJID_tag.d *= obj.sf_pujid.d;
      } else {  
        ews.sf_PUJID_mistag.c *= obj.sf_pujid.c;
        ews.sf_PUJID_mistag.u *= obj.sf_pujid.u;
        ews.sf_PUJID_mistag.d *= obj.sf_pujid.d;
      }
    }

    // total and variations
    ews.CombineWeights();

    return ews;
  }

  // PileUP  ================================================================================================

  // PHOTONS ================================================================================================
  bool check_pass_flashgg_mva( hzura::Photon photon){
      // MVA Check variables 
      double  lp_mva_thresh =  0.07;
      double slp_mva_thresh = -0.03;

      double mva = photon.mva_value; 
      double eta = abs( photon.tlv.Eta() );

      // leading photon 
      // EB 
      if ( eta < 1.4442 )
        if (mva > lp_mva_thresh) return true;
      // EE 
      if (( eta > 1.566) && ( eta < 2.5))
        if (mva > slp_mva_thresh) return true;
      return false;
    }

  void calc_photon_iso(hzura::Photon * p){
    // ??? FIXME
    // https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolationRun2
    // https://arxiv.org/pdf/1502.02702.pdf
    // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2

    const Float_t & pt                 = p->tlv.Pt();
    const Float_t & eta                = p->tlv.Eta();

    const Float_t & sumChargedHadronPt = hzura::glob::event->GetPhotonSumChargedHadronPt( p->index ); 
    const Float_t & sumNeutralHadronEt = hzura::glob::event->GetPhotonSumNeutralHadronEt( p->index ); 
    const Float_t & sumPhotonEt        = hzura::glob::event->GetPhotonSumPhotonEt( p->index ); 

    if( hzura::glob::year_era == "2016" ){
    } else 
    if( hzura::glob::year_era == "2017" ){
    } else
    if( hzura::glob::year_era == "2018" ){
    }
  };

  void apply_energy_correction( Photon & p, const string & type ){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaMiniAODV2#Applying_the_Energy_Scale_and_sm
      const int & index = p.index;

      Float_t E_corrected = 1;
      if( type == "ecalEnergyPreCorr" ) E_corrected = hzura::glob::event->GetPhotonEcalEnergyPreCorr( index ); //  ecalEnergy before scale & smearing corrections
      else if( type == "ecalEnergyPostCorr" ) E_corrected = hzura::glob::event->GetPhotonEcalEnergyPostCorr( index ); //  ecalEnergy of electron after scale & smearing corrections
      else if( type == "energyScaleUp" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleUp( index ); //  energy with the ecal energy scale shifted 1 sigma up (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleDown" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleDown( index ); //  energy with the ecal energy scale shifted 1 sigma down (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleStatUp" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleStatUp( index ); //  energy with the ecal energy scale shifted 1 sigma(stat) up
      else if( type == "energyScaleStatDown" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleStatDown( index ); //  energy with the ecal energy scale shifted 1 sigma(stat) down
      else if( type == "energyScaleSystUp" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleSystUp( index ); //  energy with the ecal energy scale shifted 1 sigma(syst) up
      else if( type == "energyScaleSystDown" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleSystDown( index ); //  energy with the ecal energy scale shifted 1 sigma(syst) down
      else if( type == "energyScaleGainUp" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleGainUp( index ); //  energy with the ecal energy scale shifted 1 sigma(gain) up
      else if( type == "energyScaleGainDown" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleGainDown( index ); //  energy with the ecal energy scale shifted 1 sigma(gain) down
      else if( type == "energyScaleEtUp" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleEtUp( index ); //  2016 legacy only: adhoc error derived from closure vs Et
      else if( type == "energyScaleEtDown" ) E_corrected = hzura::glob::event->GetPhotonEnergyScaleEtDown( index ); //  2016 legacy only: adhoc error dervied from closure vs Et
      else if( type == "energySigmaUp" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma up
      else if( type == "energySigmaDown" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma down
      else if( type == "energySigmaPhiUp" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaPhiUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma(phi) up
      else if( type == "energySigmaPhiDown" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaPhiDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma(phi) down
      else if( type == "energySigmaRhoUp" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaRhoUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma(rho) up
      else if( type == "energySigmaRhoDown" ) E_corrected = hzura::glob::event->GetPhotonEnergySigmaRhoDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma(rho) down
      else msg_err( "hzura::apply_energy_correction(): can't find energy correction name\"", type, "\"" );
      
      p.tlv = p.tlv * (E_corrected / p.tlv.E());
  }

  template<class T> // hzura::Electron, hzura::Photon
  void set_egamma_sfs(T & egamma, SFCalculator & sf_calculator, const int & sf_type){
    const Double_t & eta = egamma.tlv.Eta();
    const Double_t & pt  = egamma.tlv.Pt();

    vector<Float_t *> valse = { & egamma.sf.c, & egamma.sf.u, & egamma.sf.d };
    vector<int> unc_vals = {0, 1, -1};
    if( sf_type == hzura::id_names::loose )
      for(int i = 0; i < valse.size(); i++)
        *( valse[i] ) = sf_calculator.GetSF_loose(eta, pt, unc_vals[i]);
    else if( sf_type == hzura::id_names::medium )
      for(int i = 0; i < valse.size(); i++)
        *( valse[i] ) = sf_calculator.GetSF_medium(eta, pt, unc_vals[i]);
    else if( sf_type == hzura::id_names::tight )
      for(int i = 0; i < valse.size(); i++) 
        *( valse[i] ) = sf_calculator.GetSF_tight(eta, pt, unc_vals[i]);
  }

  // ELECTRONS ================================================================================================
  void apply_energy_correction( Electron & p, const string & type ){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaMiniAODV2#Applying_the_Energy_Scale_and_sm
      const int & index = p.index;

      Float_t E_corrected = 0;
      if( type == "ecalTrkEnergyPreCorr" ) E_corrected = hzura::glob::event->GetElectronEcalTrkEnergyPreCorr( index ); //  ECAL-Trk combined electron energy before scale & smearing corrections
      else if( type == "ecalTrkEnergyPostCorr" ) E_corrected = hzura::glob::event->GetElectronEcalTrkEnergyPostCorr( index ); //  ECAL-Trk combined electron energy after scale & smearing corrections
      else if( type == "energyScaleUp" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleUp( index ); //  energy with the ecal energy scale shifted 1 sigma up (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleDown" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleDown( index ); //  energy with the ecal energy scale shifted 1 sigma down (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleStatUp" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleStatUp( index ); //  energy with the ecal energy scale shifted 1 sigma(stat) up
      else if( type == "energyScaleStatDown" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleStatDown( index ); //  energy with the ecal energy scale shifted 1 sigma(stat) down
      else if( type == "energyScaleSystUp" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleSystUp( index ); //  energy with the ecal energy scale shifted 1 sigma(syst) up
      else if( type == "energyScaleSystDown" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleSystDown( index ); //  energy with the ecal energy scale shifted 1 sigma(syst) down
      else if( type == "energyScaleGainUp" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleGainUp( index ); //  energy with the ecal energy scale shifted 1 sigma(gain) up
      else if( type == "energyScaleGainDown" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleGainDown( index ); //  energy with the ecal energy scale shifted 1 sigma(gain) down
      else if( type == "energyScaleEtUp" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleEtUp( index ); //  2016 legacy only: adhoc error derived from closure vs Et
      else if( type == "energyScaleEtDown" ) E_corrected = hzura::glob::event->GetElectronEnergyScaleEtDown( index ); //  2016 legacy only: adhoc error dervied from closure vs Et
      else if( type == "energySigmaUp" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma up
      else if( type == "energySigmaDown" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma down
      else if( type == "energySigmaPhiUp" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaPhiUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma(phi) up
      else if( type == "energySigmaPhiDown" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaPhiDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma(phi) down
      else if( type == "energySigmaRhoUp" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaRhoUp( index ); //  energy with the ecal energy smearing value shifted 1 sigma(rho) up
      else if( type == "energySigmaRhoDown" ) E_corrected = hzura::glob::event->GetElectronEnergySigmaRhoDown( index ); //  energy with the ecal energy smearing value shifted 1 sigma(rho) down
      
      p.tlv = p.tlv * (E_corrected / p.tlv.E());
  }

  // MUONS ================================================================================================
  void calc_muon_iso(hzura::Muon & muon){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Particle_Flow_isolation
    const int & index = muon.index;
    Float_t iso = hzura::glob::event->GetMuonRelIsoPF( index );

    if( iso < 0.25 ) muon.isLooseISO  = true; else return;
    if( iso < 0.20 ) muon.isMediumISO = true; else return;
    if( iso < 0.15 ) muon.isTightISO  = true; else return;
  }

  void set_muon_sfs(hzura::Muon & muon, SFCalculator & muon_sf_calculator, const int & sf_type){
    const Double_t & eta = muon.tlv.Eta();
    const Double_t & pt  = muon.tlv.Pt();

    if( sf_type == id_names::loose ){
      muon.sf_id.c   = muon_sf_calculator.GetSF_id_loose(  eta, pt,  0 );
      muon.sf_id.u   = muon_sf_calculator.GetSF_id_loose(  eta, pt,  1 );
      muon.sf_id.d   = muon_sf_calculator.GetSF_id_loose(  eta, pt, -1 );
      muon.sf_iso.c  = muon_sf_calculator.GetSF_iso_loose(  eta, pt,  0 );
      muon.sf_iso.u  = muon_sf_calculator.GetSF_iso_loose(  eta, pt,  1 );
      muon.sf_iso.d  = muon_sf_calculator.GetSF_iso_loose(  eta, pt, -1 );

      if( muon.sf_id.c < 0.9)
        msg("Muon SF = ", muon.sf_id.c, eta, pt);
    } else 
    if( sf_type == id_names::tight ){
      muon.sf_id.c   = muon_sf_calculator.GetSF_id_tight(  eta, pt,  0 );
      muon.sf_id.u   = muon_sf_calculator.GetSF_id_tight(  eta, pt,  1 );
      muon.sf_id.d   = muon_sf_calculator.GetSF_id_tight(  eta, pt, -1 );
      muon.sf_iso.c  = muon_sf_calculator.GetSF_iso_tight(  eta, pt,  0 );
      muon.sf_iso.u  = muon_sf_calculator.GetSF_iso_tight(  eta, pt,  1 );
      muon.sf_iso.d  = muon_sf_calculator.GetSF_iso_tight(  eta, pt, -1 );
    }
  }

  // JETS ================================================================================================
  // PUJID ==--
  void calc_pujid_variables(hzura::Jet & j, const PUJIDReader & pujid_reader){ // FIXME 2016 2017 2018
     j.PUJID = hzura::glob::event->GetJetPUJID( j.index );
     j.PUJID_tagged = hzura::glob::event->GetJetGetJetPt( j.index ) > 0.;

    const Double_t & eta = j.tlv.Eta();
    const Double_t & pt  = j.tlv.Pt();
    // https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Working_points
    if(hzura::glob::year == 2016){
    }
    else if(hzura::glob::year == 2017){
      j.PUJID_isTight  = pujid_reader.GetID( j.PUJID, pt, eta, hzura::id_names::tight  );
      j.PUJID_isMedium = pujid_reader.GetID( j.PUJID, pt, eta, hzura::id_names::medium );
      j.PUJID_isLoose  = pujid_reader.GetID( j.PUJID, pt, eta, hzura::id_names::loose  );
    }
    else if(hzura::glob::year == 2018){
    }
  }

  void set_pujid_sfs(hzura::Jet & j, const PUJIDReader & pujid_reader, const int & wp){ // FIXME 2016 2017 2018
    const Double_t & eta = j.tlv.Eta();
    const Double_t & pt  = j.tlv.Pt();
    if( pt > 50 ){
      j.sf_pujid.Set(1.f);
      return;
    }
    if( j.PUJID_tagged ){
      if( wp == id_names::loose ){
        j.sf_pujid.d = pujid_reader.GetSF_eff_loose(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_eff_loose(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_eff_loose(eta, pt,  1);
      }
      if( wp == id_names::medium ){
        j.sf_pujid.d = pujid_reader.GetSF_eff_medium(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_eff_medium(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_eff_medium(eta, pt,  1);
      }
      if( wp == id_names::tight ){
        j.sf_pujid.d = pujid_reader.GetSF_eff_tight(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_eff_tight(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_eff_tight(eta, pt,  1);
      }
    } else {
      if( wp == id_names::loose ){
        j.sf_pujid.d = pujid_reader.GetSF_mistag_loose(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_mistag_loose(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_mistag_loose(eta, pt,  1);
      }
      if( wp == id_names::medium ){
        j.sf_pujid.d = pujid_reader.GetSF_mistag_medium(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_mistag_medium(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_mistag_medium(eta, pt,  1);
      }
      if( wp == id_names::tight ){
        j.sf_pujid.d = pujid_reader.GetSF_mistag_tight(eta, pt, -1);
        j.sf_pujid.c = pujid_reader.GetSF_mistag_tight(eta, pt,  0);
        j.sf_pujid.u = pujid_reader.GetSF_mistag_tight(eta, pt,  1);
      }
    }
  }

  // btags ==--
  void calc_btag_variables(hzura::Jet & j){ // FIXME 2016 2017 2018
    const int & i = j.index;
    // b-tagging
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2017#B_tagging
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    // DeepCSV
    // jet.pfDeepCSVJetTags_probb    = j.bDiscriminator("pfDeepCSVJetTags:probb");
    // jet.pfDeepCSVJetTags_probbb   = j.bDiscriminator("pfDeepCSVJetTags:probbb");
    // jet.pfDeepCSVJetTags_probc    = j.bDiscriminator("pfDeepCSVJetTags:probc");
    // jet.pfDeepCSVJetTags_probudsg = j.bDiscriminator("pfDeepCSVJetTags:probudsg");

    // pfDeepCSVJetTags:probb + pfDeepCSVJetTags:probbb
    j.btag_DeepCSV_val = hzura::glob::event->GetJetPfDeepCSVJetTagsProbb(i) + hzura::glob::event->GetJetPfDeepCSVJetTagsProbbb(i);

    if(hzura::glob::year == 2016){
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      j.btag_DeepCSV_isLoose  = (j.btag_DeepCSV_val > 0.2219) ? true : false;
      j.btag_DeepCSV_isMedium = (j.btag_DeepCSV_val > 0.6324) ? true : false;
      j.btag_DeepCSV_isTight  = (j.btag_DeepCSV_val > 0.8958) ? true : false;
    }
    else if(hzura::glob::year == 2017){
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
      j.btag_DeepCSV_isLoose  = (j.btag_DeepCSV_val > 0.1522) ? true : false;
      j.btag_DeepCSV_isMedium = (j.btag_DeepCSV_val > 0.4941) ? true : false;
      j.btag_DeepCSV_isTight  = (j.btag_DeepCSV_val > 0.8001) ? true : false;
    }
    else if(hzura::glob::year == 2018){
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      j.btag_DeepCSV_isLoose  = (j.btag_DeepCSV_val > 0.1241) ? true : false;
      j.btag_DeepCSV_isMedium = (j.btag_DeepCSV_val > 0.4184) ? true : false;
      j.btag_DeepCSV_isTight  = (j.btag_DeepCSV_val > 0.7527) ? true : false;
    }

    // DeepJet
    // jet.pfDeepFlavourJetTags_probb    = j.bDiscriminator("pfDeepFlavourJetTags:probb");
    // jet.pfDeepFlavourJetTags_probbb   = j.bDiscriminator("pfDeepFlavourJetTags:probbb");
    // jet.pfDeepFlavourJetTags_problepb = j.bDiscriminator("pfDeepFlavourJetTags:problepb");
    // jet.pfDeepFlavourJetTags_probc    = j.bDiscriminator("pfDeepFlavourJetTags:probc");
    // jet.pfDeepFlavourJetTags_probuds  = j.bDiscriminator("pfDeepFlavourJetTags:probuds");
    // jet.pfDeepFlavourJetTags_probg    = j.bDiscriminator("pfDeepFlavourJetTags:probg");

    // pfDeepFlavourJetTags:probb + pfDeepFlavourJetTags:probbb + pfDeepFlavourJetTags:problepb
    j.btag_DeepFlavour_val = hzura::glob::event->GetJetPfDeepFlavourJetTagsProbb(i) + hzura::glob::event->GetJetPfDeepFlavourJetTagsProbbb(i) + hzura::glob::event->GetJetPfDeepFlavourJetTagsProblepb(i);
    if( j.btag_DeepFlavour_val < 0. ) {
      msg("Negative Deep Flavour Jet Tag value = ", j.btag_DeepFlavour_val, hzura::glob::event->GetJetPfDeepFlavourJetTagsProbb(i), hzura::glob::event->GetJetPfDeepFlavourJetTagsProbbb(i), hzura::glob::event->GetJetPfDeepFlavourJetTagsProblepb(i) );
      j.btag_DeepFlavour_val = -0.01;
    }
    
    if(hzura::glob::year == 2016){
      j.btag_DeepFlavour_isLoose  = (j.btag_DeepFlavour_val > 0.0494) ? true : false;
      j.btag_DeepFlavour_isMedium = (j.btag_DeepFlavour_val > 0.2770) ? true : false;
      j.btag_DeepFlavour_isTight  = (j.btag_DeepFlavour_val > 0.7264) ? true : false;
    }
    else if(hzura::glob::year == 2017){
      j.btag_DeepFlavour_isLoose  = (j.btag_DeepFlavour_val > 0.0521) ? true : false;
      j.btag_DeepFlavour_isMedium = (j.btag_DeepFlavour_val > 0.3033) ? true : false;
      j.btag_DeepFlavour_isTight  = (j.btag_DeepFlavour_val > 0.7489) ? true : false;
    }
    else if(hzura::glob::year == 2018){
      // TODO
    }
  };

  void set_jet_btag_sfs(hzura::Jet & j, const BTagSFReader & btag_sf_calculator_b, const BTagSFReader & btag_sf_calculator_c, const BTagSFReader & btag_sf_calculator_l){
    // https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Standalone
    const Float_t & pt  = j.tlv.Pt();
    const Float_t & eta = j.tlv.Eta();
    if( j.hadronFlavour == 5 ){
      j.sf_btag.c = btag_sf_calculator_b.GetSF("central", eta, pt); 
      j.sf_btag.u = btag_sf_calculator_b.GetSF("up", eta, pt); 
      j.sf_btag.d = btag_sf_calculator_b.GetSF("down", eta, pt); 
    } else if ( j.hadronFlavour == 4 ){
      j.sf_btag.c = btag_sf_calculator_c.GetSF("central", eta, pt); 
      j.sf_btag.u = btag_sf_calculator_c.GetSF("up", eta, pt); 
      j.sf_btag.d = btag_sf_calculator_c.GetSF("down", eta, pt); 
    } else if ( j.hadronFlavour > -1 ){
      j.sf_btag.c = btag_sf_calculator_l.GetSF("central", eta, pt); 
      j.sf_btag.u = btag_sf_calculator_l.GetSF("up", eta, pt); 
      j.sf_btag.d = btag_sf_calculator_l.GetSF("down", eta, pt); 
    }
  };

  // Evaluate JER smearing after nominal JEC is applied but before systematic variaion in JEC
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution#Smearing_procedures
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution?rev=54#Smearing_procedures
  void apply_jer_smearing(hzura::Jet & j, const string & type = ""){
    const int & i = j.index;
    Float_t cjer = 1.f;
    Float_t sjet = hzura::glob::event->GetJetSf(i);
    // msg(sjet, hzura::glob::event->Jets_sf_u[i], hzura::glob::event->Jets_sf_d[i]);
    if( type == "up")   sjet = hzura::glob::event->GetJetSfU(i);
    if( type == "down") sjet = hzura::glob::event->GetJetSfD(i);

    Float_t getJet_pt = hzura::glob::event->GetJetGetJetPt(i);
    if(getJet_pt > 0)
      cjer = 1.f + sjet * ( j.tlv.Pt() - getJet_pt ) / j.tlv.Pt();
    else 
      cjer = 1.f + hzura::glob::randGen.Gaus(0., hzura::glob::event->GetJetResolution(i) ) * std::sqrt(std::max(std::pow(sjet, 2) - 1., 0.));;

    if( cjer < 0 ){
      //msg( "hzura::apply_jer_smearing(): negative cJER factor", cjer, getJet_pt, sjet * ( j.tlv.Pt() - getJet_pt ) / j.tlv.Pt(), hzura::glob::event->GetJetResolution(i) );  
      //if(getJet_pt > 0) msg( "getJet_pt, sjet * ( j.tlv.Pt() - getJet_pt ) / j.tlv.Pt() = ", getJet_pt, sjet * ( j.tlv.Pt() - getJet_pt ) / j.tlv.Pt() );
      //else              msg( "Jets_resolution[i], SF, std::sqrt(std::max(std::pow(SF, 2) - 1., 0.)) = ", hzura::glob::event->GetJetResolution(i), sjet, std::sqrt(std::max(std::pow(sjet, 2) - 1., 0.)) );
      cjer = 0;
    }
    j.tlv *= cjer;
  };

  // MET ==========================================================================================================
  void set_met_filter_flag(hzura::MET & met_candidate){
    met_candidate.filter_flag = true;
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagGoodVertices();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagGlobalSuperTightHalo2016Filter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagHBHENoiseFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagHBHENoiseIsoFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagEcalDeadCellTriggerPrimitiveFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagBadPFMuonFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagBadChargedCandidateFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagEeBadScFilter();
    met_candidate.filter_flag = met_candidate.filter_flag && hzura::glob::event->GetMETFlagEcalBadCalibReducedMINIAODFilter();
  }

  // calculate MET variation due to systematic uncertanties in JEC, JER and Unclastered energy 
  // { Var::JetEnUp, Var::JetEnDown, Var::JetResUp, Var::JetResDown, Var::UnclusteredEnUp, Var::UnclusteredEnDown }
  void apply_met_systematic_variation(hzura::MET & met_candidate, const string & type = "" ){
      int index = -1;
           if( type == "JetEnUp" )           index = 0;
      else if( type == "JetEnDown" )         index = 1;
      else if( type == "JetResUp" )          index = 2;
      else if( type == "JetResDown" )        index = 3;
      else if( type == "UnclusteredEnUp" )   index = 4;
      else if( type == "UnclusteredEnDown" ) index = 5;
      if(index < 0) return;

      met_candidate.pt  = hzura::glob::event->GetMETPtUncV() [ index ];
      met_candidate.phi = hzura::glob::event->GetMETPhiUncV()[ index ];
  }

};











#endif

