
#ifndef hzura_analyse_configuration_helpers_h
#define hzura_analyse_configuration_helpers_h 1

namespace hzura {

  // btag SF readers ===============================================================================================
  // class definition =-
  class BTagSFReader{
    public:
      BTagSFReader(const std::string & working_point_, const std::string & jet_flavour_){
        working_point = working_point_;
        jet_flavour   = jet_flavour_;
        
        if( working_point == "loose"  ) wp = BTagEntry::OP_LOOSE;
        if( working_point == "medium" ) wp = BTagEntry::OP_MEDIUM;
        if( working_point == "tight"  ) wp = BTagEntry::OP_TIGHT;

        if( jet_flavour == "b" ) jf = BTagEntry::FLAV_B;
        if( jet_flavour == "c" ) jf = BTagEntry::FLAV_C;
        if( jet_flavour == "l" ) jf = BTagEntry::FLAV_UDSG;

        reader = BTagCalibrationReader(wp, "central", {"up", "down"});
      }

      void AddFile(const std::string tagger_name, const std::string file){
        // https://twiki.cern.ch/twiki/bin/view/CMS/BTagCalibration#Standalone
        msg("hzura::BTagSFReader.AddFile(): loading ", tagger_name, " from ", file);
        BTagCalibration calib(tagger_name.c_str(), file.c_str());
        msg("hzura::BTagSFReader.AddFile(): ...");
        reader.load(calib, jf, "comb");
        msg("hzura::BTagSFReader.AddFile(): ... ok");
      }

      Float_t GetSF(const std::string type, const Float_t & eta, const Float_t & pt) const {
        return reader.eval_auto_bounds(type, jf, eta, pt);
      }
    
      BTagEntry::OperatingPoint wp;
      BTagEntry::JetFlavor jf;
      std::string working_point, jet_flavour;
      BTagCalibrationReader reader;
  };

  // definition of loaded data =-
  BTagSFReader get_btag_sf_reader(const std::string & working_point, const std::string & jet_flavour){ // TODO 2017 2018
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    BTagSFReader answer(working_point, jet_flavour);
    if(hzura::glob::year_era == "2016")
      answer.AddFile("DeepCSV", "data/jets_sf_2016/DeepCSV_2016LegacySF_WP_V1.csv"); // jets_sf_2016/DeepCSV_2016LegacySF_V1.csv
    // if(hzura::glob::year_era == "2017")
    // if(hzura::glob::year_era == "2018")
    return answer;
  }

  // SF readers ===============================================================================================
  // class definition =-
  class SFFileReader {
    public : 
      SFFileReader(const std::string input_file, double w = 1){
        pmlib::msg( "hzura::SFReader process", input_file );
        weight = w;
        std::unique_ptr<TFile> file( TFile::Open(input_file.c_str()) );

        // loop on all entries of this file
        // https://root.cern.ch/root/html/tutorials/io/copyFiles.C.html
        TKey *key;
        TIter nextkey(file->GetListOfKeys());
        while ((key = (TKey*)nextkey())) {
          const char *classname = key->GetClassName();
          TClass *cl = gROOT->GetClass(classname);
          if (!cl) continue;
          if (!cl->InheritsFrom(TH2::Class())) continue;
      
          // https://root-forum.cern.ch/t/get-histogram-and-close-file/18867
          TH2 * hist = (TH2*) (file->Get( key->GetName() ));
          hist->SetDirectory(0);
          names_x_hists[ key->GetName() ] = hist;

          pmlib::msg( "hzura::SFReader(): add", key->GetName() );
        }
      
        file->Close();
      }

      Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & hist_name) const {
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
          msg_err("hzura::SFReader.GetSF: can't find hist", hist_name, ", available histograms:");
          for(auto it = names_x_hists.begin(); it != names_x_hists.end(); it++)
            msg( it->first );
          return -1;
        }
        TH2* hist = it_hist->second;
        return weight * hist->GetBinContent( hist->FindBin(eta, pt) );
      }

      Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & hist_name) const {
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
          msg_err("hzura::SFReader.GetErr: can't find hist", hist_name, ", available histograms:");
          for(auto it = names_x_hists.begin(); it != names_x_hists.end(); it++)
            msg( it->first );
          return -1;
        }
        TH2* hist = it_hist->second;
        return weight * hist->GetBinError( hist->FindBin(eta, pt) );
      }

      double weight;
      std::map<std::string, TH2*> names_x_hists;
  };

  class SFCalculator {
    public:
    void AddReader(std::string name, SFFileReader* reader){
      sf_readers[ name ] = reader;
      fname_loose  = "l";
      fname_medium = "m";
      fname_tight  = "t";
    }

    Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name) const {
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        return -1;
      }
      return it_reader->second->GetSF(eta, pt, hist_name);
    }

    Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name) const {
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        return -1;
      }
      return it_reader->second->GetErr(eta, pt, hist_name);
    }

    Float_t GetSF_loose(const Float_t & eta, const Float_t & pt, const int & unc){
      return GetSF(eta, pt, fname_loose, hname_loose) + unc * GetErr(eta, pt, fname_loose, hname_loose);
    }

    Float_t GetSF_medium(const Float_t & eta, const Float_t & pt, const int & unc){
      return GetSF(eta, pt, fname_medium, hname_medium) + unc * GetErr(eta, pt, fname_medium, hname_medium);
    }

    Float_t GetSF_tight(const Float_t & eta, const Float_t & pt, const int & unc){
      return GetSF(eta, pt, fname_tight, hname_tight) + unc * GetErr(eta, pt, fname_tight, hname_tight);
    }

    std::string fname_loose, fname_medium, fname_tight;
    std::string hname_loose, hname_medium, hname_tight;

    std::map<std::string, SFFileReader*> sf_readers;
    std::function<Float_t (const Float_t &, const Float_t &, const int &, const std::string &)> GetSF_iso, GetSF_id;
    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_id_loose, GetSF_id_medium, GetSF_id_tight;
    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_iso_loose, GetSF_iso_medium, GetSF_iso_tight;
  };

  // definition of loaded data =-
  // photons SFs .., TODO 2017 2018
  SFCalculator get_photons_sf_reader(){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Photon_efficiencies_and_scale_fa
    SFCalculator answer; 

    if(hzura::glob::year_era == "2016"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/photons_sf_2016/Fall17V2_2016_Loose_photons.root" );
      SFFileReader *sf_m  = new SFFileReader( "data/photons_sf_2016/Fall17V2_2016_Medium_photons.root" );
      SFFileReader *sf_t  = new SFFileReader( "data/photons_sf_2016/Fall17V2_2016_Tight_photons.root" );

      answer.AddReader( "l",  sf_l );
      answer.AddReader( "m",  sf_m );
      answer.AddReader( "t",  sf_t );

      answer.hname_loose  = "EGamma_SF2D";
      answer.hname_medium = "EGamma_SF2D";
      answer.hname_tight  = "EGamma_SF2D";
    }

    return answer;
  }

  // electrons SFs .., TODO 2017 2018
  SFCalculator get_electrons_sf_reader(){
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale
    SFCalculator answer; 

    if(hzura::glob::year_era == "2016"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronLoose_Fall17V2.root" );
      SFFileReader *sf_m  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronMedium_Fall17V2.root" );
      SFFileReader *sf_t  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronTight_Fall17V2.root" );

      answer.AddReader( "l",  sf_l );
      answer.AddReader( "m",  sf_m );
      answer.AddReader( "t",  sf_t );

      answer.hname_loose  = "EGamma_SF2D";
      answer.hname_medium = "EGamma_SF2D";
      answer.hname_tight  = "EGamma_SF2D";
    }
    
    return answer;
  }

  // muons SFs ... TODO 2017 2018
  SFCalculator get_muons_sf_reader(){
    SFCalculator answer; 

    if(hzura::glob::year_era == "2016"){
      double Lumi_BCDEF = 1.0;
      double Lumi_GH    = 1.0; // FIXME

      double w1 = Lumi_BCDEF / (Lumi_BCDEF + Lumi_GH);
      double w2 = Lumi_GH    / (Lumi_BCDEF + Lumi_GH);
  
      SFFileReader *sf_id_BCDEF  = new SFFileReader("data/muons_sf_2016/RunBCDEF_SYS_ID.root",  w1);
      SFFileReader *sf_iso_BCDEF = new SFFileReader("data/muons_sf_2016/RunBCDEF_SYS_ISO.root", w1);

      SFFileReader *sf_id_GH  = new SFFileReader("data/muons_sf_2016/RunGH_SYS_ID.root",  w2);
      SFFileReader *sf_iso_GH = new SFFileReader("data/muons_sf_2016/RunGH_SYS_ISO.root", w2);

      answer.AddReader( "id_bcdef",  sf_id_BCDEF );
      answer.AddReader( "iso_bcdef", sf_iso_BCDEF );
      answer.AddReader( "id_gh",     sf_id_GH );
      answer.AddReader( "iso_gh",    sf_iso_GH );

      answer.GetSF_id = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id_bcdef", hname) + answer.GetSF(eta, pt, "id_gh", hname) + unc * ( answer.GetErr(eta, pt, "id_bcdef", hname) + answer.GetErr(eta, pt, "id_gh", hname) ); };
      answer.GetSF_iso  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "iso_bcdef", hname) + answer.GetSF(eta, pt, "iso_gh", hname) + unc * ( answer.GetErr(eta, pt, "iso_bcdef", hname) + answer.GetErr(eta, pt, "iso_gh", hname) ); };

      answer.GetSF_id_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(eta, pt, unc, "NUM_LooseID_DEN_genTracks_eta_pt");  };
      answer.GetSF_id_medium = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt"); };
      answer.GetSF_id_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(eta, pt, unc, "NUM_TightID_DEN_genTracks_eta_pt");  }; 

      /*
      hzura::SFReader add NUM_LooseRelIso_DEN_LooseID_eta_pt
      hzura::SFReader add NUM_LooseRelIso_DEN_MediumID_eta_pt
      hzura::SFReader add NUM_LooseRelIso_DEN_TightIDandIPCut_eta_pt
      hzura::SFReader add NUM_TightRelIso_DEN_MediumID_eta_pt
      hzura::SFReader add NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt
      hzura::SFReader add NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt
      */
      answer.GetSF_iso_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(eta, pt, unc, "NUM_LooseRelIso_DEN_LooseID_eta_pt"); };
      // answer.GetSF_iso_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_iso_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(eta, pt, unc, "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt"); }; 
    }
  
    return answer;
  }

  // PileUp reweighting reader ===============================================================================================
  // class definition =-
  class PileUpSFReader{
    public:
    bool Init(const std::string input_file){
      TFile *fIn=TFile::Open( input_file.c_str() );
      if(not fIn){
        msg_err("hzura::PileUpSFReader.Init: can't open file", input_file);
        return false;
      }

      puWgtGr     = (TGraph *)fIn->Get("puwgts_nom");
      puWgtDoGr   = (TGraph *)fIn->Get("puwgts_down");
      puWgtUpGr   = (TGraph *)fIn->Get("puwgts_up");
      fIn->Close();

      /*
      puWgtGr->Print();
      for(int i = 0, n = puWgtGr->GetN(); i < n; i++){
        Double_t x, y;
        puWgtGr->GetPoint( i, x, y );
        msg( i, x, y );
      }
      */

      Double_t x, y;
      puWgtGr->GetPoint( puWgtGr->GetN()-1, x, y );
      max_range_x = x;
      
      return true;
    }

    void CalcSFs(const Int_t npv, Float_t & sf, Float_t & sf_up, Float_t & sf_down) const {
      sf      = 0;
      sf_up   = 0;
      sf_down = 0;

      if( npv > max_range_x ){
        msg_err("hzura::PileUpSFReader.CalcSFs: number of MC PVs", npv, "is greater than range of reweighting histogram,", max_range_x);
        return;
      }

      sf      = puWgtGr  ->Eval( npv );
      sf_up   = puWgtUpGr->Eval( npv );
      sf_down = puWgtDoGr->Eval( npv );
    }

    Double_t max_range_x;
    TGraph *puWgtGr, *puWgtDoGr, *puWgtUpGr;
  };

  //  definition of loaded data =-
  PileUpSFReader get_pileup_sf_reader(){
    PileUpSFReader answer;
    if(hzura::glob::year_era == "2016") answer.Init("data/pileup/puweights_2016.root");
    if(hzura::glob::year_era == "2017") answer.Init("data/pileup/puweights_2017.root");
    if(hzura::glob::year_era == "2018") answer.Init("data/pileup/puweights_2018.root");
    
    return answer;
  }

  // Definition of JEC uncertanties ==--
  class JecReader {
    public:
    std::map<std::string, JetCorrectionUncertainty*> uncertanties;

    void RemakeJets(vector<hzura::Jet> & jet_candidates, const std::string & unc_name, bool up=true){
      auto it = uncertanties.find( unc_name );
      if(it == uncertanties.end()){
        msg_err("hzura::JecReader.RemakeJets: can't find JEC uncertantie with name:", unc_name, ", available uncertanties are:");
        for(it = uncertanties.begin(); it != uncertanties.end(); ++it){
          msg_err(" ", it->first);
        }
        return;
      }

      // https://twiki.cern.ch/twiki/bin/view/CMS/JECUncertaintySources
      JetCorrectionUncertainty *unc = it->second;
      if( up ){
        for(hzura::Jet & jet : jet_candidates){
          unc->setJetPt( jet.tlv.Pt() );
          unc->setJetEta( jet.tlv.Eta() );
          jet.tlv *= (1. + unc->getUncertainty( up )); 
        }
      } else { 
        for(hzura::Jet & jet : jet_candidates){
          unc->setJetPt( jet.tlv.Pt() );
          unc->setJetEta( jet.tlv.Eta() );
          jet.tlv *= (1. - unc->getUncertainty( up )); 
        }
      }
      
    }
  };

  JecReader get_jec_uncertanties(){ // FIXME 2016 2017 2018
    // https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC
    std::vector<std::string> unc_names;
    std::string unc_inp_file;
    if(hzura::glob::year_era == "2016") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "data/JEC/Summer16_07Aug2017_V11_MC/Summer16_07Aug2017_V11_MC_UncertaintySources_AK4PF.txt";
    }
    if(hzura::glob::year_era == "2017") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "";
    }
    if(hzura::glob::year_era == "2018") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "";
    }

    msg("hzura::get_jec_uncertanties() ... ", unc_inp_file);
    JecReader answer;
    for(const std::string & unc_name : unc_names) {
      JetCorrectorParameters     *p = new JetCorrectorParameters(unc_inp_file, unc_name);
      JetCorrectionUncertainty *unc = new JetCorrectionUncertainty(*p);

      answer.uncertanties[ unc_name ] = unc;
    }

    msg("hzura::get_jec_uncertanties() ... ok");
    return answer;
  };

};

#endif






