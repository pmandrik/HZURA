
// extra libraries
#include "pmlib_msg.hh"

// hzura
#include <hzura_objects.hh>
#include <hzura_analyses_helpers.hh>
#include <hzura_selection_helpers.hh>

// GRIDNER interface
#include <Events.C>
#include <EventsMeta.C>

using namespace hzura;
using namespace pmlib;

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

  int verbose_lvl = pmlib::verbose::VERBOSE;
  hzura::glob::year_era = "2016";
  get_muons_sf_reader();

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

  // -1. run options
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
  Float_t Muon_pt_cut  = 20;
  Float_t Muon_eta_cut = 2.4;

  // FIXME
  Float_t Jet_pt_cut  = 20;
  Float_t Jet_eta_cut = 3;

  // 0. read events
  Reader * reader = new Reader( input_file );
  hzura::glob::event = reader->event;
  Events * event     = reader->event;

  Long64_t entrys = hzura::glob::event->fChain->GetEntriesFast();
  Long64_t entry = 0;

  vector<hzura::Photon>   photon_candidates;
  vector<hzura::Electron> electron_candidates;
  vector<hzura::Muon>     muon_candidates;
  vector<hzura::Jet>      jet_candidates;
  vector<hzura::Jet>      bjet_candidates;

  hzura::Photon   photon_candidate;
  hzura::Electron electron_candidate;
  hzura::Muon     muon_candidate;
  hzura::Jet      jet_candidate;

  MSG_INFO("hzura::main(): start loop, total number of events", entrys);
  entrys = TMath::Min( (Long64_t)100, entrys);

  for(;entry < entrys; entry++){
    hzura::glob::event->GetEntry(entry);
    
    if( not (entry % 1000) )
      pmlib::msg_progress( float(entry)/entrys );

    // 0. remove info from previous event
    photon_candidates.clear();
    electron_candidates.clear();
    muon_candidates.clear();
    jet_candidates.clear();
    bjet_candidates.clear();

    // 1. make some control plots
    // msg("Event ... ");
    // for(int i = 0; i < event->trigger_fires.size(); i++) msg( "trigger", i, event->trigger_fires[i] );

    // OBJECT SELECTIONS ==============================================
    // 2. apply basic cuts on events

    // photons =-
    for(int i = 0; i < event->Photons_; i++){
      photon_candidate.Init( i );
      apply_energy_correction( photon_candidate, "ecalEnergyPostCorr" );
      TLorentzVector & vec = photon_candidate.tlv;

      if( vec.Pt() < Photons_pt_cut ) continue;
      if( TMath::Abs( vec.Eta() ) > Photons_eta_cut ) continue;
      if( TMath::Abs( vec.Eta() ) > Photons_eta_hole_cut_start and TMath::Abs( vec.Eta() ) < Photons_eta_hole_cut_end ) continue;
      if( not event->Photons_isLoose[i] ) continue;

      photon_candidates.emplace_back( photon_candidate );
    }

    // electrons =-
    for(int i = 0; i < event->Electrons_; i++){
      electron_candidate.Init( i );
      apply_energy_correction( electron_candidate, "ecalTrkEnergyPostCorr" );
      TLorentzVector & vec = electron_candidate.tlv;

      if( vec.Pt() < Electron_pt_cut ) continue;
      if( TMath::Abs( vec.Eta() ) > Electron_eta_cut ) continue;
      if( TMath::Abs( vec.Eta() ) > Electron_eta_hole_cut_start and TMath::Abs( vec.Eta() ) < Electron_eta_hole_cut_end ) continue;
      if( not event->Electrons_isLoose[i] ) continue;

      electron_candidates.emplace_back( electron_candidate );
    }

    // muons =-
    for(int i = 0; i < event->Muons_; i++){
      if( event->Muons_pt[i] < Muon_pt_cut ) continue;
      if( TMath::Abs( event->Muons_eta[i] ) > Muon_eta_cut ) continue;
      if( not event->Muons_isLoose[i] ) continue;

      // TODO add loose medium tight ISO selections ...
      // TODO add calc of muon SFs ...
      // TODO add calc of events based on muons SFs ...

      // TODO SFs for photons   ???
      // TODO SFs for electrons ???

      muon_candidate.Init( i );
      muon_candidates.emplace_back( muon_candidate );
    }

    // jets =-
    for(int i = 0; i < event->Jets_; i++){
      if( event->Jets_pt[i] < Jet_pt_cut ) continue;
      if( TMath::Abs( event->Jets_eta[i] ) > Jet_eta_cut ) continue;
      if( not event->Jets_isTight[i] ) continue;

      jet_candidate.Init( i );

      hzura::calc_btag_variables( & jet_candidate );

      if( jet_candidate.btag_DeepCSV_isTight ) 
        bjet_candidates.emplace_back( jet_candidate );
      else
         jet_candidates.emplace_back( jet_candidate );
    }

    // 2.5 make some control plots
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

    for(auto p : muon_pairs) h_muon_pairs_m->Fill( p.tlv.M() );
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


















