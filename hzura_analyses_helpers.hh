
#ifndef hzura_analyses_helpers_h
#define hzura_analyses_helpers_h 1

// extra libraries
#include "pmlib_msg.hh"
using namespace pmlib;

namespace hzura {

  // BASIC SELECTORS ===============================================================================================
  

  // COMBINATION OF PARTICLES ===============================================================================================
  // p1 + p2 => p
  template <typename A, typename B>
  void make_combination(const A & p1, const B & p2, Particle & p){
    p.tlv = p1.tlv + p2.tlv;
    p.mothers.push_back( (Particle*) (& p1) );
    p.mothers.push_back( (Particle*) (& p2) );
  }

  // p1 p2 p3 p4 p5 ...  -> p1+p2 p1+p3 ... p2+p3 p2+p4 ...
  template <typename A>
  void make_combinations(const vector<A> & inp,  vector<Particle> & out){
    int size = inp.size();
    Particle p;
    for( int i = 0; i < size; i++ ){
      for( int j = i+1; j < size; j++ ){
        make_combination( inp[i], inp[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A>
  void make_combinations_filtered( const vector<A> & inp, vector<Particle> & out, bool (*filter)(Particle *, Particle *) ){
    int size = inp.size();
    Particle p;
    for( int i = 0; i < size; i++ ){
      for( int j = i+1; j < size; j++ ){
        if( (*filter)( (Particle*) & inp[i], (Particle*) & inp[j] ) ) continue;
        make_combination( inp[i], inp[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A> void make_sameCharge_combinations( const vector<A> & inp, vector<Particle> & out ){
    make_combinations_filtered(inp, out, [](Particle * a, Particle * b) { return ((int)a->Charge() != (int)b->Charge()); } );
  }

  template <typename A> void make_diffCharge_combinations( const vector<A> & inp, vector<Particle> & out ){
    make_combinations_filtered(inp, out, [](Particle * a, Particle * b) { return ((int)a->Charge() == (int)b->Charge()); } );
  }

  // (p11 p12 p13 p14 p15 ... ) (p21 p22 p23 p24 p25 ... ) -> p11+p21 p11+p22 ... p12+p21 p12+p21 ...
  template <typename A, typename B>
  void make_combinations(const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out){
    Particle p;
    for( int i = 0, size_1 = inp_1.size(); i < size_1; i++ ){
      for( int j = 0, size_2 = inp_2.size(); j < size_2; j++ ){
        make_combination( inp_1[i], inp_2[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A, typename B>
  void make_combinations_filtered(const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out, bool (*filter)(Particle *, Particle *)  ){
    Particle p;
    for( int i = 0, size_1 = inp_1.size(); i < size_1; i++ ){
      for( int j = 0, size_2 = inp_2.size(); j < size_2; j++ ){
        if( (*filter)( (Particle*) & inp_1[i], (Particle*) & inp_2[j] ) ) continue;
        make_combination( inp_1[i], inp_2[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A, typename B> void make_sameCharge_combinations( const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out ){
    make_combinations_filtered(inp_1, inp_2, out, [](Particle * a, Particle * b) { return (a->Charge() != b->Charge()); } );
  }

  template <typename A, typename B> void make_diffCharge_combinations( const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out ){
    make_combinations_filtered(inp_1, inp_2, out, [](Particle * a, Particle * b) { return (a->Charge() == b->Charge()); } );
  }

  // SF readers ===============================================================================================
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

};

#endif






