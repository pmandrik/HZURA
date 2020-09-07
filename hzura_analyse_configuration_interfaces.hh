
#ifndef hzura_analyse_configuration_interfaces_h
#define hzura_analyse_configuration_interfaces_h

namespace hzura {

  // btag Efficiency readers ===============================================================================================
  class BTagEffReader{
    public:
      BTagEffReader(){};
      BTagEffReader(std::string input_file){
        pm::msg( "hzura::BTagEffReader(): process", input_file );
        std::unique_ptr<TFile> file( TFile::Open(input_file.c_str()) );

        // loop on all entries of this file
        // https://root.cern.ch/root/html/tutorials/io/copyFiles.C.html
        for(int i = 0; i < 6; i++){
          TH2D * hist = (TH2D*) (file->Get( ("eff_" + to_string(i)).c_str() ));
          hist->SetDirectory(0);
          pm::msg( "hzura::SFReader(): add", "eff_" + to_string(i) );

          effs.push_back( hist );
        }
      
        file->Close();
      }

    double GetEff(const double & pt, const double & eta, const int & jf){
      TH2D* eff = effs.at( jf );
      // msg( "GetEff", pt, eta, "=>", eff->FindBin( pt, eta ), "=>", eff->GetBinContent( eff->FindBin( pt, eta) ) );
      return eff->GetBinContent( eff->FindBin( pt, eta) );
    }

    bool Valid(){
      return effs.size();
    }

    vector<TH2D*> effs;
  };

  // btag SF readers ===============================================================================================
  // Interface to read SF related to b-tagging
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
  class BTagSFReader{
    public:
      BTagSFReader(){};
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
        calib = BTagCalibration(tagger_name.c_str(), file.c_str());
        msg("hzura::BTagSFReader.AddFile(): ...");
        reader.load(calib, jf, "comb");
        msg("hzura::BTagSFReader.AddFile(): ... ok");
      }

      Float_t GetSF(const std::string type, const Float_t & eta, const Float_t & pt) const {
        // msg( "hzura::BTagSFReader.GetSF(", type, eta, pt, ") ... "  );
        // msg( reader.eval_auto_bounds(type, jf, eta, pt), working_point, jet_flavour );
        return reader.eval_auto_bounds(type, jf, eta, pt);
      }
    
      BTagEntry::OperatingPoint wp;
      BTagEntry::JetFlavor jf;
      std::string working_point, jet_flavour;

      BTagCalibration calib;
      BTagCalibrationReader reader;
  };

  // SF readers ===============================================================================================
  // Interface to read SF related to electrons, muons and photons
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Electron_efficiencies_and_scale
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2#Photon_efficiencies_and_scale_fa
  class SFFileReader {
    public : 
      SFFileReader(const std::string input_file, double w = 1, bool msg_if_outside_range_ = true, float value_if_outside_range_ = 0){
        pm::msg( "hzura::SFReader process", input_file );
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

          pm::msg( "hzura::SFReader(): add", key->GetName() );
        }
      
        file->Close();

        msg_if_outside_range = msg_if_outside_range_;
        value_if_outside_range = value_if_outside_range_;
      }

      Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & hist_name) const {
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
          msg_err("hzura::SFReader.GetSF: can't find hist", "\"" + hist_name + "\",", "available histograms:");
          for(auto it = names_x_hists.begin(); it != names_x_hists.end(); it++)
            msg( it->first );
          return -1;
        }
        TH2* hist = it_hist->second;
        if( eta < hist->GetXaxis()->GetXmin() or eta > hist->GetXaxis()->GetXmax() ){
          if(msg_if_outside_range) msg_err("hzura::SFReader.GetSF: point outside range ", "\"" + hist_name + "\",", eta, "[" , hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), "]");
          return value_if_outside_range;
        }
        if( pt < hist->GetYaxis()->GetXmin() or pt > hist->GetYaxis()->GetXmax() ){
          if(msg_if_outside_range) if(msg_if_outside_range)msg_err("hzura::SFReader.GetSF: point outside range ", "\"" + hist_name + "\",", pt, "[" , hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax(), "]");
          return value_if_outside_range;
        }
        return weight * hist->GetBinContent( hist->FindBin(eta, pt) );
      }

      Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & hist_name) const {
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
          msg_err("hzura::SFReader.GetErr: can't find hist", "\"" + hist_name + "\",", "available histograms:");
          for(auto it = names_x_hists.begin(); it != names_x_hists.end(); it++)
            msg( it->first );
          return -1;
        }
        TH2* hist = it_hist->second;
        // FIXME outside the area ???
        return weight * hist->GetBinError( hist->FindBin(eta, pt) );
      }

      bool msg_if_outside_range;
      Float_t value_if_outside_range;
      Float_t weight;
      std::map<std::string, TH2*> names_x_hists;
  };

  // Interface to calculate SF related to electrons, muons and photons
  // using different files wrapped by SFFileReader
  class SFCalculator : public pm::PmMsg {
    public:
    SFCalculator(){}

    void AddReader(std::string name, SFFileReader* reader){
      MSG_DEBUG("hzura::SFReader process ... ", name );

      sf_readers[ name ] = reader;
      fname_loose  = "l";
      fname_medium = "m";
      fname_tight  = "t";
    }

    Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name) const {
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        msg_err("hzura::SFCalculator.GetSF: can't find SFFileReader", "\"" + reader_name + "\",", "available SFFileReader:");
        for(auto it = sf_readers.begin(); it != sf_readers.end(); it++) msg( it->first );
        return -999;
      }
      return it_reader->second->GetSF(eta, pt, hist_name);
    }

    Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name) const {
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        msg_err("hzura::SFCalculator.GetErr: can't find SFFileReader", "\"" + reader_name + "\",", "available SFFileReader:");
        for(auto it = sf_readers.begin(); it != sf_readers.end(); it++) msg( it->first );
        return -999;
      }
      return it_reader->second->GetErr(eta, pt, hist_name);
    }

    Float_t GetSF_loose(const Float_t & eta, const Float_t & pt, const int & unc) const {
      return GetSF(eta, pt, fname_loose, hname_loose) + unc * GetErr(eta, pt, fname_loose, hname_loose);
    }

    Float_t GetSF_medium(const Float_t & eta, const Float_t & pt, const int & unc) const {
      return GetSF(eta, pt, fname_medium, hname_medium) + unc * GetErr(eta, pt, fname_medium, hname_medium);
    }

    Float_t GetSF_tight(const Float_t & eta, const Float_t & pt, const int & unc) const {
      return GetSF(eta, pt, fname_tight, hname_tight) + unc * GetErr(eta, pt, fname_tight, hname_tight);
    }

    std::string fname_loose, fname_medium, fname_tight;
    std::string hname_loose, hname_medium, hname_tight;

    std::map<std::string, SFFileReader*> sf_readers;
    std::function<Float_t (const Float_t &, const Float_t &, const int &, const std::string &)> GetSF_iso, GetSF_id;
    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_id_loose, GetSF_id_medium, GetSF_id_tight;
    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_iso_loose, GetSF_iso_medium, GetSF_iso_tight;
  };

  // PUJID SF & WP ===============================================================================================
  // https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Working_points
  class PUJIDReader : public SFCalculator{
    public:
    PUJIDReader(){}

    bool GetID(const double PUJID, const double & pt, double eta, const int & wp) const {
      eta = TMath::Abs( eta );

      int pt_index = 0;
      for( ; pt_index < pt_range.size(); pt_index++){
        if( pt > pt_range[ pt_index ] ) continue;
        break;
      }

      int eta_index = 0;
      for( ; eta_index < eta_range.size(); eta_index++){
        if( eta > eta_range[ eta_index ] ) continue;
        break;
      }

      if( pt_index >= pt_range.size() or eta_index >= eta_range.size() ) return true;
      if( wp == hzura::id_names::loose )  return (PUJID > loose[pt_index][eta_index]);
      if( wp == hzura::id_names::medium ) return (PUJID > medium[pt_index][eta_index]);
      if( wp == hzura::id_names::tight )  return (PUJID > tight[pt_index][eta_index]);
      return true;
    }

    Float_t GetSF_alt(const Float_t & eta, const Float_t & pt, const int & unc, std::string file_name, std::string hist_name, std::string file_err_name, std::string hist_err_name) const {
      return GetSF(eta, pt, file_name, hist_name) + unc * GetSF(eta, pt, file_err_name, hist_err_name);
    }

    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_eff_loose,    GetSF_eff_medium,    GetSF_eff_tight;
    std::function<Float_t (const Float_t &, const Float_t &, const int &)> GetSF_mistag_loose, GetSF_mistag_medium, GetSF_mistag_tight;

    std::vector< std::vector<double> > loose, medium, tight;
    std::vector<double> eta_range, pt_range;
  };

  // PileUp reweighting reader ===============================================================================================
  // Interface to read SF related to Pile-Up
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
      max_range_x = x + 0.5; //FIXME
      puWgtGr->GetPoint( 0, x, y );
      min_range_x = x - 0.5; //FIXME
      return true;
    }

    Float_t CalcWeight(const Int_t npv) const {
      if( npv > max_range_x ){
        msg_err("hzura::PileUpSFReader.CalcSFs: number of MC PVs", npv, "is greater than range of reweighting histogram,", max_range_x);
        return 0;
      }
      if( npv < min_range_x ){
        msg_err("hzura::PileUpSFReader.CalcSFs: number of MC PVs", npv, "is smaller than range of reweighting histogram,", min_range_x);
        return 0;
      }

      Float_t sf      = puWgtGr  ->Eval( npv );

      if(sf < 0){
        msg_err("hzura::PileUpSFReader.CalcSFs: negative SF", sf, "for NPVs = ", npv);
        return 0;
      }
      return sf;
    }

    void CalcSFs(const Int_t npv, Float_t & sf, Float_t & sf_up, Float_t & sf_down) const {
      sf      = 0;
      sf_up   = 0;
      sf_down = 0;

      if( npv > max_range_x ){
        msg_err("hzura::PileUpSFReader.CalcSFs: number of MC PVs", npv, "is greater than range of reweighting histogram,", max_range_x);
        return;
      }
      if( npv < min_range_x ){
        msg_err("hzura::PileUpSFReader.CalcSFs: number of MC PVs", npv, "is smaller than range of reweighting histogram,", min_range_x);
        return;
      }

      sf      = puWgtGr  ->Eval( npv );
      sf_up   = puWgtUpGr->Eval( npv );
      sf_down = puWgtDoGr->Eval( npv );

      if(sf < 0){
        msg_err("hzura::PileUpSFReader.CalcSFs: negative SF", sf, "for NPVs = ", npv);
        sf = 0;
        sf_up = 0;
        sf_down = 0;
        return;
      }
    }

    Double_t max_range_x, min_range_x;
    TGraph *puWgtGr, *puWgtDoGr, *puWgtUpGr;
  };

  // Definition of JEC uncertanties  ===============================================================================================
  // Interface to read uncertanties related to JEC
  // https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC
  class JecReader {
    public:
    std::map<std::string, JetCorrectionUncertainty*> uncertanties;
    JetCorrectionUncertainty * active_unc;

    JetCorrectionUncertainty * GetJetCorrectionUncertainty(const std::string & unc_name){
      auto it = uncertanties.find( unc_name );
      if(it == uncertanties.end()){
        msg_err("hzura::JecReader.GetJetCorrectionUncertainty: can't find JEC uncertantie with name:", unc_name, ", available uncertanties are:");
        for(it = uncertanties.begin(); it != uncertanties.end(); ++it){
          msg_err(" ", it->first);
        }
        return nullptr;
      }

      // https://twiki.cern.ch/twiki/bin/view/CMS/JECUncertaintySources
      return it->second;
    }

    void SetActiveJetCorrectionUncertainty( const std::string & unc_name ){
      if( not unc_name.size() ){
        active_unc = nullptr;
        return;
      }
      active_unc = GetJetCorrectionUncertainty( unc_name );
    }

    void RemakeJet(hzura::Jet & jet, bool up=true){
      if(not active_unc) return;
      if( TMath::Abs(jet.tlv.Eta()) > 5.1 ){
        msg_err( "hzura::JecReader.GetJetCorrectionUncertainty.RemakeJet(): jet p4 outside JEC Tool range", jet.tlv.Pt(), jet.tlv.Eta() );
        return;
      }
      if( up ){
        active_unc->setJetPt( jet.tlv.Pt() );
        active_unc->setJetEta( jet.tlv.Eta() );
        jet.tlv *= (1. + active_unc->getUncertainty( up )); 
      } else { 
        active_unc->setJetPt( jet.tlv.Pt() );
        active_unc->setJetEta( jet.tlv.Eta() );
        jet.tlv *= (1. - active_unc->getUncertainty( up )); 
      }
    }

    void RemakeJets(vector<hzura::Jet> & jet_candidates, const std::string & unc_name, bool up=true){
      JetCorrectionUncertainty *unc = GetJetCorrectionUncertainty( unc_name );
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






};

#endif
