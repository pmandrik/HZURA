
#ifndef hzura_objects_preselectors_h
#define hzura_objects_preselectors_h


namespace hzura {

  class ObjectPreselector {
    public:
    ObjectPreselector(){
      muon_sf_calculator     = get_muons_sf_reader();
      electron_sf_calculator = get_electrons_sf_reader();
      photon_sf_calculator   = get_photons_sf_reader();
      btag_sf_calculator_b   = get_btag_sf_reader("tight", "b");
      btag_sf_calculator_c   = get_btag_sf_reader("tight", "c");
      btag_sf_calculator_l   = get_btag_sf_reader("tight", "l");
      pileup_sf_calculator = get_pileup_sf_reader();
      jec_unc_calculator   = get_jec_uncertanties();

      verbose_lvl = 1; // FIXME
    }

    SFCalculator muon_sf_calculator     ;
    SFCalculator electron_sf_calculator ;
    SFCalculator photon_sf_calculator   ;
    BTagSFReader btag_sf_calculator_b, btag_sf_calculator_c, btag_sf_calculator_l ;
    PileUpSFReader pileup_sf_calculator ;
    JecReader jec_unc_calculator        ;
    BTagEffReader btag_eff_reader       ;
    //Events * event;
    ReaderGRINDER * event;
    
    int verbose_lvl;

    // gen particles ================================================================================================
    std::shared_ptr< std::vector<hzura::GenParticle> > PreselectGenParticles( const hzura::EventCfg & cfg ){
      MSG_DEBUG("hzura::ObjectPreselector.PreselectGenParticles():  ... ");
      std::shared_ptr< std::vector<hzura::GenParticle> > genparticles ( new std::vector<hzura::GenParticle> );
      if( not cfg.USE_GENPARTICLES ) return genparticles;
      hzura::GenParticle genparticle;
      for(int i = 0, i_max = event->GetGenParticlesN(); i < i_max; i++){
        genparticle.Init( i );

        if( cfg.GENPARTICLES_SELECT_STATUS_CODES.size() )
          if( find(cfg.GENPARTICLES_SELECT_STATUS_CODES.begin(), cfg.GENPARTICLES_SELECT_STATUS_CODES.end(), genparticle.status ) != cfg.GENPARTICLES_SELECT_STATUS_CODES.end() )continue;

        if( cfg.GENPARTICLES_SELECT_IDS.size() )
         if( find(cfg.GENPARTICLES_SELECT_IDS.begin(), cfg.GENPARTICLES_SELECT_IDS.end(), genparticle.pdg_id) != cfg.GENPARTICLES_SELECT_IDS.end() ) continue;

        genparticles->emplace_back( genparticle );
      }
      MSG_DEBUG("hzura::ObjectPreselector.PreselectGenParticles():  ... end");
      return genparticles;
    }

    // photons ================================================================================================
    std::shared_ptr< std::vector<hzura::Photon> > PreselectPhotons( const hzura::EventCfg & cfg ){
      MSG_DEBUG("hzura::ObjectPreselector.PreselectPhotons():  ... ");
      // cfg.PHOTON_ENERGY_CORRECTION_TYPE
      // cfg.PHOTON_PT_CUT
      // cfg.PHOTON_ETA_CUT
      // cfg.PHOTON_ETA_HOLE_CUT_START
      // cfg.PHOTON_ETA_HOLE_CUT_END
      // cfg.PHOTON_ID_CUT
      // auto deleter = [](std::vector<hzura::Photon> * p){ std::cout << "[deleter called]\n"; delete p; };
      // std::shared_ptr< std::vector<hzura::Photon> > photon_candidates ( new std::vector<hzura::Photon> , deleter);
      std::shared_ptr< std::vector<hzura::Photon> > photon_candidates ( new std::vector<hzura::Photon> );
      hzura::Photon   photon_candidate;

      for(int i = 0, i_max = event->GetPhotonsN(); i < i_max; i++){
        photon_candidate.Init( i );
        if( cfg.PHOTON_ENERGY_CORRECTION_TYPE.size() ) apply_energy_correction( photon_candidate, cfg.PHOTON_ENERGY_CORRECTION_TYPE );
        TLorentzVector & vec = photon_candidate.tlv;

        if( vec.Pt() < cfg.PHOTON_PT_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.PHOTON_ETA_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.PHOTON_ETA_HOLE_CUT_START and TMath::Abs( vec.Eta() ) < cfg.PHOTON_ETA_HOLE_CUT_END ) continue;

        if( cfg.PHOTON_ID_CUT == hzura::id_names::loose  and not event->GetPhotonIsLoose(i) )  continue;
        if( cfg.PHOTON_ID_CUT == hzura::id_names::medium and not event->GetPhotonIsMedium(i) ) continue;
        if( cfg.PHOTON_ID_CUT == hzura::id_names::tight  and not event->GetPhotonIsTight(i) )  continue;
        if( cfg.PHOTON_SET_SFS ) set_egamma_sfs( photon_candidate, photon_sf_calculator, cfg.PHOTON_ID_CUT );
        else photon_candidate.sf.Set( 1.f );

        photon_candidates->emplace_back( photon_candidate );
      }

      MSG_DEBUG("hzura::ObjectPreselector.preselect_photons(): photon_candidates.size() =", photon_candidates->size());
      return photon_candidates;
    }

    std::shared_ptr< std::vector<hzura::Photon> > PreselectFlashggPhotons( const hzura::EventCfg & cfg ){
      MSG_DEBUG("hzura::ObjectPreselector.preselect_photons():  ... ");
      std::shared_ptr< std::vector<hzura::Photon> > photon_candidates ( new std::vector<hzura::Photon> );
      hzura::Photon   photon_candidate;

      int N_start_index = 2*cfg.FLASHGG_DIPHOTON_INDEX;
      int N_end_index   = 2*cfg.FLASHGG_DIPHOTON_INDEX + 2;
      for(int i = N_start_index; i < N_end_index; i++){
        photon_candidate.Init( i );
        TLorentzVector & vec = photon_candidate.tlv;

        if( not check_pass_flashgg_mva( photon_candidate ) ) continue;
        if( vec.Pt() < cfg.PHOTON_PT_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.PHOTON_ETA_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.PHOTON_ETA_HOLE_CUT_START and TMath::Abs( vec.Eta() ) < cfg.PHOTON_ETA_HOLE_CUT_END ) continue;

        photon_candidate.sf.Set( 1.f );
        photon_candidates->emplace_back( photon_candidate );
      }

      MSG_DEBUG("hzura::ObjectPreselector.preselect_photons(): photon_candidates.size() =", photon_candidates->size());
      return photon_candidates;
    }

    // electrons ================================================================================================
    std::shared_ptr< std::vector<hzura::Electron> > PreselectElectrons( const hzura::EventCfg &  cfg ){
      MSG_DEBUG("hzura::ObjectPreselector.preselect_electrons():  ... ");
      // cfg.ELECTRON_ENERGY_CORRECTION_TYPE
      // cfg.ELECTRON_PT_CUT
      // cfg.ELECTRON_ETA_CUT
      // cfg.ELECTRON_ETA_HOLE_CUT_START
      // cfg.ELECTRON_ETA_HOLE_CUT_END
      // cfg.ELECTRON_ID_CUT
      std::shared_ptr< std::vector<hzura::Electron> > electron_candidates (new std::vector<hzura::Electron>);
      hzura::Electron   electron_candidate;

      for(int i = 0, i_max = event->GetElectronsN(); i < i_max; i++){
        electron_candidate.Init( i );
        if( not hzura::glob::is_data and cfg.ELECTRON_ENERGY_CORRECTION_TYPE.size() ) apply_energy_correction( electron_candidate, cfg.ELECTRON_ENERGY_CORRECTION_TYPE ); // such as "ecalTrkEnergyPostCorr"
        TLorentzVector & vec = electron_candidate.tlv;

        if( vec.Pt() < cfg.ELECTRON_PT_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.ELECTRON_ETA_CUT ) continue;
        if( TMath::Abs( vec.Eta() ) > cfg.ELECTRON_ETA_HOLE_CUT_START and TMath::Abs( vec.Eta() ) < cfg.ELECTRON_ETA_HOLE_CUT_END ) continue;

        if( cfg.ELECTRON_ID_CUT == hzura::id_names::loose  and not event->GetElectronIsLoose(i) )  continue;
        if( cfg.ELECTRON_ID_CUT == hzura::id_names::medium and not event->GetElectronIsMedium(i) ) continue;
        if( cfg.ELECTRON_ID_CUT == hzura::id_names::tight  and not event->GetElectronIsTight(i) )  continue;
        if( not hzura::glob::is_data ) set_egamma_sfs( electron_candidate, electron_sf_calculator, cfg.ELECTRON_ID_CUT );

        if(cfg.FLASHGG_DIPHOTON_INDEX >= 0 and event->GetElectronDiphotonsVeto( i ).at(cfg.FLASHGG_DIPHOTON_INDEX) ) continue;

        electron_candidates->emplace_back( electron_candidate );
      }

      MSG_DEBUG("hzura::ObjectPreselector.preselect_electrons(): electron_candidates.size() =", electron_candidates->size());
      return electron_candidates;
    }

    // muons ================================================================================================
    std::shared_ptr< std::vector<hzura::Muon> > PreselectMuons( const hzura::EventCfg & cfg ){
      MSG_DEBUG("hzura::ObjectPreselector.preselect_muons():  ... ");
      // cfg.MUON_PT_CUT
      // cfg.MUON_ETA_CUT
      // cfg.MUON_ISOID_CUT
      std::shared_ptr< std::vector<hzura::Muon> > muon_candidates ( new std::vector<hzura::Muon> );
      hzura::Muon   muon_candidate;

      for(int i = 0, i_max = event->GetMuonsN(); i < i_max; i++){
        if( event->GetMuonPt(i) < cfg.MUON_PT_CUT ) continue;
        if( TMath::Abs( event->GetMuonEta(i) ) > cfg.MUON_ETA_CUT ) continue;

        if( cfg.MUON_ISOID_CUT == hzura::id_names::loose  and event->GetMuonIsLoose(i)  ) continue;
        if( cfg.MUON_ISOID_CUT == hzura::id_names::medium and event->GetMuonIsMedium(i) ) continue;
        if( cfg.MUON_ISOID_CUT == hzura::id_names::tight  and event->GetMuonIsTight(i)  ) continue;

        muon_candidate.Init( i );

        // calculate muon ISO working points
        hzura::calc_muon_iso( muon_candidate );
        if( cfg.MUON_ISOID_CUT == hzura::id_names::loose  and muon_candidate.isLooseISO )  continue;
        if( cfg.MUON_ISOID_CUT == hzura::id_names::medium and muon_candidate.isMediumISO ) continue;
        if( cfg.MUON_ISOID_CUT == hzura::id_names::tight  and muon_candidate.isTightISO )  continue;

        if(cfg.FLASHGG_DIPHOTON_INDEX >= 0 and event->GetMuonDiphotonsVeto( i ).at(cfg.FLASHGG_DIPHOTON_INDEX) ) continue;

        // calculate SFs for choosen combination of ISO and ID cuts
        if( not hzura::glob::is_data ) hzura::set_muon_sfs( muon_candidate, muon_sf_calculator, cfg.MUON_ISOID_CUT );

        muon_candidates->emplace_back( muon_candidate );
      }
      MSG_DEBUG("hzura::ObjectPreselector.preselect_muons(): muon_candidates.size() =", muon_candidates->size());
      return muon_candidates;
    }

    // jets ================================================================================================
    void PreselectJets( const hzura::EventCfg &  cfg, hzura::HzuraEvent & hzura_event ){
      MSG_DEBUG("hzura::ObjectPreselector.preselect_jets():  ... ");
      // cfg.JET_PT_CUT
      // cfg.JET_ETA_CUT
      // cfg.JET_ID_CUT
      // cfg.JET_BTAGGER
      // cfg.JET_BTAGGER_ID
      // cfg.JET_JER
      // cfg.JET_JEC_TYPE
      // cfg.JET_JEC_DIR

      hzura_event.ljet_candidates = std::shared_ptr< std::vector<hzura::Jet> > ( new std::vector<hzura::Jet> );
      hzura_event.bjet_candidates = std::shared_ptr< std::vector<hzura::Jet> > ( new std::vector<hzura::Jet> );
      hzura::Jet      jet_candidate;
      bool btagged = false;

      jec_unc_calculator.SetActiveJetCorrectionUncertainty( cfg.JET_JEC_TYPE );
      for(int i = 0, i_max = event->GetJetsN(); i < i_max; i++){
        jet_candidate.Init( i );
        // Evaluate JER smearing after nominal JEC is applied but before systematic variaion in JEC - only for MC FIXME
        if( not hzura::glob::is_data ) apply_jer_smearing( jet_candidate, cfg.JET_JER ); // such as "central"

        // Remake jets due to JEC systematic variaion
        if( not hzura::glob::is_data ) jec_unc_calculator.RemakeJet( jet_candidate, cfg.JET_JEC_DIR ); // worked in case if SetActiveJetCorrectionUncertainty called before

        // basic selections
        if( jet_candidate.tlv.Pt() < cfg.JET_PT_CUT ) continue;
        if( TMath::Abs( jet_candidate.tlv.Eta() ) > cfg.JET_ETA_CUT ) continue;
        if( cfg.JET_ID_CUT == hzura::id_names::tight  and not event->GetJetIsTight(i) )  continue; // only tight jets are available

        // split based on btagging decigion
        btagged = false;
        hzura::calc_btag_variables( jet_candidate );

        if( cfg.JET_BTAGGER == "DeepCSV" ){
          hzura::set_jet_btag_sfs( jet_candidate, btag_sf_calculator_b, btag_sf_calculator_c, btag_sf_calculator_l ); 
          if(cfg.JET_BTAGGER_ID == hzura::id_names::loose and jet_candidate.btag_DeepCSV_isLoose )   btagged = true;
          if(cfg.JET_BTAGGER_ID == hzura::id_names::medium and jet_candidate.btag_DeepCSV_isMedium ) btagged = true;
          if(cfg.JET_BTAGGER_ID == hzura::id_names::tight and jet_candidate.btag_DeepCSV_isTight )   btagged = true;
        }
        else if( cfg.JET_BTAGGER == "DeepFlavour" ){
          hzura::set_jet_btag_sfs( jet_candidate, btag_sf_calculator_b, btag_sf_calculator_c, btag_sf_calculator_l ); 
          if(cfg.JET_BTAGGER_ID == hzura::id_names::loose and jet_candidate.btag_DeepFlavour_isLoose )   btagged = true;
          if(cfg.JET_BTAGGER_ID == hzura::id_names::medium and jet_candidate.btag_DeepFlavour_isMedium ) btagged = true;
          if(cfg.JET_BTAGGER_ID == hzura::id_names::tight and jet_candidate.btag_DeepFlavour_isTight )   btagged = true;
        }

        if( btagged ) hzura_event.bjet_candidates->emplace_back( jet_candidate );
        else          hzura_event.ljet_candidates->emplace_back( jet_candidate );
      }

      if(btag_eff_reader.Valid()){
        for( hzura::Jet & jet : *(hzura_event.bjet_candidates) ) jet.btag_eff = btag_eff_reader.GetEff( jet.tlv.Pt(), jet.tlv.Eta(), jet.hadronFlavour );
        for( hzura::Jet & jet : *(hzura_event.ljet_candidates) ) jet.btag_eff = btag_eff_reader.GetEff( jet.tlv.Pt(), jet.tlv.Eta(), jet.hadronFlavour );
      }

      MSG_DEBUG("hzura::ObjectPreselector.preselect_jets(): bjet_candidates->size() ljet_candidates->size() =", hzura_event.bjet_candidates->size(), hzura_event.ljet_candidates->size());
    }

    // get_events_from_cfgs ================================================================================================
    std::vector<hzura::HzuraEvent> PreselectEvents( const std::vector<hzura::EventCfg> & cfgs ){
      MSG_DEBUG("hzura::ObjectPreselector.get_events_from_cfgs(): begin ... ");
      event = hzura::glob::event;
      vector<HzuraEvent> answer;

      // PrimaryVertexes =- nothing to do ... 
      // Pile-up reweight using
      // https://github.com/pfs/BJetEnergyPeak/blob/master/scripts/runPileupEstimation.py

      // calculate default met
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
      MSG_DEBUG("hzura::ObjectPreselector.get_events_from_cfgs(): calculate default met ... ");
      hzura::MET event_met;
      event_met.Init();
      set_met_filter_flag( event_met );

      //===================================
      MSG_DEBUG( "hzura::ObjectPreselector.get_events_from_cfgs(): loop over cfgs ... " );
      for(int i = 0; i < cfgs.size(); ++i){
        MSG_DEBUG( "hzura::ObjectPreselector.get_events_from_cfgs(): process cfg N ... ", i);
        HzuraEvent event_i;

        const hzura::EventCfg & cfg_i = cfgs[i];

        // use previously calcullated objects 
        for(int j = 0; j < i; ++j){
          const hzura::EventCfg & cfg_j = cfgs[j];
          const HzuraEvent & event_j = answer[j];

          if( cfg_i.SamePhotons( cfg_j )  )   event_i.photon_candidates   = event_j.photon_candidates;
          if( cfg_i.SameElectrons( cfg_j )  ) event_i.electron_candidates = event_j.electron_candidates;
          if( cfg_i.SameMuons( cfg_j )  )     event_i.muon_candidates     = event_j.muon_candidates;
          if( cfg_i.SameJets( cfg_j )  ) { 
            event_i.ljet_candidates     = event_j.ljet_candidates;
            event_i.bjet_candidates     = event_j.bjet_candidates;
          }
          if( cfg_i.SameGenParticles( cfg_j )  ) event_i.genparticles = event_j.genparticles;
        }

        if( not event_i.photon_candidates   ) event_i.photon_candidates   = PreselectFlashggPhotons( cfg_i ); // PreselectPhotons( cfg_i ); FIXME - ok for now
        if( not event_i.electron_candidates ) event_i.electron_candidates = PreselectElectrons( cfg_i );
        if( not event_i.muon_candidates     ) event_i.muon_candidates     = PreselectMuons( cfg_i );
        if( not event_i.ljet_candidates or not event_i.bjet_candidates )    PreselectJets( cfg_i, event_i );
        if( not event_i.genparticles ) event_i.genparticles = PreselectGenParticles( cfg_i );

        // in any cases calculate MET
        event_i.met = event_met;
        
        // MET uncertanties, such uncertanties should be synchronised with ones applyed for jets
        if( not cfg_i.MET_SYS.size() ){
               if( cfg_i.JET_JER == "up" )   apply_met_systematic_variation( event_i.met, "JetResUp"   );
          else if( cfg_i.JET_JER == "down" ) apply_met_systematic_variation( event_i.met, "JetResDown" );
          else if( cfg_i.JET_JEC_TYPE.size() and     cfg_i.JET_JEC_DIR ) apply_met_systematic_variation( event_i.met, "JetResUp"   ); // not a very correct but only available options out of the box
          else if( cfg_i.JET_JEC_TYPE.size() and not cfg_i.JET_JEC_DIR ) apply_met_systematic_variation( event_i.met, "JetResDown" );
        }
        else apply_met_systematic_variation( event_i.met, cfg_i.MET_SYS );

        // apply met phi correction
        // METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, int year, bool isMC, int npv)
        if( cfg_i.MET_XYCORR ){
          std::pair<double,double> corr_pt_phi = METXYCorr_Met_MetPhi( event_i.met.pt, event_i.met.phi, hzura::glob::event->GetEventRun(), hzura::glob::year, hzura::glob::is_data, hzura::glob::event->GetEventRecoNumInteractions() );
          event_i.met.pt  = corr_pt_phi.first;
          event_i.met.phi = corr_pt_phi.second;
        }

        answer.emplace_back( event_i );
      }

      MSG_DEBUG("hzura::ObjectPreselector.get_events_from_cfgs(): return ... ");
      return answer;
    }

    // print infos about selected events ================================================================================================
    void DumpEventsInfo( const std::vector<hzura::EventCfg> & cfgs, const std::vector<hzura::HzuraEvent> & events ) const {

      int N_configs = cfgs.size();
      if(events.size() != N_configs or not N_configs) return;

      int t_size_y = 6;
      std::vector<std::string> line = std::vector<std::string>(N_configs+1, "");
      std::vector< std::vector<std::string> > table;
      for(int i = 0; i < t_size_y; ++i)
        table.push_back( line );

      // header
      std::vector<std::string> fline; 
      for(int i = 0; i < N_configs; ++i){
        const hzura::EventCfg & config = cfgs[i];
        table[0][i+1] = cfgs[i].name;
        const hzura::HzuraEvent & hevents = events[i]; 
        table[1][i+1] = to_string( hevents.photon_candidates->size() );     
        table[2][i+1] = to_string( hevents.electron_candidates->size() );   
        table[3][i+1] = to_string( hevents.muon_candidates->size() );        
        table[4][i+1] = to_string( hevents.ljet_candidates->size() );        
        table[5][i+1] = to_string( hevents.bjet_candidates->size() );        
      }
      table[1][0] = "N photons";
      table[2][0] = "N electrons";
      table[3][0] = "N muons";
      table[4][0] = "N l-jets";
      table[5][0] = "N b-jets";

      pm::print_as_table( table );
    }
  };

  // FIND B_TAGGING EFFICIENCY ===============================================================================================
  void calc_btagging_efficiency(std::string outname, const EventCfg & cfg, const PileUpSFReader & pileup_sf_calculator, const int & N_bins_pt, const double & pt_min, const double & pt_max, const int & N_bins_eta, const double & eta_min, const double & eta_max){
    TFile * outfile = new TFile(outname.c_str(), "RECREATE");
    std::vector<TH2D*> tots, tags, effs;
    for(int i = 0; i < 6; i++){
      tots.push_back( new TH2D(("tot_" + to_string(i)).c_str(), ("tot_" + to_string(i)).c_str(), N_bins_pt, pt_min, pt_max, N_bins_eta, eta_min, eta_max) );
      tags.push_back( new TH2D(("tag_" + to_string(i)).c_str(), ("tag_" + to_string(i)).c_str(), N_bins_pt, pt_min, pt_max, N_bins_eta, eta_min, eta_max) );
      effs.push_back( new TH2D(("eff_" + to_string(i)).c_str(), ("eff_" + to_string(i)).c_str(), N_bins_pt, pt_min, pt_max, N_bins_eta, eta_min, eta_max) );
    }

    hzura::glob::event->file->cd();
    hzura::ObjectPreselector preselector;
    vector<EventCfg> analyses_configs = { cfg };
    Long64_t entrys = hzura::glob::event->GetEntries();
    Long64_t entry = 0;
    for(;entry < entrys; entry++){
      hzura::glob::event->GetEntry(entry);
      if( not (entry % 1000) ) pm::msg_progress( float(entry)/entrys );

      vector<hzura::HzuraEvent> hzura_events = preselector.PreselectEvents( analyses_configs );
      const hzura::HzuraEvent & hevents = hzura_events.at(0); 
      const hzura::EventCfg   & config  = analyses_configs.at(0); 

      std::vector<hzura::Jet>       & ljets     = *(hevents.ljet_candidates);
      std::vector<hzura::Jet>       & bjets     = *(hevents.bjet_candidates);

      //   EventWeights calc_event_weight( const vector<Photon> & selected_photons, const vector<Electron> & selected_electrons, const vector<Muon> & selected_muons,
      //                                   const vector<Jet> & selected_ljets,  const vector<Jet> & selected_bjets, const PileUpSFReader & pileup_sf_calculator )
      EventWeights ew = calc_event_weight( vector<Photon>(), vector<Electron>(), vector<Muon>(), vector<Jet>(), vector<Jet>(), pileup_sf_calculator );
      double event_weight = ew.combined_weight;;

      for(auto jet : ljets){
        if(jet.tlv.Pt() < pt_min) msg( jet.tlv.Pt() );
        tots.at( jet.hadronFlavour )->Fill( jet.tlv.Pt(), jet.tlv.Eta(), event_weight );
      }

      for(auto jet : bjets){
        tots.at( jet.hadronFlavour )->Fill( jet.tlv.Pt(), jet.tlv.Eta(), event_weight );
        tags.at( jet.hadronFlavour )->Fill( jet.tlv.Pt(), jet.tlv.Eta(), event_weight );
      }
    }

    for(int i = 0; i < 6; i++){
      TH2D* eff = effs.at(i);
      TH2D* tag = tags.at(i);
      TH2D* tot = tots.at(i);
      for(int x = 0; x <= tot->GetNbinsX()+1; x++){
        for(int y = 0; y <= tot->GetNbinsY()+1; y++){
          double tot_value = tot->GetBinContent( x, y );
          double tag_value = tag->GetBinContent( x, y );
          if( tot_value < 0.000000000001 ) { tag_value = 0; tot_value = 1; };
          eff->SetBinContent( x, y, tag_value / tot_value );
        }
      }
    }
    
    outfile->cd();
    outfile->Write();
    outfile->Close();
  }

};


#endif







