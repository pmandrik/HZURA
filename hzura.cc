
#include <hzura_objects.hh>

#include <Events.hh>
#include <EventsMeta.hh>

class Reader{
  Reader(string input_file){
    TFile * file = TFile::Open( input_file.c_str()  );
    gDirectory->cd("grinderMain");

    TTree * t1 = (TTree*) gDirectory->Get("Events");
    TTree * t2 = (TTree*) gDirectory->Get("EventsMeta");

    Events     * events = new Events( t1 );
    EventsMeta * meta   = new Events( t2 );

    meta->GetEntry( 0 );
  }

  ~Reader(){
    delete meta;
    delete events;
    file->Close();
  }
};

int main(){

  // WORKFLOW :
  // process input files in condor or local
  // 0. read events
  // 1. make some control plots
  // 2. apply basic cuts on events
  // 3. make object reconstruction
  // 4. apply cuts
  // 5. calculate main weights
  // 6. calculate systematics weights

  TH1D * selections = new TH1D("selections", "selections", 100, 0, 100);
  selections->Fill("Total", 0);
  selections->Fill("Total_X_Weight", 0);
  selections->Fill("Selected", 0);
  selections->Fill("Selected_X_Weight", 0);

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
  Float_t Jet_pt_cut;
  Float_t Jet_eta_cut;

  // 0. read events
  Reader * reader = Reader( input_file );
  Events * events = reader->events;
  Long64_t entrys = tree->GetEntries();
  Long64_t entry = 0;

  vector<hzura::Photon>   photon_candidates;
  vector<hzura::Electron> electron_candidates;
  vector<hzura::Muon>     muon_candidates;
  vector<hzura::Jet>      jet_candidates;

  hzura::Photon   photon_candidate;
  hzura::Electron electron_candidate;
  hzura::Muon     muon_candidate;
  hzura::Jet      jet_candidate;

  for(;entry < entrys;entry++){
    events->GetEntry(entry);

    // 0. remove info from previous event
    photon_candidates.clear();
    electron_candidates.clear();
    muon_candidates.clear();
    jet_candidates.clear();

    // 1. make some control plots

    // OBJECT SELECTIONS ==============================================
    // 2. apply basic cuts on events

    // photons =-
    for(int i = 0; i < events->Photons_; i++){
      if( events->Photons_pt[i] < Photons_pt_cut ) continue;
      if( TMath::Abs( reader->Photon_eta[i] ) > Photons_eta_cut ) continue;
      if( TMath::Abs( reader->Photon_eta[i] ) > Photons_eta_hole_cut_start and TMath::Abs( reader->Photon_eta[i] ) < Photons_eta_hole_cut_end ) continue;

      photon_candidate.Set( events, i );
      photon_candidates.emplace_back( photon_candidate );
    }

    // electrons =-
    for(int i = 0; i < events->Electron_; i++){
      if( events->Electron_pt[i] < Electron_pt_cut ) continue;
      if( TMath::Abs( reader->Electron_eta[i] ) > Electron_eta_cut ) continue;
      if( TMath::Abs( reader->Electron_eta[i] ) > Electron_eta_hole_cut_start and TMath::Abs( reader->Electron_eta[i] ) < Electron_eta_hole_cut_end ) continue;

      electron_candidate.Set( events, i );
      electron_candidates.emplace_back( electron_candidate );
    }

    // muons =-
    for(int i = 0; i < events->Muon_; i++){
      if( events->Muon_pt[i] < Muon_pt_cut ) continue;
      if( TMath::Abs( reader->Muon_eta[i] ) > Muon_eta_cut ) continue;

      muon_candidate.Set( events, i );
      muon_candidates.emplace_back( muon_candidate );
    }

    // jets =-
    for(int i = 0; i < events->Jets_; i++){
      if( events->Jet_pt[i] < Jet_pt_cut ) continue;
      if( TMath::Abs( reader->Muon_eta[i] ) > Jet_eta_cut ) continue;

      jet_candidate.Set( events, i );
      jet_candidates.emplace_back( jet_candidate );
    }
    
    // EVENT SELECTIONS ==============================================
    // 3. make object reconstruction

    // 4. apply cuts
    if()

    // 5. calculate main weights
    // 6. calculate systematics weights
  }

  return 0;
}

