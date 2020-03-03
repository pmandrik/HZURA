
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
    }

    Weight sf_photons, sf_electrons, sf_muons_id, sf_muons_iso;
    Weight sf_ljets_id, sf_ljets_btag;
    Weight sf_bjets_id, sf_bjets_btag;
  };

  EventWeights calc_event_weight(const vector<Photon> & selected_photons, const vector<Electron> & selected_electrons, const vector<Muon> & selected_muons,
                            const vector<Jet> & selected_ljets,  const vector<Jet> selected_bjets){
    EventWeights ews;
    // apply photons SFs
    // ID+ISO
    for(const Photon & obj : selected_photons)
      ews.sf_photons.Mult( obj.sf );

    // apply electrons SFs
    // ID+ISO
    for(const Electron & obj : selected_electrons)
      ews.sf_electrons.Mult( obj.sf );

    // apply muons SFs
    // ID+ISO
    for(const Muon & obj : selected_muons){
      ews.sf_muons_id.Mult( obj.sf_id );
      ews.sf_muons_iso.Mult( obj.sf_iso );
    }

    // apply bjets SFs
    // ID + btag
    for(const Jet & obj : selected_ljets){
      ews.sf_ljets_id;

      ews.sf_ljets_btag.c *= 1. - obj.sf_btag.c * obj.eff_btag.c;
      ews.sf_ljets_btag.u *= 1. - obj.sf_btag.u * obj.eff_btag.u;
      ews.sf_ljets_btag.d *= 1. - obj.sf_btag.d * obj.eff_btag.d;
    }

    // apply bjets SFs
    // ID + btag
    for(const Jet & obj : selected_bjets){
      ews.sf_bjets_id;

      ews.sf_bjets_btag.c *= obj.sf_btag.c * obj.eff_btag.c;
      ews.sf_bjets_btag.u *= obj.sf_btag.u * obj.eff_btag.u;
      ews.sf_bjets_btag.d *= obj.sf_btag.d * obj.eff_btag.d;
    }

    // total
    // Float_t weight = weight_photons * weight_electrons * weight_muons_id * weight_muons_iso;
    return ews;
  }

  // PHOTONS ================================================================================================
  void calc_photon_iso(hzura::Photon * p){
    // ??? FIXME
    // https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolationRun2
    // https://arxiv.org/pdf/1502.02702.pdf
    // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2

    const Float_t & pt                 = hzura::glob::event->Photons_pt[ p->index ];
    const Float_t & eta                = hzura::glob::event->Photons_eta[ p->index ];

    const Float_t & sumChargedHadronPt = hzura::glob::event->Photons_sumChargedHadronPt[ p->index ]; 
    const Float_t & sumNeutralHadronEt = hzura::glob::event->Photons_sumNeutralHadronEt[ p->index ]; 
    const Float_t & sumPhotonEt        = hzura::glob::event->Photons_sumPhotonEt[ p->index ]; 

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

      Float_t E_corrected = 0;
      if( type == "ecalEnergyPreCorr" ) E_corrected = hzura::glob::event->Photons_ecalEnergyPreCorr[index]; //  ecalEnergy before scale & smearing corrections
      else if( type == "ecalEnergyPostCorr" ) E_corrected = hzura::glob::event->Photons_ecalEnergyPostCorr[index]; //  ecalEnergy of electron after scale & smearing corrections
      else if( type == "energyScaleUp" ) E_corrected = hzura::glob::event->Photons_energyScaleUp[index]; //  energy with the ecal energy scale shifted 1 sigma up (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleDown" ) E_corrected = hzura::glob::event->Photons_energyScaleDown[index]; //  energy with the ecal energy scale shifted 1 sigma down (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleStatUp" ) E_corrected = hzura::glob::event->Photons_energyScaleStatUp[index]; //  energy with the ecal energy scale shifted 1 sigma(stat) up
      else if( type == "energyScaleStatDown" ) E_corrected = hzura::glob::event->Photons_energyScaleStatDown[index]; //  energy with the ecal energy scale shifted 1 sigma(stat) down
      else if( type == "energyScaleSystUp" ) E_corrected = hzura::glob::event->Photons_energyScaleSystUp[index]; //  energy with the ecal energy scale shifted 1 sigma(syst) up
      else if( type == "energyScaleSystDown" ) E_corrected = hzura::glob::event->Photons_energyScaleSystDown[index]; //  energy with the ecal energy scale shifted 1 sigma(syst) down
      else if( type == "energyScaleGainUp" ) E_corrected = hzura::glob::event->Photons_energyScaleGainUp[index]; //  energy with the ecal energy scale shifted 1 sigma(gain) up
      else if( type == "energyScaleGainDown" ) E_corrected = hzura::glob::event->Photons_energyScaleGainDown[index]; //  energy with the ecal energy scale shifted 1 sigma(gain) down
      else if( type == "energyScaleEtUp" ) E_corrected = hzura::glob::event->Photons_energyScaleEtUp[index]; //  2016 legacy only: adhoc error derived from closure vs Et
      else if( type == "energyScaleEtDown" ) E_corrected = hzura::glob::event->Photons_energyScaleEtDown[index]; //  2016 legacy only: adhoc error dervied from closure vs Et
      else if( type == "energySigmaUp" ) E_corrected = hzura::glob::event->Photons_energySigmaUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma up
      else if( type == "energySigmaDown" ) E_corrected = hzura::glob::event->Photons_energySigmaDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma down
      else if( type == "energySigmaPhiUp" ) E_corrected = hzura::glob::event->Photons_energySigmaPhiUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma(phi) up
      else if( type == "energySigmaPhiDown" ) E_corrected = hzura::glob::event->Photons_energySigmaPhiDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma(phi) down
      else if( type == "energySigmaRhoUp" ) E_corrected = hzura::glob::event->Photons_energySigmaRhoUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma(rho) up
      else if( type == "energySigmaRhoDown" ) E_corrected = hzura::glob::event->Photons_energySigmaRhoDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma(rho) down
      
      p.tlv = p.tlv * (E_corrected / p.tlv.E());
  }

  template<class T> // hzura::Electron, hzura::Photon
  void set_egamma_sfs(T & egamma, SFCalculator & sf_calculator, const string & sf_type){
    const Double_t & eta = egamma.tlv.Eta();
    const Double_t & pt  = egamma.tlv.Pt();

    vector<Float_t *> valse = { & egamma.sf.c, & egamma.sf.u, & egamma.sf.d };
    vector<int> unc_vals = {0, 1, -1};
    if( sf_type == "loose" )
      for(int i = 0; i < valse.size(); i++)
        *( valse[i] ) = sf_calculator.GetSF_loose(eta, pt, unc_vals[i]);
    else if( sf_type == "medium" )
      for(int i = 0; i < valse.size(); i++)
        *( valse[i] ) = sf_calculator.GetSF_medium(eta, pt, unc_vals[i]);
    else if( sf_type == "tight" )
      for(int i = 0; i < valse.size(); i++) 
        *( valse[i] ) = sf_calculator.GetSF_tight(eta, pt, unc_vals[i]);
  }

  // ELECTRONS ================================================================================================
  void apply_energy_correction( Electron & p, const string & type ){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaMiniAODV2#Applying_the_Energy_Scale_and_sm
      const int & index = p.index;

      Float_t E_corrected = 0;
      if( type == "ecalTrkEnergyPreCorr" ) E_corrected = hzura::glob::event->Electrons_ecalTrkEnergyPreCorr[index]; //  ECAL-Trk combined electron energy before scale & smearing corrections
      else if( type == "ecalTrkEnergyPostCorr" ) E_corrected = hzura::glob::event->Electrons_ecalTrkEnergyPostCorr[index]; //  ECAL-Trk combined electron energy after scale & smearing corrections
      else if( type == "energyScaleUp" ) E_corrected = hzura::glob::event->Electrons_energyScaleUp[index]; //  energy with the ecal energy scale shifted 1 sigma up (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleDown" ) E_corrected = hzura::glob::event->Electrons_energyScaleDown[index]; //  energy with the ecal energy scale shifted 1 sigma down (adding gain/stat/syst in quadrature)
      else if( type == "energyScaleStatUp" ) E_corrected = hzura::glob::event->Electrons_energyScaleStatUp[index]; //  energy with the ecal energy scale shifted 1 sigma(stat) up
      else if( type == "energyScaleStatDown" ) E_corrected = hzura::glob::event->Electrons_energyScaleStatDown[index]; //  energy with the ecal energy scale shifted 1 sigma(stat) down
      else if( type == "energyScaleSystUp" ) E_corrected = hzura::glob::event->Electrons_energyScaleSystUp[index]; //  energy with the ecal energy scale shifted 1 sigma(syst) up
      else if( type == "energyScaleSystDown" ) E_corrected = hzura::glob::event->Electrons_energyScaleSystDown[index]; //  energy with the ecal energy scale shifted 1 sigma(syst) down
      else if( type == "energyScaleGainUp" ) E_corrected = hzura::glob::event->Electrons_energyScaleGainUp[index]; //  energy with the ecal energy scale shifted 1 sigma(gain) up
      else if( type == "energyScaleGainDown" ) E_corrected = hzura::glob::event->Electrons_energyScaleGainDown[index]; //  energy with the ecal energy scale shifted 1 sigma(gain) down
      else if( type == "energyScaleEtUp" ) E_corrected = hzura::glob::event->Electrons_energyScaleEtUp[index]; //  2016 legacy only: adhoc error derived from closure vs Et
      else if( type == "energyScaleEtDown" ) E_corrected = hzura::glob::event->Electrons_energyScaleEtDown[index]; //  2016 legacy only: adhoc error dervied from closure vs Et
      else if( type == "energySigmaUp" ) E_corrected = hzura::glob::event->Electrons_energySigmaUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma up
      else if( type == "energySigmaDown" ) E_corrected = hzura::glob::event->Electrons_energySigmaDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma down
      else if( type == "energySigmaPhiUp" ) E_corrected = hzura::glob::event->Electrons_energySigmaPhiUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma(phi) up
      else if( type == "energySigmaPhiDown" ) E_corrected = hzura::glob::event->Electrons_energySigmaPhiDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma(phi) down
      else if( type == "energySigmaRhoUp" ) E_corrected = hzura::glob::event->Electrons_energySigmaRhoUp[index]; //  energy with the ecal energy smearing value shifted 1 sigma(rho) up
      else if( type == "energySigmaRhoDown" ) E_corrected = hzura::glob::event->Electrons_energySigmaRhoDown[index]; //  energy with the ecal energy smearing value shifted 1 sigma(rho) down
      
      p.tlv = p.tlv * (E_corrected / p.tlv.E());
  }

  // MUONS ================================================================================================
  void calc_muon_iso(hzura::Muon & muon){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Particle_Flow_isolation
    const int & index = muon.index;
    Float_t iso = hzura::glob::event->Muons_relIsoPF[ index ];

    if( iso < 0.25 ) muon.isLooseISO = true;  else return;
    if( iso < 0.20 ) muon.isMediumISO = true; else return;
    if( iso < 0.15 ) muon.isTightISO = true;  else return;
  }

  void set_muon_sfs(hzura::Muon & muon, SFCalculator & muon_sf_calculator, const string & sf_type){
    const Double_t & eta = muon.tlv.Eta();
    const Double_t & pt  = muon.tlv.Pt();

    if( sf_type == "loose" ){
      muon.sf_id.c   = muon_sf_calculator.GetSF_id_loose(  eta, pt,  0 );
      muon.sf_id.u   = muon_sf_calculator.GetSF_id_loose(  eta, pt,  1 );
      muon.sf_id.d   = muon_sf_calculator.GetSF_id_loose(  eta, pt, -1 );
      muon.sf_iso.c  = muon_sf_calculator.GetSF_iso_loose(  eta, pt,  0 );
      muon.sf_iso.u  = muon_sf_calculator.GetSF_iso_loose(  eta, pt,  1 );
      muon.sf_iso.d  = muon_sf_calculator.GetSF_iso_loose(  eta, pt, -1 );
    } else 
    if( sf_type == "tight" ){
      muon.sf_id.c   = muon_sf_calculator.GetSF_id_tight(  eta, pt,  0 );
      muon.sf_id.u   = muon_sf_calculator.GetSF_id_tight(  eta, pt,  1 );
      muon.sf_id.d   = muon_sf_calculator.GetSF_id_tight(  eta, pt, -1 );
      muon.sf_iso.c  = muon_sf_calculator.GetSF_iso_tight(  eta, pt,  0 );
      muon.sf_iso.u  = muon_sf_calculator.GetSF_iso_tight(  eta, pt,  1 );
      muon.sf_iso.d  = muon_sf_calculator.GetSF_iso_tight(  eta, pt, -1 );
    }
  }

  // JETS ================================================================================================
  void calc_btag_variables(hzura::Jet & j){ // FIXME 2016 2017 2018
    Events * e = hzura::glob::event;
    const int & i = j.index;

    // b-tagging
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2017#B_tagging
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    // DeepCSV
    // jet.pfDeepCSVJetTags_probb    = j.bDiscriminator("pfDeepCSVJetTags:probb");
    // jet.pfDeepCSVJetTags_probbb   = j.bDiscriminator("pfDeepCSVJetTags:probbb");
    // jet.pfDeepCSVJetTags_probc    = j.bDiscriminator("pfDeepCSVJetTags_probc");
    // jet.pfDeepCSVJetTags_probudsg = j.bDiscriminator("pfDeepCSVJetTags:probudsg");

    // pfDeepCSVJetTags:probb + pfDeepCSVJetTags:probbb 
    j.btag_DeepCSV_val = e->Jets_pfDeepCSVJetTags_probb[i] + e->Jets_pfDeepCSVJetTags_probbb[i];
    j.btag_DeepCSV_isLoose  = (j.btag_DeepCSV_val > 0.1241) ? true : false;
    j.btag_DeepCSV_isMedium = (j.btag_DeepCSV_val > 0.4184) ? true : false;
    j.btag_DeepCSV_isTight  = (j.btag_DeepCSV_val > 0.7527) ? true : false;

    // DeepJet
    // jet.pfDeepFlavourJetTags_probb    = j.bDiscriminator("pfDeepFlavourJetTags_probb");
    // jet.pfDeepFlavourJetTags_probbb   = j.bDiscriminator("pfDeepFlavourJetTags_probbb");
    // jet.pfDeepFlavourJetTags_problepb = j.bDiscriminator("pfDeepFlavourJetTags_problepb");
    // jet.pfDeepFlavourJetTags_probc    = j.bDiscriminator("pfDeepFlavourJetTags:probc");
    // jet.pfDeepFlavourJetTags_probuds  = j.bDiscriminator("pfDeepFlavourJetTags:probuds");
    // jet.pfDeepFlavourJetTags_probg    = j.bDiscriminator("pfDeepFlavourJetTags:probg");

    // pfDeepFlavourJetTags:probb + pfDeepFlavourJetTags:probbb + pfDeepFlavourJetTags:problepb
    j.btag_DeepFlavour_val = e->Jets_pfDeepFlavourJetTags_probb[i] + e->Jets_pfDeepFlavourJetTags_probbb[i] + e->Jets_pfDeepFlavourJetTags_problepb[i];
    j.btag_DeepFlavour_isLoose  = (j.btag_DeepFlavour_val > 0.0494) ? true : false;
    j.btag_DeepFlavour_isMedium = (j.btag_DeepFlavour_val > 0.2770) ? true : false;
    j.btag_DeepFlavour_isTight  = (j.btag_DeepFlavour_val > 0.7264) ? true : false;
  };

  void set_jet_btag_sfs(hzura::Jet & j, const BTagSFReader & btag_sf_calculator){
    // https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Standalone
    const Float_t & pt  = j.tlv.Pt();
    const Float_t & eta = j.tlv.Eta();
    j.sf_btag.c = btag_sf_calculator.GetSF("central", eta, pt); 
    j.sf_btag.u = btag_sf_calculator.GetSF("up", eta, pt); 
    j.sf_btag.d = btag_sf_calculator.GetSF("down", eta, pt); 
  };






};











#endif

