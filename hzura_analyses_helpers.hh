
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
      SFFileReader(const std::string input_file, double & w = 1){
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
          names_x_hists[ hist->GetName() ] = hist;

          pmlib::msg( "hzura::SFReader add", hist->GetName() );
        }
      
        file->Close();
      }

      Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & hist_name){
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
          return -1;
        }
        TH2* hist = it_hist->second;
        return weight * hist->GetBinContent( hist->FindBin(eta, pt) );
      }

      Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & hist_name){
        auto it_hist = names_x_hists.find( hist_name );
        if( it_hist == names_x_hists.end() ){
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
    }

    Float_t GetSF(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name){
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        return -1;
      }
      return it_reader->second->GetSF(eta, pt, reader_name);
    }

    Float_t GetErr(const Float_t & eta, const Float_t & pt, const std::string & reader_name, const std::string & hist_name){
      auto it_reader = sf_readers.find(reader_name);
      if( it_reader == sf_readers.end() ){
        return -1;
      }
      return it_reader->second->GetErr(eta, pt, reader_name);
    }

    std::map<std::string, SFFileReader*> sf_readers;
  };

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

      answer.GetSF_iso = [](const Float_t & eta, const Float_t & pt, const int & unc, const std::string hname){ return GetSF(eta, pt, "id_bcdef", hname) + GetSF(eta, pt, "id_gh", hname) + unc * ( GetErr(eta, pt, "id_bcdef", hname) + GetErr(eta, pt, "id_gh", hname) ); };
      answer.GetSF_id  = [](const Float_t & eta, const Float_t & pt, const int & unc, const std::string hname){ return GetSF(eta, pt, "iso_bcdef", hname) + GetSF(eta, pt, "iso_gh", hname) + unc * ( GetErr(eta, pt, "iso_bcdef", hname) + GetErr(eta, pt, "iso_gh", hname) ); };

      answer.GetSF_id_loose  = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_id(eta, pt, unc, "NUM_LooseID_DEN_genTracks_eta_pt") };
      answer.GetSF_id_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_id(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_id_tight  = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_id(eta, pt, unc, "NUM_TightID_DEN_genTracks_eta_pt") }; 

      /*
      hzura::SFReader add NUM_LooseRelIso_DEN_LooseID_eta_pt
      hzura::SFReader add NUM_LooseRelIso_DEN_MediumID_eta_pt
      hzura::SFReader add NUM_LooseRelIso_DEN_TightIDandIPCut_eta_pt
      hzura::SFReader add NUM_TightRelIso_DEN_MediumID_eta_pt
      hzura::SFReader add NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt
      hzura::SFReader add NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt
      */
      answer.GetSF_iso_loose  = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_LooseRelIso_DEN_LooseID_eta_pt") };
      // answer.GetSF_iso_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_iso_tight  = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt") }; 
    }
  
    return answer;
  }

};

#endif






