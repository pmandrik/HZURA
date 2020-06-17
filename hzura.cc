
// extra libraries
#include "external/PMANDRIK_LIBRARY/pmlib_msg.hh"
using namespace pm;

// external libraries 
// https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Standalone
#include "external/BTagCalibrationStandalone.h"
#include "external/BTagCalibrationStandalone.cpp"
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
#include "external/XYMETCorrection.h"
// JEC
// https://github.com/cms-sw/cmssw/tree/CMSSW_11_1_0_pre2/CondFormats/JetMETObjects/src
// https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_11_1_0_pre2/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h
// https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_11_1_0_pre2/CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h
// https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_11_1_0_pre2/CondFormats/JetMETObjects/src/JetCorrectorParameters.cc
// https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_11_1_0_pre2/CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc
// https://twiki.cern.ch/twiki/bin/view/CMS/JECUncertaintySources
// https://github.com/andrey-popov/mensura/tree/d9cf55dc809b1eef89d2d7bb79d0d3b15b6feee0/src/external/JERC
#include "external/JERC/SimpleJetCorrectionUncertainty.cpp"
#include "external/JERC/JetCorrectionUncertainty.cpp"
#include "external/JERC/JetCorrectorParameters.cpp"

// hzura
#include "hzura_cfg.hh"
#include <hzura_objects.hh>
#include <hzura_analyses_helpers.hh>
#include <hzura_analyse_configuration_interfaces.hh>
#include <hzura_analyse_configuration_helpers.hh>
#include <hzura_objects_helpers.hh>
#include <hzura_objects_preselectors.hh>
#include <hzura_output.hh>
using namespace hzura;

// GRIDNER interface
#include <Events.C>
#include <EventsMeta.C>


class Reader{
  public:
  Reader(string input_file){
    file = TFile::Open( input_file.c_str()  );
    gDirectory->cd("grinderMain");

    TTree * t1 = (TTree*) gDirectory->Get("Events");
    TTree * t2 = (TTree*) gDirectory->Get("EventsMeta");

    event  = new Events( t1 );
    meta   = new EventsMeta( t2 );

    meta->GetEntry( 0 );
  }

  ~Reader(){
    delete meta;
    delete event;
    file->Close();
  }

    Events     * event;
    EventsMeta * meta;
    TFile * file;
};

int main(int argc, char *argv[]) { // FIXME
  if( argc < 2 ){
    return 1;
  }
  string input_file = argv[1];

  int verbose_lvl = pm::verbose::VERBOSE;
  hzura::glob::Init( "2017", nullptr );
  hzura::ObjectPreselector preselector;
  preselector.verbose_lvl = pm::verbose::VERBOSE;

  MSG_INFO("hzura::main(): process input file", input_file);

  // WORKFLOW :
  // process input files in condor or local
  // 0. read events
  // 1. make some control plots
  // 2. apply basic cuts on events
  // 3. make object reconstruction
  // 4. apply cuts
  // 5. calculate main weights
  // 6. calculate systematics weights
  // TODO measure b-tag efficiency in MC samples https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation#Useful_Tools

  // create histos
  TFile * file_out = new TFile("hzura_out.root", "RECREATE");
  TH1D * selections = new TH1D("selections", "selections", 100, 0, 100);
  selections->Fill("Total", 0);
  selections->Fill("Total_X_Weight", 0);
  selections->Fill("Selected", 0);
  selections->Fill("Selected_X_Weight", 0);

  TH1D * h_phoLooseIsoCharged  = new TH1D("h_phoLooseIsoCharged", "h_phoLooseIsoCharged", 100, 0, 5);
  TH1D * h_phoLooseIsoNeutral  = new TH1D("h_phoLooseIsoNeutral", "h_phoLooseIsoNeutral", 100, 0, 5);
  TH1D * h_phoLooseIsoPhoton   = new TH1D("h_phoLooseIsoPhoton", "h_phoLooseIsoPhoton", 100, 0, 5);
  TH1D * h_phoMediumIsoCharged = new TH1D("h_phoMediumIsoCharged", "h_phoMediumIsoCharged", 100, 0, 5);
  TH1D * h_phoMediumIsoNeutral = new TH1D("h_phoMediumIsoNeutral", "h_phoMediumIsoNeutral", 100, 0, 5);
  TH1D * h_phoMediumIsoPhoton  = new TH1D("h_phoMediumIsoPhoton", "h_phoMediumIsoPhoton", 100, 0, 5);
  TH1D * h_phoTightIsoCharged  = new TH1D("h_phoTightIsoCharged", "h_phoTightIsoCharged", 100, 0, 5);
  TH1D * h_phoTightIsoNeutral  = new TH1D("h_phoTightIsoNeutral", "h_phoTightIsoNeutral", 100, 0, 5);
  TH1D * h_phoTightIsoPhoton   = new TH1D("h_phoTightIsoPhoton", "h_phoTightIsoPhoton", 100, 0, 5);

  TH1D * h_ljets_pt = new TH1D("h_ljets_pt", "h_ljets_pt", 100, 0, 300);
  TH1D * h_bjets_pt = new TH1D("h_bjets_pt", "h_bjets_pt", 100, 0, 300);

  TH1D * h_jets_pairs_m  = new TH1D("h_jets_pairs_m",  "h_jets_pairs_m",  100, 0, 300);
  TH1D * h_bjets_pairs_m = new TH1D("h_bjets_pairs_m", "h_bjets_pairs_m", 100, 0, 300);

  TH1D * h_photon_pairs_m  = new TH1D("h_photon_pairs_m",  "h_photon_pairs_m",  100, 0, 300);

  TH1D * h_electon_pairs_m     = new TH1D("h_electon_pairs_m",     "h_electon_pairs_m",     100, 0, 300);
  TH1D * h_electon_SC_pairs_m  = new TH1D("h_electon_SC_pairs_m",  "h_electon_SC_pairs_m",  100, 0, 300);
  TH1D * h_electon_DC_pairs_m  = new TH1D("h_electon_DC_pairs_m",  "h_electon_DC_pairs_m",  100, 0, 300);

  TH1D * h_muon_pairs_m     = new TH1D("h_muon_pairs_m",     "h_muon_pairs_m",     100, 0, 300);
  TH1D * h_muon_SC_pairs_m  = new TH1D("h_muon_SC_pairs_m",  "h_muon_SC_pairs_m",  100, 0, 300);
  TH1D * h_muon_DC_pairs_m  = new TH1D("h_muon_DC_pairs_m",  "h_muon_DC_pairs_m",  100, 0, 300);

  TH1D * h_mue_pairs_m     = new TH1D("h_mue_pairs_m",     "h_mue_pairs_m",     100, 0, 300);
  TH1D * h_mue_SC_pairs_m  = new TH1D("h_mue_SC_pairs_m",  "h_mue_SC_pairs_m",  100, 0, 300);
  TH1D * h_mue_DC_pairs_m  = new TH1D("h_mue_DC_pairs_m",  "h_mue_DC_pairs_m",  100, 0, 300);

  // -1. set run options ================================================================================================
  // https://arxiv.org/pdf/1804.02716.pdf
  Float_t Photons_pt_cut  = 20;
  Float_t Photons_eta_hole_cut_start = 1.44;
  Float_t Photons_eta_hole_cut_end   = 1.57;
  Float_t Photons_eta_cut = 2.5;

  // https://arxiv.org/pdf/1812.10529.pdf
  Float_t Electron_pt_cut  = 20;
  Float_t Electron_eta_hole_cut_start = 1.479;
  Float_t Electron_eta_hole_cut_end   = 1.566;
  Float_t Electron_eta_cut = 2.5;

  // https://arxiv.org/pdf/1807.06325.pdf
  // https://arxiv.org/pdf/1706.09936.pdf

  // TODO
  EventCfg cfg     = EventCfg();
  cfg.name = "Basic";
  // https://arxiv.org/pdf/1804.02716.pdf
  cfg.PHOTON_ENERGY_CORRECTION_TYPE = ""; // "ecalEnergyPostCorr";
  cfg.PHOTON_PT_CUT  = 20;
  cfg.PHOTON_ETA_CUT = 2.5;
  cfg.PHOTON_ETA_HOLE_CUT_START = 1.44;
  cfg.PHOTON_ETA_HOLE_CUT_END = 1.57;
  cfg.PHOTON_ID_CUT = id_names::none;                        // id_names::loose id_names::medium id_names::tight
  cfg.PHOTON_SET_SFS = false; // not going to set them for flashgg
  // https://arxiv.org/pdf/1812.10529.pdf
  cfg.ELECTRON_ENERGY_CORRECTION_TYPE = "ecalEnergyPostCorr"; // same as PHOTON_ENERGY_CORRECTION_TYPE
  cfg.ELECTRON_PT_CUT = 20;
  cfg.ELECTRON_ETA_CUT = 2.5;;
  cfg.ELECTRON_ETA_HOLE_CUT_START = 1.479;
  cfg.ELECTRON_ETA_HOLE_CUT_END = 1.566;
  cfg.ELECTRON_ID_CUT = id_names::tight;      // id_names::loose id_names::medium id_names::tight
  // https://arxiv.org/pdf/1807.06325.pdf
  // https://arxiv.org/pdf/1706.09936.pdf
  cfg.MUON_PT_CUT    = 20;
  cfg.MUON_ETA_CUT   = 2.4;
  cfg.MUON_ISOID_CUT = id_names::tight;       // id_names::loose id_names::medium id_names::tight
  cfg.JET_PT_CUT = 20;
  cfg.JET_ETA_CUT = 3;
  cfg.JET_ID_CUT = id_names::tight;           // id_names::loose id_names::medium id_names::tight
  cfg.JET_BTAGGER = "DeepCSV";                // "DeepCSV"
  cfg.JET_BTAGGER_ID = id_names::tight;       // id_names::loose id_names::medium id_names::tight
  cfg.JET_JER = "central";                    // "central" "up" "down"
  cfg.JET_JEC_TYPE = "";                      // "Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"
  cfg.JET_JEC_DIR = true;
  cfg.MET_SYS     = "";                       // "UnclusteredEnUp" "UnclusteredEnDown"
  cfg.MET_XYCORR = true;

  EventCfg cfg_uncorrected = cfg;
  cfg_uncorrected.name = "Uncorrected";
  cfg_uncorrected.ELECTRON_ENERGY_CORRECTION_TYPE = "";
  cfg_uncorrected.PHOTON_ENERGY_CORRECTION_TYPE   = "";

  EventCfg cfg_zero = EventCfg();
  cfg_zero.name = "Zero selections";
  std::vector<hzura::EventCfg> analyses_configs = { cfg, cfg_uncorrected, cfg_zero };
  std::vector<TH1D*> selections_vec;
  for(auto config : analyses_configs) 
    selections_vec.push_back( new TH1D( (config.name + "selections").c_str(), (config.name + "selections").c_str(), 100, 0, 1)) );

  // setup output variables ================================================================================================
  TFile * output_file = TFile::Open();
      double dR_yy, dR_jj, dR_WL, dR_WW, dR_HH;
      double m_yy, y1_Et_div_m_yy, y2_Et_div_m_yy
      TLorentzVector y1_tlv, y2_tlv, H_yy_tlv, ljet1_tlv, ljet2_tlv, W_jj_tlv, mu_leading_tlv, el_leading_tlv, lep_leading_tlv;
      TLorentzVector nu_reco_tlv, W_elnu_tlv, H_WW_tlv, nu_tlv, HH_tlv, HWl_tlv;

  // 0. read events ================================================================================================
  input_file.cd();
  Reader * reader = new Reader( input_file );
  Events * event     = reader->event;
  hzura::glob::event = event;

  Long64_t entrys = hzura::glob::event->fChain->GetEntriesFast();
  Long64_t entry = 0;

  vector<hzura::Electron> electron_candidates;
  vector<hzura::Muon>     muon_candidates;
  vector<hzura::Jet>      jet_candidates;
  vector<hzura::Jet>      bjet_candidates;

  hzura::Electron electron_candidate;
  hzura::Muon     muon_candidate;
  hzura::Jet      jet_candidate;
  hzura::MET      met_candidate;

  MSG_INFO("hzura::main(): start loop, total number of events", entrys);
  entrys = TMath::Min( (Long64_t)1000, entrys);

  for(;entry < entrys; entry++){
    hzura::glob::event->GetEntry(entry);
    
    if( not (entry % 1000) )
      pm::msg_progress( float(entry)/entrys );

    // 0. remove info from previous event

    // OBJECT SELECTIONS ==============================================
    // 2. apply basic cuts on events
    vector<hzura::HzuraEvent> hzura_events = preselector.PreselectEvents( analyses_configs );
    preselector.DumpEventsInfo( analyses_configs, hzura_events );

    for(int i = 0, imax = hzura_events.size(); i < imax; ++i){
      const hzura::HzuraEvent & hevents = hzura_events[i]; 
      const hzura::EventCfg & config = analyses_configs[i];
      TH1D * selections              = selections_vec[i];

      msg( "process cfg: ", config.name );

      std::vector<hzura::Photon>    & photons = *(hevents.photon_candidates);
      std::vector<hzura::Electron>  & electrons = *(hevents.electron_candidates);
      std::vector<hzura::Muon>      & muons = *(hevents.muon_candidates);
      std::vector<hzura::Jet>       & ljets = *(hevents.ljet_candidates);
      std::vector<hzura::Jet>       & bjets = *(hevents.bjet_candidates);
      hzura::MET & met = hevents.met;

      // EVENT SELECTIONS ==============================================
      hevents.selection_weight = false;                   // remove event by default

      // At least one lepton
      if( electrons.size() + muons.size() < 1 ) continue;
      hevents.selections->Fill("At least one lepton", 1);

      // No b-jets:
      if( ljets.size() < 2 ) continue;
      selections->Fill("At least two light jets", 1);

      // at least two photons
      if( photons.size() < 2) continue;
      selections->Fill("At least two photons", 1);

      y1_tlv = photons[0].tlv ;
      y2_tlv  = photons[1].tlv ;
      H_yy_tlv = y1_tlv + y2_tlv ;

      // M yy in [105, 160]
      if( H_yy_tlv.M() < 105 or H_yy_tlv.M() > 160 ) continue;
      selections->Fill("H_yy_tlv.M() in [105, 160]", 1);

      hevents.selection_weight = true;
      // EVENT HLV RECONSTRUCTION ==============================================
      // W->jj candidate
      ljet1_tlv = ljets[0].tlv;
      ljet2_tlv = ljets[1].tlv;
      W_jj_tlv = ljet_1 + ljet_2;


      // W->elnu candidate
      // el
      int leadin_mu_index = get_leading_by_pt( muons );
      int leadin_el_index = get_leading_by_pt( electrons );
      if( leadin_mu_index >= 0 ) mu_leading_tlv = muons[ leadin_mu_index ];
      if( leadin_el_index >= 0 ) el_leading_tlv = electrons[ leadin_el_index ];
      lep_leading_tlv = mu_leading_tlv;
      if( el_leading_tlv.Pt() > lep_leading_tlv.Pt() ) lep_leading_tlv = el_leading_tlv;

      // nu
      nu_tlv.SetPtEtaPhiM(met.pt, 0, met.phi, 0.0);

      // H->WW
      bool HH_reconstructed = hzura::reconstruct_H_from_WW( nu_tlv, W_jj_tlv + el_leading_tlv, nu_reco_tlv, H_WW_tlv);
      W_elnu_tlv = nu_reco_tlv + el_leading_tlv;
      
      // HH
      HH_tlv = H_yy_tlv + H_WW_tlv;
      
      // HWL
      HWl_tlv = nu_reco_tlv + H_WW_tlv;

      // Delta Rs
      dR_yy  = y1_tlv.DeltaR( y2_tlv );
      dR_jj  = ljet1_tlv.DeltaR( ljet2_tlv );
      dR_WL  = W_jj_tlv.DeltaR( el_leading_tlv );
      dR_WW  = -1, dR_HH = -1;
      if( HH_reconstructed ) {
        dR_WW  = W_jj_tlv.DeltaR( W_elnu_tlv );
        dR_HH = H_yy_tlv.DeltaR( H_WW_tlv )
      }

      // Delta Phi
      double dPhi_nuL = nu_tlv.DeltaPhi( el_leading_tlv );

      // other
      m_yy = H_yy_tlv.M();
      y1_Et_div_m_yy = y1_tlv.Et() / m_yy;
      y2_Et_div_m_yy = y2_tlv.Et() / m_yy;
    }


    /*
    // 1. make some control plots
    // msg("Event ... ");
    // for(int i = 0; i < event->trigger_fires.size(); i++) msg( "trigger", i, event->trigger_fires[i] );

    // 2.5 make some control plots ====================================================================================
    // check if ISO is used in ID criteria
    for( auto p : photon_candidates ){
      const Float_t & sumChargedHadronPt = hzura::glob::event->Photons_sumChargedHadronPt[ p.index ]; 
      const Float_t & sumNeutralHadronEt = hzura::glob::event->Photons_sumNeutralHadronEt[ p.index ]; 
      const Float_t & sumPhotonEt        = hzura::glob::event->Photons_sumPhotonEt[ p.index ]; 

      const bool & is_loose  = hzura::glob::event->Photons_isLoose[ p.index ]; 
      const bool & is_medium = hzura::glob::event->Photons_isMedium[ p.index ]; 
      const bool & is_tight  = hzura::glob::event->Photons_isTight[ p.index ]; 

      const Float_t & pt = 1.;

      if( is_loose ){ 
        h_phoLooseIsoCharged->Fill( sumChargedHadronPt / pt );
        h_phoLooseIsoNeutral->Fill( sumNeutralHadronEt / pt );
        h_phoLooseIsoPhoton ->Fill( sumPhotonEt / pt );
      }
      if( is_medium ){ 
        h_phoMediumIsoCharged->Fill( sumChargedHadronPt / pt );
        h_phoMediumIsoNeutral->Fill( sumNeutralHadronEt / pt );
        h_phoMediumIsoPhoton ->Fill( sumPhotonEt / pt );
      }
      if( is_tight ){ 
        h_phoTightIsoCharged->Fill( sumChargedHadronPt / pt );
        h_phoTightIsoNeutral->Fill( sumNeutralHadronEt / pt );
        h_phoTightIsoPhoton ->Fill( sumPhotonEt / pt );
      }
    }

    // photons pairs
    vector<Particle> photon_pairs;
    make_combinations(photon_candidates, photon_pairs);
    // if( photon_pairs.size() ) msg( "photons", event->Photons_, photon_candidates.size(), photon_pairs.size() );
    for( int i = 0; i < photon_candidates.size(); i++ ){
      auto tlv_1 = photon_candidates[i].tlv;
      for( int j = i+1; j < photon_candidates.size(); j++ ){
        auto tlv_2 = photon_candidates[j].tlv;
        msg( (tlv_1 + tlv_2).M() );
      }
    }

    for( auto p : photon_pairs ){
      // msg( p.tlv.M() );
      h_photon_pairs_m->Fill( p.tlv.M() );
    }

    // electron pairs
    vector<Particle> electron_pairs;
    make_combinations( electron_candidates, electron_pairs );
    //msg( "electrons ", event->Electrons_, electron_candidates.size(), electron_pairs.size() );
    if( not electron_pairs.size() ) continue;

    vector<Particle> electron_pairs_SC;
    make_sameCharge_combinations( electron_candidates, electron_pairs_SC );

    vector<Particle> electron_pairs_DC;
    make_diffCharge_combinations( electron_candidates,  electron_pairs_DC );

    for(auto p : electron_pairs)    h_electon_pairs_m->Fill( p.tlv.M() );
    for(auto p : electron_pairs_SC) h_electon_SC_pairs_m->Fill( p.tlv.M() );
    for(auto p : electron_pairs_DC) h_electon_DC_pairs_m->Fill( p.tlv.M() );

    // muon pairs
    vector<Particle> muon_pairs;
    make_combinations( muon_candidates, muon_pairs );
    //msg( "muons ", event->Muons_, muon_candidates.size(), muon_pairs.size() );

    vector<Particle> muon_pairs_SC;
    make_sameCharge_combinations( muon_candidates, muon_pairs_SC );

    vector<Particle> muon_pairs_DC;
    make_diffCharge_combinations( muon_candidates, muon_pairs_DC );

    for(auto p : muon_pairs)    h_muon_pairs_m->Fill( p.tlv.M() );
    for(auto p : muon_pairs_SC) h_muon_SC_pairs_m->Fill( p.tlv.M() );
    for(auto p : muon_pairs_DC) h_muon_DC_pairs_m->Fill( p.tlv.M() );

    // emu pairs
    vector<Particle> emu_pairs;
    make_combinations( electron_candidates, muon_candidates, emu_pairs );
    //msg( "emy pairs ", emu_pairs.size() );

    vector<Particle> emu_pairs_SC;
    make_sameCharge_combinations( electron_candidates, muon_candidates, emu_pairs_SC );

    vector<Particle> emu_pairs_DC;
    make_diffCharge_combinations( electron_candidates, muon_candidates, emu_pairs_DC );

    for(auto p : emu_pairs) h_mue_pairs_m->Fill( p.tlv.M() );
    for(auto p : emu_pairs_SC) h_mue_SC_pairs_m->Fill( p.tlv.M() );
    for(auto p : emu_pairs_DC) h_mue_DC_pairs_m->Fill( p.tlv.M() );
  

    // jets
    for( auto j : bjet_candidates ){
      h_bjets_pt->Fill( j.tlv.Pt() );
    }

    for( auto j : jet_candidates ){
      h_ljets_pt->Fill( j.tlv.Pt() );
    }

    // jets pairs
    for( int i = 0; i < bjet_candidates.size(); i++ ){
      auto tlv_1 = bjet_candidates[i].tlv;
      for( int j = i+1; j < bjet_candidates.size(); j++ ){
        auto tlv_2 = bjet_candidates[j].tlv;
        h_bjets_pairs_m->Fill( (tlv_1 + tlv_2).M() );
      }
    }

    for( int i = 0; i < jet_candidates.size(); i++ ){
      auto tlv_1 = jet_candidates[i].tlv;
      for( int j = i+1; j < jet_candidates.size(); j++ ){
        auto tlv_2 = jet_candidates[j].tlv;
        h_jets_pairs_m->Fill( (tlv_1 + tlv_2).M() );
      }
    }
    
    // EVENT SELECTIONS ==============================================
    // 3. make object reconstruction

    // 4. apply cuts

    // 5. calculate main weights
    // 6. calculate systematics weights
    // TODO calc central and event weights based on muons SFs ...

    auto selected_photons   = photon_candidates;
    auto selected_electrons = electron_candidates;
    auto selected_muons     = muon_candidates;
    auto selected_ljets     = jet_candidates;
    auto selected_bjets     = bjet_candidates;
    auto wgt = calc_event_weight(selected_photons, selected_electrons, selected_muons, selected_ljets,  selected_bjets, pileup_sf_calculator);

    wgt.Print();
    */
  }

  file_out->cd();
  file_out->Write();

  return 0;
}



int main_root_wrapper(string input_file) {
  char * argv [5];
  const char argv_1[] = "main";
  const char * argv_2 = input_file.c_str();

  argv[0] = (char*)argv_1;
  argv[1] = (char*)argv_2;

  int answer = main(2, argv);
  return answer;
};


















