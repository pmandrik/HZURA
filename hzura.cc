
// extra libraries
#include "external/PMANDRIK_LIBRARY/pmlib_msg.hh"
#include "external/PMANDRIK_LIBRARY/pmlib_other.hh"
using namespace pm;

// external libraries 
#include <LHAPDF/LHAPDF.h>

// https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Standalone
#include "external/BTagCalibrationStandalone.h"
#include "external/BTagCalibrationStandalone.cpp"
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
#include "external/XYMETCorrection.h"
#include "/afs/cern.ch/work/p/pmandrik/dihiggs/8_mvanaluse/VSEVA/HHWWgg/reweight/reweight_HH.C"
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

// #define GRINDER_PATH "/afs/cern.ch/work/p/pmandrik/dihiggs/4_slashgg/CMSSW_10_5_0/src/Analysis/GRINDER/interface/Event.hh"
// #define GRINDER_PATH "/afs/cern.ch/work/p/pmandrik/dihiggs/5_slashgg_10_6_8/CMSSW_10_6_8/src/Analysis/GRINDER/interface/Event.hh"
#define GRINDER_PATH "/afs/cern.ch/work/p/pmandrik/dihiggs/5_slashgg_10_6_8_RE2/CMSSW_10_6_8/src/Analysis/GRINDER/interface/Event.hh"
#include GRINDER_PATH

class ReaderGRINDER :  public PmMsg {
  public:
    ReaderGRINDER(string input_file){
      MSG_INFO( "hzura::Reader(): process ", input_file, "..." );
      file = TFile::Open( input_file.c_str()  );

      t_Events = (TTree*) gDirectory->Get("GrinderflashggHHWWggTag/Events");
      t_EventsMeta = (TTree*) gDirectory->Get("GrinderflashggHHWWggTag/EventsMeta");

      msg( "ReaderGRINDER ... ", t_Events, t_EventsMeta );

      string gp = GRINDER_PATH;
      gInterpreter->GenerateDictionary( "std::vector<grinder::Electron>",    ("vector;" + gp).c_str() );
      gInterpreter->GenerateDictionary( "std::vector<grinder::Muon>",        ("vector;" + gp).c_str() );
      gInterpreter->GenerateDictionary( "std::vector<grinder::Photon>",      ("vector;" + gp).c_str() );
      gInterpreter->GenerateDictionary( "std::vector<grinder::Jet>",         ("vector;" + gp).c_str() );
      gInterpreter->GenerateDictionary( "std::vector<grinder::GenParticle>", ("vector;" + gp).c_str() );

      electrons = 0;
      muons     = 0;
      photons   = 0;
      jets      = 0;
      genparticles = 0;
      met       = 0;
      event     = 0;

      t_Events->SetBranchAddress("Electrons",  &electrons);
      t_Events->SetBranchAddress("Muons",      &muons);
      t_Events->SetBranchAddress("Photons",    &photons);
      t_Events->SetBranchAddress("Jets",       &jets);
      t_Events->SetBranchAddress("MET",        &met);
      t_Events->SetBranchAddress("Event",      &event);
      t_Events->SetBranchAddress("Particles",  &genparticles);

      msg( "ReaderGRINDER ... ", genparticles );
 
      eventmetadata = 0;
      t_EventsMeta->SetBranchAddress("EventMeta",      &eventmetadata);

      n_events_total = 0;
      for (Long64_t ev=0, nentries = t_EventsMeta->GetEntries(); ev<nentries; ev++) {
        t_EventsMeta->GetEntry(ev);
        n_events_total += eventmetadata->numEvents;
        // cout << ev << "/" << nentries << " " << n_events_total-eventmetadata->numEvents << "+" << eventmetadata->numEvents << endl;
      }
      t_EventsMeta->GetEntry( 0 );
      msg( "N MiniAOD files = ", t_EventsMeta->GetEntries(), " N events = ", n_events_total);
      msg( "ReaderGRINDER ... ok " );
    }

    void GetEntry(Long64_t index){ t_Events->GetEntry( index ); }
    Long64_t GetEntries(){ return t_Events->GetEntriesFast(); }

    ~ReaderGRINDER(){
      file->Close();
    }

    std::vector<grinder::Electron> * electrons ;
    std::vector<grinder::Muon>     * muons ;
    std::vector<grinder::Photon>   * photons ;
    std::vector<grinder::Jet>      * jets ;
    std::vector<grinder::GenParticle> * genparticles ;
    grinder::MET * met ;
    grinder::Event * event ;
    grinder::EventMetadata * eventmetadata;

    Long64_t n_events_total;
    TFile * file;
    TTree * t_Events, * t_EventsMeta;

    Int_t GetPhotonsN(){ return photons->size(); }
    Int_t GetElectronsN(){ return electrons->size(); }
    Int_t GetMuonsN(){ return muons->size(); }
    Int_t GetJetsN(){ return jets->size(); }
    Int_t GetGenParticlesN(){ return genparticles->size(); }
    //=========================class_name Photon
    Float_t GetPhotonPt (const int & index) const { return  photons->at(index).pt ; }
    Float_t GetPhotonEta (const int & index) const { return  photons->at(index).eta ; }
    Float_t GetPhotonPhi (const int & index) const { return  photons->at(index).phi ; }
    Bool_t GetPhotonIsLoose (const int & index) const { return  photons->at(index).isLoose ; }
    Bool_t GetPhotonIsMedium (const int & index) const { return  photons->at(index).isMedium ; }
    Bool_t GetPhotonIsTight (const int & index) const { return  photons->at(index).isTight ; }
    Float_t GetPhotonMvaValue (const int & index) const { return  photons->at(index).mva_value ; }
    Int_t GetPhotonMvaCategory (const int & index) const { return  photons->at(index).mva_category ; }
    Float_t GetPhotonSumChargedHadronPt (const int & index) const { return  photons->at(index).sumChargedHadronPt ; }
    Float_t GetPhotonSumNeutralHadronEt (const int & index) const { return  photons->at(index).sumNeutralHadronEt ; }
    Float_t GetPhotonSumPhotonEt (const int & index) const { return  photons->at(index).sumPhotonEt ; }
    Float_t GetPhotonSumPUPt (const int & index) const { return  photons->at(index).sumPUPt ; }
    Float_t GetPhotonEcalEnergyPreCorr (const int & index) const { return  photons->at(index).ecalEnergyPreCorr ; }
    Float_t GetPhotonEcalEnergyPostCorr (const int & index) const { return  photons->at(index).ecalEnergyPostCorr ; }
    Float_t GetPhotonEnergyScaleValue (const int & index) const { return  photons->at(index).energyScaleValue ; }
    Float_t GetPhotonEnergySigmaValue (const int & index) const { return  photons->at(index).energySigmaValue ; }
    Float_t GetPhotonEnergyScaleUp (const int & index) const { return  photons->at(index).energyScaleUp ; }
    Float_t GetPhotonEnergyScaleDown (const int & index) const { return  photons->at(index).energyScaleDown ; }
    Float_t GetPhotonEnergyScaleStatUp (const int & index) const { return  photons->at(index).energyScaleStatUp ; }
    Float_t GetPhotonEnergyScaleStatDown (const int & index) const { return  photons->at(index).energyScaleStatDown ; }
    Float_t GetPhotonEnergyScaleSystUp (const int & index) const { return  photons->at(index).energyScaleSystUp ; }
    Float_t GetPhotonEnergyScaleSystDown (const int & index) const { return  photons->at(index).energyScaleSystDown ; }
    Float_t GetPhotonEnergyScaleGainUp (const int & index) const { return  photons->at(index).energyScaleGainUp ; }
    Float_t GetPhotonEnergyScaleGainDown (const int & index) const { return  photons->at(index).energyScaleGainDown ; }
    Float_t GetPhotonEnergyScaleEtUp (const int & index) const { return  photons->at(index).energyScaleEtUp ; }
    Float_t GetPhotonEnergyScaleEtDown (const int & index) const { return  photons->at(index).energyScaleEtDown ; }
    Float_t GetPhotonEnergySigmaUp (const int & index) const { return  photons->at(index).energySigmaUp ; }
    Float_t GetPhotonEnergySigmaDown (const int & index) const { return  photons->at(index).energySigmaDown ; }
    Float_t GetPhotonEnergySigmaPhiUp (const int & index) const { return  photons->at(index).energySigmaPhiUp ; }
    Float_t GetPhotonEnergySigmaPhiDown (const int & index) const { return  photons->at(index).energySigmaPhiDown ; }
    Float_t GetPhotonEnergySigmaRhoUp (const int & index) const { return  photons->at(index).energySigmaRhoUp ; }
    Float_t GetPhotonEnergySigmaRhoDown (const int & index) const { return  photons->at(index).energySigmaRhoDown ; }
    //=========================class_name Electron
    std::vector<Bool_t> GetElectronDiphotonsVeto (const int & index) const { return  electrons->at(index).diphotons_veto ; }
    Float_t GetElectronPt (const int & index) const { return  electrons->at(index).pt ; }
    Float_t GetElectronEta (const int & index) const { return  electrons->at(index).eta ; }
    Float_t GetElectronPhi (const int & index) const { return  electrons->at(index).phi ; }
    Int_t GetElectronCharge (const int & index) const { return  electrons->at(index).charge ; }
    Bool_t GetElectronIsLoose (const int & index) const { return  electrons->at(index).isLoose ; }
    Bool_t GetElectronIsMedium (const int & index) const { return  electrons->at(index).isMedium ; }
    Bool_t GetElectronIsTight (const int & index) const { return  electrons->at(index).isTight ; }
    Float_t GetElectronSumChargedHadronPt (const int & index) const { return  electrons->at(index).sumChargedHadronPt ; }
    Float_t GetElectronSumNeutralHadronEt (const int & index) const { return  electrons->at(index).sumNeutralHadronEt ; }
    Float_t GetElectronSumPhotonEt (const int & index) const { return  electrons->at(index).sumPhotonEt ; }
    Float_t GetElectronSumPUPt (const int & index) const { return  electrons->at(index).sumPUPt ; }
    Float_t GetElectronEcalTrkEnergyPreCorr (const int & index) const { return  electrons->at(index).ecalTrkEnergyPreCorr ; }
    Float_t GetElectronEcalTrkEnergyPostCorr (const int & index) const { return  electrons->at(index).ecalTrkEnergyPostCorr ; }
    Float_t GetElectronEnergyScaleValue (const int & index) const { return  electrons->at(index).energyScaleValue ; }
    Float_t GetElectronEnergySigmaValue (const int & index) const { return  electrons->at(index).energySigmaValue ; }
    Float_t GetElectronEnergyScaleUp (const int & index) const { return  electrons->at(index).energyScaleUp ; }
    Float_t GetElectronEnergyScaleDown (const int & index) const { return  electrons->at(index).energyScaleDown ; }
    Float_t GetElectronEnergyScaleStatUp (const int & index) const { return  electrons->at(index).energyScaleStatUp ; }
    Float_t GetElectronEnergyScaleStatDown (const int & index) const { return  electrons->at(index).energyScaleStatDown ; }
    Float_t GetElectronEnergyScaleSystUp (const int & index) const { return  electrons->at(index).energyScaleSystUp ; }
    Float_t GetElectronEnergyScaleSystDown (const int & index) const { return  electrons->at(index).energyScaleSystDown ; }
    Float_t GetElectronEnergyScaleGainUp (const int & index) const { return  electrons->at(index).energyScaleGainUp ; }
    Float_t GetElectronEnergyScaleGainDown (const int & index) const { return  electrons->at(index).energyScaleGainDown ; }
    Float_t GetElectronEnergyScaleEtUp (const int & index) const { return  electrons->at(index).energyScaleEtUp ; }
    Float_t GetElectronEnergyScaleEtDown (const int & index) const { return  electrons->at(index).energyScaleEtDown ; }
    Float_t GetElectronEnergySigmaUp (const int & index) const { return  electrons->at(index).energySigmaUp ; }
    Float_t GetElectronEnergySigmaDown (const int & index) const { return  electrons->at(index).energySigmaDown ; }
    Float_t GetElectronEnergySigmaPhiUp (const int & index) const { return  electrons->at(index).energySigmaPhiUp ; }
    Float_t GetElectronEnergySigmaPhiDown (const int & index) const { return  electrons->at(index).energySigmaPhiDown ; }
    Float_t GetElectronEnergySigmaRhoUp (const int & index) const { return  electrons->at(index).energySigmaRhoUp ; }
    Float_t GetElectronEnergySigmaRhoDown (const int & index) const { return  electrons->at(index).energySigmaRhoDown ; }
    //=========================class_name Muon
    std::vector<Bool_t> GetMuonDiphotonsVeto (const int & index) const { return  muons->at(index).diphotons_veto ; }
    Float_t GetMuonPt (const int & index) const { return  muons->at(index).pt ; }
    Float_t GetMuonEta (const int & index) const { return  muons->at(index).eta ; }
    Float_t GetMuonPhi (const int & index) const { return  muons->at(index).phi ; }
    Float_t GetMuonRelIsoTrk (const int & index) const { return  muons->at(index).relIsoTrk ; }
    Float_t GetMuonRelIsoPF (const int & index) const { return  muons->at(index).relIsoPF ; }
    Int_t GetMuonCharge (const int & index) const { return  muons->at(index).charge ; }
    Bool_t GetMuonIsLoose (const int & index) const { return  muons->at(index).isLoose ; }
    Bool_t GetMuonIsMedium (const int & index) const { return  muons->at(index).isMedium ; }
    Bool_t GetMuonIsTight (const int & index) const { return  muons->at(index).isTight ; }
    //=========================class_name Jet
    Float_t GetJetPtRaw (const int & index) const { return  jets->at(index).ptRaw ; }
    Float_t GetJetEtaRaw (const int & index) const { return  jets->at(index).etaRaw ; }
    Float_t GetJetPhiRaw (const int & index) const { return  jets->at(index).phiRaw ; }
    Float_t GetJetMRaw (const int & index) const { return  jets->at(index).mRaw ; }
    Float_t GetJetCharge (const int & index) const { return  jets->at(index).charge ; }
    Float_t GetJetArea (const int & index) const { return  jets->at(index).area ; }
    Float_t GetJetPt (const int & index) const { return  jets->at(index).pt ; }
    Float_t GetJetEta (const int & index) const { return  jets->at(index).eta ; }
    Float_t GetJetPhi (const int & index) const { return  jets->at(index).phi ; }
    Float_t GetJetM (const int & index) const { return  jets->at(index).m ; }
    std::vector<Float_t> GetJetJECUncVU (const int & index) const { return  jets->at(index).JEC_unc_v_u ; }
    std::vector<Float_t> GetJetJECUncVD (const int & index) const { return  jets->at(index).JEC_unc_v_d ; }
    Float_t GetJetResolution (const int & index) const { return  jets->at(index).resolution ; }
    Float_t GetJetSf (const int & index) const { return  jets->at(index).sf ; }
    Float_t GetJetSfU (const int & index) const { return  jets->at(index).sf_u ; }
    Float_t GetJetSfD (const int & index) const { return  jets->at(index).sf_d ; }
    Float_t GetJetGetJetPt (const int & index) const { return  jets->at(index).getJet_pt ; }
    Bool_t GetJetIsTight (const int & index) const { return  jets->at(index).isTight ; }
    Int_t GetJetHadronFlavour (const int & index) const { return  jets->at(index).hadronFlavour ; }
    Int_t GetJetPartonFlavour (const int & index) const { return  jets->at(index).partonFlavour ; }
    Float_t GetJetPUJID (const int & index) const { return  jets->at(index).PUJID ; }
    Float_t GetJetPfDeepCSVJetTagsProbb (const int & index) const { return  jets->at(index).pfDeepCSVJetTags_probb ; }
    Float_t GetJetPfDeepCSVJetTagsProbbb (const int & index) const { return  jets->at(index).pfDeepCSVJetTags_probbb ; }
    Float_t GetJetPfDeepCSVJetTagsProbc (const int & index) const { return  jets->at(index).pfDeepCSVJetTags_probc ; }
    Float_t GetJetPfDeepCSVJetTagsProbudsg (const int & index) const { return  jets->at(index).pfDeepCSVJetTags_probudsg ; }
    Float_t GetJetPfDeepFlavourJetTagsProbb (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_probb ; }
    Float_t GetJetPfDeepFlavourJetTagsProbbb (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_probbb ; }
    Float_t GetJetPfDeepFlavourJetTagsProblepb (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_problepb ; }
    Float_t GetJetPfDeepFlavourJetTagsProbc (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_probc ; }
    Float_t GetJetPfDeepFlavourJetTagsProbuds (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_probuds ; }
    Float_t GetJetPfDeepFlavourJetTagsProbg (const int & index) const { return  jets->at(index).pfDeepFlavourJetTags_probg ; }
    //=========================class_name MET
    Float_t GetMETPt (void) const { return  met->pt ; }
    Float_t GetMETEta (void) const { return  met->eta ; }
    Float_t GetMETPhi (void) const { return  met->phi ; }
    Float_t GetMETGenPt (void) const { return  met->gen_pt ; }
    Float_t GetMETGenPhi (void) const { return  met->gen_phi ; }
    Float_t GetMETSignificance (void) const { return  met->significance ; }
    std::vector<Float_t> GetMETPtUncV (void) const { return  met->pt_unc_v ; }
    std::vector<Float_t> GetMETPhiUncV (void) const { return  met->phi_unc_v ; }
    Bool_t GetMETFlagGoodVertices (void) const { return  met->Flag_goodVertices ; }
    Bool_t GetMETFlagGlobalSuperTightHalo2016Filter (void) const { return  met->Flag_globalSuperTightHalo2016Filter ; }
    Bool_t GetMETFlagHBHENoiseFilter (void) const { return  met->Flag_HBHENoiseFilter ; }
    Bool_t GetMETFlagHBHENoiseIsoFilter (void) const { return  met->Flag_HBHENoiseIsoFilter ; }
    Bool_t GetMETFlagEcalDeadCellTriggerPrimitiveFilter (void) const { return  met->Flag_EcalDeadCellTriggerPrimitiveFilter ; }
    Bool_t GetMETFlagBadPFMuonFilter (void) const { return  met->Flag_BadPFMuonFilter ; }
    Bool_t GetMETFlagBadChargedCandidateFilter (void) const { return  met->Flag_BadChargedCandidateFilter ; }
    Bool_t GetMETFlagEeBadScFilter (void) const { return  met->Flag_eeBadScFilter ; }
    Bool_t GetMETFlagEcalBadCalibReducedMINIAODFilter (void) const { return  met->Flag_ecalBadCalibReducedMINIAODFilter ; }
    //=========================class_name GenParticle
    Float_t GetGenParticlePt (const int & index) const { return  genparticles->at(index).pt ; }
    Float_t GetGenParticleEta (const int & index) const { return  genparticles->at(index).eta ; }
    Float_t GetGenParticlePhi (const int & index) const { return  genparticles->at(index).phi ; }
    Float_t GetGenParticleM (const int & index) const { return  genparticles->at(index).m ; }
    Int_t GetGenParticlePdgId (const int & index) const { return  genparticles->at(index).pdg_id ; }
    Int_t GetGenParticleStatus (const int & index) const { return  genparticles->at(index).status ; }
    //=========================class_name Event
    UInt_t GetEventRun (void) const { return  event->run ; }
    UInt_t GetEventLumi (void) const { return  event->lumi ; }
    ULong64_t GetEventEvent (void) const { return  event->event ; }
    UShort_t GetEventBunchCrossing (void) const { return  event->bunchCrossing ; }
    Float_t GetDiphotonMva() const { return event->diphoton_mva ; }
    Float_t GetEventAngularPtDensity (void) const { return  event->angular_pt_density ; }
    Float_t GetEventAngularPtDensityCentral (void) const { return  event->angular_pt_density_central ; }
    Float_t GetEventWeight (void) const { return  event->weight ; }
    Float_t GetEventOriginalXWGTUP (void) const { return  event->originalXWGTUP ; }
    std::vector<Float_t> GetEventWeights (void) const { return  event->weights ; }
    std::vector<Float_t> GetEventPsWeights (void) const { return  event->ps_weights ; }
    Int_t GetEventDicedMCNumInteractions (void) const { return  event->DicedMCNumInteractions ; }
    Int_t GetEventTrueMCNumInteractions (void) const { return  event->TrueMCNumInteractions ; }
    Int_t GetEventRecoNumInteractions (void) const { return  event->RecoNumInteractions ; }
    std::vector<Float_t> GetEventTriggerFires (void) const { return  event->trigger_fires ; }
    Float_t GetEventFlashggPuweight (void) const { return  event->flashgg_puweight ; }
    Float_t GetEventFlashggNvtx (void) const { return  event->flashgg_nvtx ; }
    Float_t GetEventFlashggNpu (void) const { return  event->flashgg_npu ; }
    Float_t GetEventFlashggWeight (void) const { return  event->flashgg_weight ; }
    std::vector<Float_t> GetEventFlashggMcWeights (void) const { return  event->flashgg_mc_weights ; }
    std::vector<Float_t> GetEventFlashggDiphotonWeights (void) const { return  event->flashgg_diphoton_weights ; }
    //=========================class_name EventMetadata
    bool GetEventMetadataIsData (void) const { return  eventmetadata->is_data ; }
    ULong64_t GetEventMetadataNumEvents (void) const { return  eventmetadata->numEvents ; }
    long double GetEventMetadataSumWeights (void) const { return  eventmetadata->sumWeights ; }
    long double GetEventMetadataOriginalXWGTUP (void) const { return  eventmetadata->originalXWGTUP ; }
    std::vector<std::string> GetEventMetadataSelectionsTriggersNames (void) const { return  eventmetadata->selections_triggers_names ; }
};

// hzura
#include "hzura_cfg.hh"
#include <hzura_objects.hh>
#include <hzura_analyses_helpers.hh>
#include <hzura_analyse_configuration_interfaces.hh>
#include <hzura_analyse_configuration_helpers.hh>
#include <hzura_objects_helpers.hh>
#include <hzura_objects_preselectors.hh>
using namespace hzura;

int main(int argc, char *argv[]) { // FIXME
  if( argc < 4 ){
    return 1;
  }
  string input_file   = argv[1];
  string output_file  = argv[2];
  string DATASET_TYPE = argv[3];
  string RUNMODE      = argv[4];
  string OTHER_PARAMETERS = argv[5];

  int verbose_lvl = pm::verbose::VERBOSE;
  hzura::glob::parameters = new RunParameters( OTHER_PARAMETERS );
  bool PRINT_WEIGHTS      = hzura::glob::parameters->CheckBool( "PRINT_WEIGHTS" );
  bool GEN_PDF_HESSIAN    = hzura::glob::parameters->CheckBool( "GEN_PDF_HESSIAN" );
  bool TARGET_PDF_HESSIAN = hzura::glob::parameters->CheckBool( "TARGET_PDF_HESSIAN" );
  bool RECALC_ALPHA_S     = hzura::glob::parameters->CheckBool( "RECALC_ALPHA_S" );
  hzura::glob::PRINT_LHE_WEIGHTS = hzura::glob::parameters->CheckBool( "PRINT_LHE_WEIGHTS" );
  bool USE_SYS_CONFIGS    = (RUNMODE.find("SYS") != std::string::npos) and not hzura::glob::parameters->CheckBool( "NOSYS" );
  
  string ERA = hzura::glob::parameters->Get( "ERA", "2017" );
  hzura::glob::Init( ERA, nullptr );
  hzura::glob::MADGRAPH = hzura::glob::parameters->CheckBool( "MADGRAPH" );

  hzura::glob::is_data = DATASET_TYPE == "D";
  hzura::ObjectPreselector preselector;
  pm::DEFAULT_VERBOSE_LEVEL = verbose_lvl;
  // preselector.verbose_lvl = pm::verbose::VERBOSE;

  MSG_INFO("hzura::main(): process input file", input_file);
  // create histos
  TFile * file_out = new TFile( output_file.c_str(), "RECREATE" );

  string TARGET_PDF = hzura::glob::parameters->Get( "TARGET_PDF" );
  string GEN_PDF    = hzura::glob::parameters->Get( "GEN_PDF" );
  if( TARGET_PDF.size() and GEN_PDF.size() ){
    string initial_pdf_name = "NNPDF31_nnlo_as_0118_nf_4"; 
    // 320900 - MG 2018, 306000 - PW 2018
    // NNPDF31_nnlo_hessian_pdfas LHAPDF ID: 306000
    // NNPDF31_nnlo_as_0118_nf_4  LHAPDF ID: 320900

    MSG_INFO("LHADPDF:: load PDFs ... ");
    hzura::glob::PDFs_target           = LHAPDF::mkPDFs( TARGET_PDF.c_str()       );
    hzura::glob::PDFs_target_n_replics = atoi( hzura::glob::parameters->Get( "TARGET_PDF_SETSIZE" ).c_str() );
    hzura::glob::PDFs_gen              = LHAPDF::mkPDFs( GEN_PDF.c_str() );

    hzura::glob::PRINT_LHE_WEIGHTS = hzura::glob::parameters->CheckBool( "PRINT_LHE_WEIGHTS" );
  } else {
    RECALC_ALPHA_S = false;
  }

  // -1. set run options ================================================================================================
  // TODO
  map<int, int> selected_efents;
  EventCfg cfg     = EventCfg();
  cfg.name = "Def";
  cfg.USE_GENPARTICLES = true;
  if( DATASET_TYPE != "S" ) cfg.USE_GENPARTICLES = false;
  // https://arxiv.org/pdf/1804.02716.pdf
  cfg.PHOTON_N_MAX = 999;
  cfg.PHOTON_ENERGY_CORRECTION_TYPE = ""; // "ecalEnergyPostCorr";
  cfg.PHOTON_PT_CUT  = 20;
  cfg.PHOTON_ETA_CUT = 2.5;
  cfg.PHOTON_ETA_HOLE_CUT_START = 1.44;
  cfg.PHOTON_ETA_HOLE_CUT_END = 1.57;
  cfg.PHOTON_ID_CUT = id_names::none;                        // id_names::loose id_names::medium id_names::tight
  cfg.PHOTON_SET_SFS = false; // not going to set them for flashgg
  cfg.FLASHGG_DIPHOTON_INDEX = 0;
  // https://arxiv.org/pdf/1812.10529.pdf
  cfg.ELECTRON_N_MAX = 2;
  cfg.ELECTRON_ENERGY_CORRECTION_TYPE = ""; // "ecalTrkEnergyPostCorr"
  cfg.ELECTRON_PT_CUT = 20;
  cfg.ELECTRON_ETA_CUT = 2.5;;
  cfg.ELECTRON_ETA_HOLE_CUT_START = 1.479;
  cfg.ELECTRON_ETA_HOLE_CUT_END = 1.566;
  cfg.ELECTRON_ID_CUT = id_names::loose;      // id_names::loose id_names::medium id_names::tight
  // https://arxiv.org/pdf/1807.06325.pdf
  // https://arxiv.org/pdf/1706.09936.pdf
  cfg.MUON_N_MAX = 2;
  cfg.MUON_PT_CUT    = 20;
  cfg.MUON_ETA_CUT   = 2.4;
  cfg.MUON_ISOID_CUT = id_names::tight;       // id_names::loose id_names::medium id_names::tight
  cfg.JET_LTAG_N_MAX = 3;
  cfg.JET_BTAG_N_MAX = 1;
  cfg.JET_PT_CUT  = 20;
  cfg.JET_ETA_CUT = 3;
  cfg.JET_ID_CUT = id_names::tight;           // id_names::loose id_names::medium id_names::tight
  cfg.JET_BTAGGER = "DeepFlavour";            // "DeepCSV" "DeepFlavour"
  cfg.JET_BTAGGER_ID = id_names::medium;      // id_names::loose id_names::medium id_names::tight #FIXME
  cfg.JET_JER = "central";                    // "central" "up" "down"
  cfg.JET_JEC_TYPE = "";                      // "Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"
  cfg.JET_JEC_DIR  = true;
  cfg.JET_PUJID_CUT  = hzura::id_names::none;
  cfg.MET_SYS      = "";                       // "UnclusteredEnUp" "UnclusteredEnDown" 36384 / 74616 / 140648 && 38580 / 83543 / 157619
  cfg.MET_XYCORR   = true; // true;

  std::vector<hzura::EventCfg> analyses_configs = { cfg };
  if( DATASET_TYPE != "D" and DATASET_TYPE != "BL" ){
    if(USE_SYS_CONFIGS){
      hzura::add_systematic_cfgs_def    ( cfg, analyses_configs );
      hzura::add_systematic_cfgs_flashgg( cfg, analyses_configs );
    }
  }

  std::vector<TH1D*> selections_vec;
  for( int i = 0; i < analyses_configs.size(); i++ ){
    hzura::EventCfg & config = analyses_configs.at( i );
    selections_vec.push_back( new TH1D( (config.name + "_selections").c_str(), (config.name + "_selections").c_str(), 10, 0, 1) );
    config.SetHist( "NPVs", new TH1D( (config.name + "_NPVs").c_str(), (config.name + "_NPVs").c_str(), 100, 0, 100) );
    // config.SetHist( "NPVs_rev", new TH1D( (config.name + "_NPVs_rew").c_str(), (config.name + "_NPVs_rew").c_str(), 100, 0, 100) );
    // config.SetHist( "NPVs_rev_flashgg", new TH1D( (config.name + "_NPVs_rew_flashgg").c_str(), (config.name + "_NPVs_rew_flashgg").c_str(), 100, 0, 100) );

    config.SetHist( "REW_m_yy", new TH1D( (config.name + "_REW_m_yy").c_str(), (config.name + "_REW_m_yy").c_str(), 100, 100, 180) );
  }

  // reweighting ================================================================================================
  double xsec_eft_old = 0;
  string EFT_NODE = hzura::glob::parameters->Get( "EFT_NODE" );
  if( EFT_NODE.size() ) xsec_eft_old = get_eft_xsec(EFT_NODE, "lo", hzura::glob::year_era, true);
  int EFT_NODE_index = -1;
  
  vector<string> out_benchmars = {"sm", "box", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
  vector<string> extra_benchmars = get_benchmarks_names();
  out_benchmars = extra_benchmars;

  ReweightGudrin   r_gudrin = ReweightGudrin("/afs/cern.ch/work/p/pmandrik/dihiggs/8_mvanaluse/VSEVA/HHWWgg/reweight/LO-Ais-13TeV.csv", "/afs/cern.ch/work/p/pmandrik/dihiggs/8_mvanaluse/VSEVA/HHWWgg/reweight/NLO-Ais-13TeV.csv");
  ReweightCarvalho r_carval = ReweightCarvalho("/afs/cern.ch/work/p/pmandrik/dihiggs/8_mvanaluse/VSEVA/HHWWgg/reweight/coefficientsByBin_extended_3M_costHHSim_59-4.txt");

  vector< vector<double> > benchmark_couplings, benchmark_couplings_cms, benchmark_couplings0, benchmark_couplings0_cms;
  vector<double> xsec_eft_new_lo, xsec_eft_new_nlo;
  for(int i = 0; i < out_benchmars.size(); i++){
    if(EFT_NODE == out_benchmars.at(i)) EFT_NODE_index = i;
    benchmark_couplings_cms.push_back ( r_gudrin.GetEFTBenchmark( out_benchmars.at(i), hzura::glob::year_era, true  ) );
    benchmark_couplings.push_back     ( r_gudrin.GetEFTBenchmark( out_benchmars.at(i), hzura::glob::year_era, false ) );
    benchmark_couplings0_cms.push_back( r_carval.GetEFTBenchmark( out_benchmars.at(i), hzura::glob::year_era, true ) );
    benchmark_couplings0.push_back    ( r_carval.GetEFTBenchmark( out_benchmars.at(i), hzura::glob::year_era, false  ) );
    xsec_eft_new_lo.push_back ( get_eft_xsec(out_benchmars.at(i), "lo",  hzura::glob::year_era, false) );
    xsec_eft_new_nlo.push_back( get_eft_xsec(out_benchmars.at(i), "nlo", hzura::glob::year_era, false) );
  }

  // 0. read events ================================================================================================
  ReaderGRINDER * reader = new ReaderGRINDER( input_file );
  hzura::glob::event = reader;
  Long64_t entrys = reader->GetEntries(); // hzura::glob::event->fChain->GetEntriesFast();
  Long64_t entry = 0;
  MSG_INFO("Process lumi weight ... ", reader->eventmetadata->sumWeights);
  MSG_INFO("Process MicroAOD events N = ", reader->n_events_total );

  // setup output variables ================================================================================================
  #include "HHWWgg/HHWWgg_outTreeVars.C"

  for(int i = 0; i < selections_vec.size(); i++){
    selections_vec[i]->Fill("MicroAOD preselections", reader->n_events_total);
  }

  MSG_INFO("hzura::main(): start loop, total number of events", entrys);

  // calc b-tag efficiency if needed
  std::string path, process_name, ext;
  pm::parce_path(input_file, path, process_name, ext);
  msg(input_file, path, process_name, ext );
  if(not hzura::glob::is_data){
    if(RUNMODE.find("BTAG") != std::string::npos) calc_btagging_efficiency( "data/btag/btag_effs_" + hzura::glob::year_era + "_" + process_name + ".root", cfg, preselector.pileup_sf_calculator, 1, cfg.JET_PT_CUT, 2000, 5, -3, 3 ); 
    preselector.btag_eff_reader = BTagEffReader( "data/btag/btag_effs_" + process_name + ".root" );
  }
  // return 0;

  if( RUNMODE.find("SHORT") != std::string::npos ) entrys = TMath::Min( (Long64_t)1000, entrys );
  // if( DATASET_TYPE == "S" ) entrys = 100000;
  event_id = -1;
  for(;entry < entrys; entry++){
    hzura::glob::event->GetEntry(entry);
    event_id++;
    
    if( not (entry % 1000) ){
      pm::msg_progress( float(entry)/entrys );
      msg( float(entry), entrys );
    }

    // 0. remove info from previous event

    // OBJECT SELECTIONS ==============================================
    // 2. apply basic cuts on events
    vector<hzura::HzuraEvent> hzura_events = preselector.PreselectEvents( analyses_configs );
    // preselector.DumpEventsInfo( analyses_configs, hzura_events );
    // cout << "N_muons, N_electrons = " << event->Muons_ << " " << event->Electrons_ << endl;
    // continue;

    for(int i = 0, imax = hzura_events.size(); i < imax; ++i){
      const hzura::HzuraEvent & hevents = hzura_events[i]; 
      const hzura::EventCfg & config = analyses_configs[i];
      TH1D * selections              = selections_vec[i];
      TTree * output_tree            = output_trees[i];
      //msg( "process cfg: ", config.name );

      // MC WEIGHTS
      // double flashgg_event_weight     = glob::event->GetEventWeight() ;
      // double originalXWGTUP   = glob::event->GetEventOriginalXWGTUP(); 
      // double flashgg_puweight = glob::event->GetEventFlashggPuweight() ;
      // double flashgg_nvtx     = glob::event->GetEventFlashggNvtx();
      // double flashgg_npu      = glob::event->GetEventFlashggNpu();
      // msg("event_weight, originalXWGTUP, flashgg_puweight, flashgg_nvtx, flashgg_npu = ", event_weight, originalXWGTUP, flashgg_puweight, flashgg_nvtx, flashgg_npu);
      // msg("hzura::glob::event->GetEventFlashggWeight() = ", hzura::glob::event->GetEventFlashggWeight());
      // msg( "flashgg_mc_weights.size() = ", glob::event->GetEventFlashggMcWeights().size() );
      // for( auto w : glob::event->GetEventFlashggMcWeights() ){
      //   cout << w << endl;
      // }
      // msg( "MC weights.size() = ", glob::event->GetEventWeights().size() );
      // msg( "ps_weights.size() = ", glob::event->GetEventPsWeights().size() );

      // msg( config.name, hevents.genparticles, hevents.photon_candidates, hevents.electron_candidates, hevents.muon_candidates, hevents.ljet_candidates, hevents.bjet_candidates);

      std::vector<hzura::GenParticle> & genparticles = *(hevents.genparticles);
      std::vector<hzura::Photon>      & photons      = *(hevents.photon_candidates);
      std::vector<hzura::Electron>    & electrons    = *(hevents.electron_candidates);
      std::vector<hzura::Muon>        & muons        = *(hevents.muon_candidates);
      std::vector<hzura::Jet>         & ljets        = *(hevents.ljet_candidates);
      std::vector<hzura::Jet>         & bjets        = *(hevents.bjet_candidates);
      const hzura::MET                & met          =   hevents.met;

      // EVENT SELECTIONS ==============================================
      /*
      if(not hzura::glob::is_data){
        config.GetHist("NPVs")->Fill( hzura::glob::event->GetEventTrueMCNumInteractions() );
        Float_t weight = preselector.pileup_sf_calculator.CalcWeight( hzura::glob::event->GetEventTrueMCNumInteractions());
        config.GetHist("NPVs_rev")->Fill( hzura::glob::event->GetEventTrueMCNumInteractions(), weight );
        config.GetHist("NPVs_rev_flashgg")->Fill( hzura::glob::event->GetEventTrueMCNumInteractions(), flashgg_puweight );
      }
      */

      // EVENT SELECTIONS ==============================================
      selections->Fill("flashgg preselections", 1);

      if( photons.size() < 2) continue;
      selections->Fill("At least two photons", 1);

      if( electrons.size() + muons.size() < 1 ) continue;
      selections->Fill("At least one lepton", 1);

      if( (electrons.size() + muons.size()) != 1 ) continue;
      selections->Fill("Exactly one lepton", 1);

      if( bjets.size() ) continue;
      selections->Fill("no b-jets", 1);

      if( ljets.size() < 2 ) continue;
      selections->Fill("At least two light jets", 1);

      //if( ljets.size() == 1 ) continue;
      //selections->Fill("Exactly one light jets", 1);

      // H->yy candidate
      y1_tlv = photons[0].tlv ;
      y2_tlv = photons[1].tlv ;
      H_yy_tlv = y1_tlv + y2_tlv ;

      // M yy in [100, 180]
      if( H_yy_tlv.M() < 100 or H_yy_tlv.M() > 180 ) continue;
      // if( DATASET_TYPE == "D" ) if( H_yy_tlv.M() < 100 and H_yy_tlv.M() > 180 ) continue; // <======= BLIND
      selections->Fill("H_yy_tlv.M() in [100, 180]", 1);

      config.GetHist( "REW_m_yy" )->Fill( H_yy_tlv.M() );

      y1_mva = photons[0].mva_value ;
      y2_mva = photons[1].mva_value ;
      yy_mva = hzura::glob::event->GetDiphotonMva() ;
      dR_yy  = y1_tlv.DeltaR( y2_tlv ) ;

      // Remove info from previos event
      dR_jj = -1; dR_WL = -1;
      dPhi_nuL = -2 * 3.14 - 1; m_yy = -1; y1_Et_div_m_yy = -1; y2_Et_div_m_yy = -1;
      ljet1_tlv = TLorentzVector(); ljet2_tlv = TLorentzVector(); W_jj_tlv = TLorentzVector();
      nu_reco_tlv = TLorentzVector(); W_elnu_tlv = TLorentzVector(); H_WW_tlv = TLorentzVector(); nu_tlv = TLorentzVector(); HH_tlv  = TLorentzVector();
      
      mu_leading_tlv = TLorentzVector(0,0,0,0);
      el_leading_tlv = TLorentzVector(0,0,0,0); 
      lep_leading_tlv = TLorentzVector(0,0,0,0);
      ljet1_btag = -2; ljet2_btag = -2;
      CosTheta_y1_Hyy = 2; CosTheta_j1_Wjj = 2; CosTheta_Hww_W_jj = 2;

      // GENPARTICLES SELECTIONS ==============================================
      //msg( "N genparticles = ", genparticles.size() );
      //for(auto particle : genparticles){
      //  if( abs(particle.pdg_id) != 24 ) continue;
      //  std::cout << particle.pdg_id << " " << particle.status << " " << particle.tlv.Px() << " " << particle.tlv.Pt() << " " << std::endl;
      //}

      if(config.USE_GENPARTICLES){
        TLorentzVector gen_H1f_tlv, gen_H2f_tlv, gen_H1i_tlv, gen_H2i_tlv;
        hzura::GenParticle leading_lepton;
        bool can_set_variables = set_HHWWgg_genparticles(genparticles, gen_H1f_tlv, gen_H2f_tlv, gen_H1i_tlv, gen_H2i_tlv, gen_y1_tlv, gen_y2_tlv, gen_W_jj_tlv, gen_W_elnu_tlv, gen_nu_tlv, gen_ljet1_tlv, gen_ljet2_tlv, leading_lepton );
        gen_HHf_tlv = gen_H1f_tlv + gen_H2f_tlv;
        gen_HHi_tlv = gen_H1i_tlv + gen_H2i_tlv;
        gen_H_yy_tlv = gen_H1f_tlv;
        gen_H_WW_tlv = gen_H2f_tlv;
        gen_Hi_yy_tlv = gen_H1i_tlv;
        gen_Hi_WW_tlv = gen_H2i_tlv;
        gen_lep_leading_tlv = leading_lepton.tlv;

        // cout << "---1" << endl;
        // msg( gen_Hi_yy_tlv.Px(), gen_Hi_yy_tlv.Py(), gen_Hi_yy_tlv.Pz() ); 
        // msg( gen_Hi_WW_tlv.Px(), gen_Hi_WW_tlv.Py(), gen_Hi_WW_tlv.Pz() );
      }

      // EVENT HLV RECONSTRUCTION ==============================================
      N_ljets     = ljets.size();
      N_muons     = muons.size();
      // N_electrons = electrons.size();
      // N_leptons   = electrons.size() + muons.size();

      // W->elnu candidate
      // el
      Int_t leadin_mu_index = get_leading_by_pt( muons );
      Int_t leadin_el_index = get_leading_by_pt( electrons );
      if( leadin_mu_index >= 0 ) mu_leading_tlv = muons[ leadin_mu_index ].tlv;
      if( leadin_el_index >= 0 ) el_leading_tlv = electrons[ leadin_el_index ].tlv;
      
      muon_channel = true;
      lep_leading_tlv = mu_leading_tlv;
      if( el_leading_tlv.Pt() > lep_leading_tlv.Pt() ){
        muon_channel = false;
        lep_leading_tlv = el_leading_tlv;
      }

      // nu
      nu_tlv.SetPtEtaPhiM(met.pt, 0, met.phi, 0.0);

      // W->el nu
      W_elnu_tlv = nu_tlv + lep_leading_tlv;

      // Delta Phi
      dPhi_nuL = nu_tlv.DeltaPhi( lep_leading_tlv );

      // other
      m_yy = H_yy_tlv.M();
      y1_Et_div_m_yy = y1_tlv.Et() / m_yy;
      y2_Et_div_m_yy = y2_tlv.Et() / m_yy;
      CosTheta_y1_Hyy = TMath::Cos( y1_tlv.Angle( H_yy_tlv.Vect() ) );

      // W->jj candidate
      bool HH_reconstructed = false;
      if( ljets.size() > 1 ){
        // W_jj_tlv_leading = ljets[0].tlv + ljets[1].tlv;
        // dR_jj_leading    = ljets[0].tlv.DeltaR( ljets[1].tlv );

        int ljet1_index = 0, ljet2_index = 1;
        ljet1_tlv = ljets[ ljet1_index ].tlv;
        ljet2_tlv = ljets[ ljet2_index ].tlv;
/*
        if( ljets.size() > 2 ){
          auto alt_jet = ljets[2].tlv;
          if( (ljet1_tlv.DeltaR( alt_jet ) < ljet2_tlv.DeltaR( alt_jet )) and (ljet1_tlv.DeltaR( alt_jet ) < ljet1_tlv.DeltaR( ljet2_tlv )) ){
            ljet2_tlv = alt_jet;
            ljet2_index = 2;
          }
          else if( (ljet1_tlv.DeltaR( alt_jet ) > ljet2_tlv.DeltaR( alt_jet )) and (ljet2_tlv.DeltaR( alt_jet ) < ljet2_tlv.DeltaR( ljet1_tlv )) ){
            ljet1_tlv = ljet2_tlv;
            ljet2_tlv =   alt_jet;
            ljet1_index = 1;
            ljet2_index = 2;
          }
        }
*/
        W_jj_tlv = ljet1_tlv + ljet2_tlv;

        ljet1_btag = ljets[ ljet1_index ].btag_DeepFlavour_val ;
        ljet2_btag = ljets[ ljet2_index ].btag_DeepFlavour_val ;

        // H->WW
        HH_reconstructed = hzura::reconstruct_H_from_WW( W_jj_tlv + lep_leading_tlv, nu_tlv, nu_reco_tlv, H_WW_tlv);
        W_elnu_tlv = nu_reco_tlv + lep_leading_tlv; // el_leading_tlv ;

        // HH
        if( HH_reconstructed ) HH_tlv = H_yy_tlv + H_WW_tlv;

        dR_jj  = ljet1_tlv.DeltaR( ljet2_tlv );
        dR_WL  = W_jj_tlv.DeltaR( lep_leading_tlv );
        CosTheta_j1_Wjj = TMath::Cos( ljet1_tlv.Angle( W_jj_tlv.Vect() ) );
      }

      dR_WW  = -1, dR_HH = -1;
      if( HH_reconstructed ) {
        dR_WW  = W_jj_tlv.DeltaR( W_elnu_tlv );
        dR_HH = H_yy_tlv.DeltaR( H_WW_tlv );

        CosTheta_Hww_W_jj = TMath::Cos( W_jj_tlv.Angle( H_WW_tlv.Vect() ) );
      }

      // const vector<Photon> & selected_photons, const vector<Electron> & selected_electrons, const vector<Muon> & selected_muons,
      // const vector<Jet>    & selected_ljets,   const vector<Jet> & selected_bjets,          const PileUpSFReader & pileup_sf_calculator
      std::vector<hzura::Photon>    used_photons   = { photons[0], photons[1] };
      std::vector<hzura::Electron>  used_electrons = {};
      std::vector<hzura::Muon>      used_muons     = {};
      if( muon_channel )  used_muons.push_back( muons[ leadin_mu_index ] );
      else                used_electrons.push_back( electrons[ leadin_el_index ] );
      std::vector<hzura::Jet> & used_ljets = ljets;
      std::vector<hzura::Jet>   used_bjets = {};

      // msg( "get weights ... " );
      if( DATASET_TYPE != "D" ){
        // I.
        // hzura::glob::event->GetEventWeight() ;
        // event.weight          = genEvtInfo->weight();
        // not available in flashgg samples ...
        // II.
        // hzura::glob::event->GetEventFlashggWeight()
        // flashgg_weight - xsection norm to sum of weights
        // because the background is data-driven could work

        mc_raw_weight  = hzura::glob::event->GetEventWeight() ;
        mc_lumi_weight = hzura::glob::event->GetEventFlashggWeight() ;

        mc_weight      = TMath::Sign(1, mc_raw_weight) * mc_lumi_weight;
        // cout << mc_weight << " " << mc_raw_weight << endl;

        EventWeights weights               = hzura::calc_event_weight(used_photons, used_electrons, used_muons, used_ljets, used_bjets, preselector.pileup_sf_calculator, GEN_PDF_HESSIAN, TARGET_PDF.size(), TARGET_PDF_HESSIAN, RECALC_ALPHA_S);
        event_weight                       = weights.combined_weight;
        vector<Weight*>   separate_weights = weights.GetWeights();
        for(int i = 0; i < dummy_weight.combined_weights_names.size(); i++){
          (*(event_alt_weights_up[i]   ))   = weights.combined_weights_up[i];
          (*(event_alt_weights_down[i] ))   = weights.combined_weights_down[i];
          (*(event_alt_weights_central[i])) = separate_weights[i]->c;
          // cout << i << " " << *(event_alt_weights_up[i]) << " " << *(event_alt_weights_down[i]) << endl;
        }
        if( PRINT_WEIGHTS ) weights.Print();
        if( event_weight < 0.001) weights.Print();
      }

      if( DATASET_TYPE == "S" and EFT_NODE_index >= 0){

        for(int i = 0; i < benchmark_couplings.size(); i++){
          (*(weights_lo_cms[i]))   = r_gudrin.GetDiffXsection( gen_HHi_tlv.M(), benchmark_couplings_cms.at(i), "lo" );
          (*(weights_lo0_cms[i]))  = r_carval.GetDiffXsection( gen_HHi_tlv.M(), gen_Hi_yy_tlv.CosTheta(), benchmark_couplings0_cms.at(i) );
        }

        for(int i = 0; i < benchmark_couplings.size(); i++){
          (*(weights_lo[i]))  = r_gudrin.GetDiffXsection( gen_HHi_tlv.M(), benchmark_couplings.at(i), "lo" );
          (*(weights_nlo[i])) = r_gudrin.GetDiffXsection( gen_HHi_tlv.M(), benchmark_couplings.at(i), "nlo" );
          TLorentzVector gen_H = gen_Hi_yy_tlv ;
          gen_H.Boost( - (gen_Hi_yy_tlv + gen_Hi_WW_tlv).BoostVector() );
          (*(weights_lo0[i])) = r_carval.GetDiffXsection( gen_HHi_tlv.M(), gen_H.CosTheta(), benchmark_couplings0.at(i) );
          
          if((*(weights_lo_cms[EFT_NODE_index])) > 0.000001){
            (*(reweight_factor_lo[i]))  = (*(weights_lo[i])) / (*(weights_lo_cms[EFT_NODE_index]))  * xsec_eft_old / xsec_eft_new_lo[i]  ;
            (*(reweight_factor_nlo[i])) = (*(weights_nlo[i])) / (*(weights_lo_cms[EFT_NODE_index])) * xsec_eft_old / xsec_eft_new_nlo[i] ;
            (*(reweight_factor_lo0[i])) = (*(weights_lo0[i])) / (*(weights_lo_cms[EFT_NODE_index])) * xsec_eft_old / xsec_eft_new_lo[i] ;
          } else {
            (*(reweight_factor_lo[i]))  = 0;
            (*(reweight_factor_nlo[i])) = 0;
            (*(reweight_factor_lo0[i])) = 0;
            selections->Fill("Zero reweighting", 1);
          } 
        }
      }

      channel = 0;
      // channels
      // 1 - 0 jet, e
      // 2 - 0 jet, mu
      // 3 - 1 jet, e
      // 4 - 1 jet, mu
      // 5 - 2+ jets, H not reconstructed, e
      // 6 - 2+ jets, H not reconstructed, mu
      // 7 - 2+ jets, H reconstructed, e
      // 8 - 2+ jets, H reconstructed, mu
      if(N_ljets == 0) channel = 1;
      if(N_ljets == 1) channel = 3;
      if(N_ljets > 1 ) channel = 5;
      if( HH_reconstructed ) channel += 2;
      if( N_muons )          channel += 1;
      
      if( H_WW_tlv.M() > 10000 or H_WW_tlv.M() < 0 ){
        msg( "H_WW_tlv M", H_WW_tlv.M() );
        W_jj_tlv.Print();
        lep_leading_tlv.Print();
        nu_tlv.Print();
        nu_reco_tlv.Print();
        H_WW_tlv.Print();
        W_elnu_tlv.Print(); 
      }

      output_tree->Fill();
      
      // cout << " ... " << endl;
      // cout << hzura::glob::event->GetEventRun () << endl;
      // cout << hzura::glob::event->GetEventLumi () << endl;
      // cout << hzura::glob::event->GetEventEvent () << endl;
      // if( selected_efents.find( hzura::glob::event->GetEventEvent () ) != selected_efents.end() ) msg("DUBLICATE !!!");
      // selected_efents[ hzura::glob::event->GetEventEvent () ] = 1;
    }

  }

  msg( " ... done!", input_file, output_file, DATASET_TYPE, RUNMODE );

  for(int i = 0, imax = analyses_configs.size(); i < imax; ++i){
    const hzura::EventCfg & config = analyses_configs[i];
    TTree * output_tree            = output_trees[i];
    msg( "process cfg: ", config.name, "selected/flashgg/MicroAOD = ", output_tree->GetEntriesFast(), "/",  entrys, "/", reader->n_events_total );
  }

  file_out->cd();
  file_out->Write();

  return 0;
}

int main_root_wrapper(string input_file, string output_file, string type, string runmode, string other_parameters="") {
  char * argv [6];
  const char argv_1[] = "main";
  const char * argv_2 = input_file.c_str();
  const char * argv_3 = output_file.c_str();
  const char * argv_4 = type.c_str();
  const char * argv_5 = runmode.c_str();
  const char * argv_6 = other_parameters.c_str();

  argv[0] = (char*)argv_1;
  argv[1] = (char*)argv_2;
  argv[2] = (char*)argv_3;
  argv[3] = (char*)argv_4;
  argv[4] = (char*)argv_5;
  argv[5] = (char*)argv_6;

  int answer = main(5, argv);
  return answer;
};












