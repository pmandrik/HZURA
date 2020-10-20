
#ifndef hzura_cfg_h
#define hzura_cfg_h 1

namespace hzura {

  enum id_names {
    none = -1,
    loose = 0,
    medium,
    tight,
  };

  class RunParameters {
    public:
    RunParameters( string raw_values ){
      std::vector<std::string> param_pairs;
      pm::split_string( raw_values, param_pairs, "," );

      for(int i = 0; i < param_pairs.size(); i++){
        std::vector<std::string> opts;
        pm::split_string( param_pairs.at(i), opts, "=");
        if( opts.size() != 2){
          msg_err("hzura::RunParameters(): Can't parse option", param_pairs.at(i));
          continue;
        }
        values[ opts[0] ] = opts[1];
        msg_err("hzura::RunParameters(): set option", opts[0], "=", opts[1]);
      }
    }

    string Get(string pname){
      map<string, string>::iterator it = values.find( pname );
      if( it == values.end() ) return "";
      return it->second;
    }
    
    map<string, string> values;
  };

  namespace glob {
  
    bool is_data;
    std::string year_era;
    int year;
    // Events * event;
    ReaderGRINDER * event;
    TRandom3 randGen;
    RunParameters * parameters;

    vector<LHAPDF::PDF*> PDFs_gen, PDFs_target;
    int PDFs_target_n_replics;

    void Init(const string & era, ReaderGRINDER * event){

      hzura::glob::year_era = era;
      hzura::glob::event = event;

      if( year_era == "2016" ) year = 2016;
      if( year_era == "2017" ) year = 2017;
      if( year_era == "2018" ) year = 2018;
      is_data = true;
    }

  };

  class EventCfg {
    public:
    std::string name;

    // GENPARTICLES OPTIONS =- -= =- -= =- -= =- -=
    bool USE_GENPARTICLES;
    std::vector<int> GENPARTICLES_SELECT_STATUS_CODES;
    std::vector<int> GENPARTICLES_SELECT_IDS;

    // PHOTONS OPTIONS =- -= =- -= =- -= =- -=
    int PHOTON_N_MAX;
    std::string PHOTON_ENERGY_CORRECTION_TYPE; // "ecalEnergyPreCorr" "ecalEnergyPostCorr" "energyScaleUp" "energyScaleDown" "energyScaleStatUp" "energyScaleStatDown" "energyScaleSystUp"
                                               // "energyScaleSystDown" "energyScaleGainUp" "energyScaleGainDown" "energyScaleEtUp" "energyScaleEtDown" 
                                               // "energySigmaUp" "energySigmaDown" "energySigmaPhiUp" "energySigmaPhiDown" "energySigmaRhoUp" "energySigmaRhoDown"
    Float_t PHOTON_PT_CUT;
    Float_t PHOTON_ETA_CUT;
    Float_t PHOTON_ETA_HOLE_CUT_START;
    Float_t PHOTON_ETA_HOLE_CUT_END;
    int PHOTON_ID_CUT;                         // id_names::loose id_names::medium id_names::tight
    bool PHOTON_SET_SFS;
    // ELECTRONS OPTIONS =- -= =- -= =- -= =- -=
    int ELECTRON_N_MAX;
    std::string ELECTRON_ENERGY_CORRECTION_TYPE; // same as PHOTON_ENERGY_CORRECTION_TYPE
    Float_t ELECTRON_PT_CUT;
    Float_t ELECTRON_ETA_CUT;
    Float_t ELECTRON_ETA_HOLE_CUT_START;
    Float_t ELECTRON_ETA_HOLE_CUT_END;
    int ELECTRON_ID_CUT;                       // id_names::loose id_names::medium id_names::tight
    // MUONS OPTIONS =- -= =- -= =- -= =- -=
    int MUON_N_MAX;
    Float_t MUON_PT_CUT;
    Float_t MUON_ETA_CUT;
    int MUON_ISOID_CUT;                        // id_names::loose id_names::medium id_names::tight
    // JETS OPTIONS =- -= =- -= =- -= =- -=
    int JET_LTAG_N_MAX;
    int JET_BTAG_N_MAX;
    Float_t JET_PT_CUT;
    Float_t JET_ETA_CUT;
    int JET_PUJID_CUT;                         // id_names::none id_names::loose id_names::medium id_names::tight
    int JET_ID_CUT;                            // id_names::loose id_names::medium id_names::tight
    std::string JET_BTAGGER;                   // "DeepCSV"
    int JET_BTAGGER_ID;                        // id_names::loose id_names::medium id_names::tight
    std::string JET_JER;                       // "central" "up" "down"
    std::string JET_JEC_TYPE;                  // "Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"
    bool JET_JEC_DIR;
    // MET OPTIONS =- -= =- -= =- -= =- -=
    std::string MET_SYS;                       // "UnclusteredEnUp" "UnclusteredEnDown"
    bool MET_XYCORR;
    // FLASHGG OPTIONS =- -= =- -= =- -= =- -=
    int FLASHGG_DIPHOTON_INDEX;

    EventCfg(std::string name_ = ""){
      name = name_;

      // GENPARTICLES OPTIONS =- -= =- -= =- -= =- -=
      USE_GENPARTICLES = true;
      GENPARTICLES_SELECT_STATUS_CODES = {};
      GENPARTICLES_SELECT_IDS = {};
      // PHOTONS OPTIONS =- -= =- -= =- -= =- -=
      FLASHGG_DIPHOTON_INDEX = -1;
      PHOTON_N_MAX = 999;
      PHOTON_PT_CUT = 0;
      PHOTON_ETA_CUT = 9999;
      PHOTON_ETA_HOLE_CUT_START = -999;
      PHOTON_ETA_HOLE_CUT_END   = -999;
      PHOTON_ID_CUT = id_names::none;
      PHOTON_ENERGY_CORRECTION_TYPE = "";
      PHOTON_SET_SFS = true;
      // ELECTRONS OPTIONS =- -= =- -= =- -= =- -=
      ELECTRON_N_MAX = 999;
      ELECTRON_ENERGY_CORRECTION_TYPE = "";
      ELECTRON_PT_CUT = 0;
      ELECTRON_ETA_CUT = 9999;
      ELECTRON_ETA_HOLE_CUT_START = -999;
      ELECTRON_ETA_HOLE_CUT_END = -999;
      ELECTRON_ID_CUT = id_names::none;
      // MUONS OPTIONS =- -= =- -= =- -= =- -=
      MUON_N_MAX = 999;
      MUON_PT_CUT = 0;
      MUON_ETA_CUT = 999;
      MUON_ISOID_CUT = id_names::none;
      // JETS OPTIONS =- -= =- -= =- -= =- -=
      JET_LTAG_N_MAX = 999;
      JET_BTAG_N_MAX = 999;
      JET_PT_CUT = 0;
      JET_ETA_CUT = 9999;
      JET_ID_CUT = id_names::none;
      JET_PUJID_CUT = id_names::none;
      JET_BTAGGER = "DeepCSV";
      JET_BTAGGER_ID = id_names::none;
      JET_JER = "central";
      JET_JEC_TYPE = "";
      JET_JEC_DIR = true;
      // MET OPTIONS =- -= =- -= =- -= =- -=
      MET_SYS = "";
      MET_XYCORR = true;
      // FLASHGG OPTIONS =- -= =- -= =- -= =- -=
      FLASHGG_DIPHOTON_INDEX = -1;
    }

    // -==--==--==--==--==--==--==--==--==-
    bool SameGenParticles( const EventCfg & other ) const {
      if( USE_GENPARTICLES != other.USE_GENPARTICLES ) return false;
      if( GENPARTICLES_SELECT_STATUS_CODES.size() != other.GENPARTICLES_SELECT_STATUS_CODES.size() ) return false;
      for(int i = 0; i < GENPARTICLES_SELECT_STATUS_CODES.size(); i++)
        if( GENPARTICLES_SELECT_STATUS_CODES[i] != other.GENPARTICLES_SELECT_STATUS_CODES[i] ) return false;
      if( GENPARTICLES_SELECT_IDS.size() != other.GENPARTICLES_SELECT_IDS.size() ) return false;
      for(int i = 0; i < GENPARTICLES_SELECT_IDS.size(); i++)
        if( GENPARTICLES_SELECT_IDS[i] != other.GENPARTICLES_SELECT_IDS[i] ) return false;
      return true;
    }
    // -==--==--==--==--==--==--==--==--==-
    bool SamePhotons( const EventCfg & other ) const {
      if( PHOTON_ENERGY_CORRECTION_TYPE != other.PHOTON_ENERGY_CORRECTION_TYPE ) return false;
      if( PHOTON_PT_CUT != other.PHOTON_PT_CUT ) return false;
      if( PHOTON_ETA_CUT != other.PHOTON_ETA_CUT ) return false;
      if( PHOTON_ETA_HOLE_CUT_START != other.PHOTON_ETA_HOLE_CUT_START ) return false;
      if( PHOTON_ETA_HOLE_CUT_END != other.PHOTON_ETA_HOLE_CUT_END ) return false;
      if( PHOTON_ID_CUT != other.PHOTON_ID_CUT ) return false;
      if( PHOTON_N_MAX != other.PHOTON_N_MAX ) return false;
      if( FLASHGG_DIPHOTON_INDEX != other.FLASHGG_DIPHOTON_INDEX ) return false;
      return true;
    }
    // -==--==--==--==--==--==--==--==--==-
    bool SameElectrons( const EventCfg & other ) const {
      if( ELECTRON_ENERGY_CORRECTION_TYPE != other.ELECTRON_ENERGY_CORRECTION_TYPE ) return false;
      if( ELECTRON_PT_CUT != other.ELECTRON_PT_CUT ) return false;
      if( ELECTRON_ETA_CUT != other.ELECTRON_ETA_CUT ) return false;
      if( ELECTRON_ETA_HOLE_CUT_START != other.ELECTRON_ETA_HOLE_CUT_START ) return false;
      if( ELECTRON_ETA_HOLE_CUT_END != other.ELECTRON_ETA_HOLE_CUT_END ) return false;
      if( ELECTRON_ID_CUT != other.ELECTRON_ID_CUT ) return false;
      if( ELECTRON_N_MAX != other.ELECTRON_N_MAX ) return false;
      if( FLASHGG_DIPHOTON_INDEX != other.FLASHGG_DIPHOTON_INDEX ) return false;
      return true;
    }
    // -==--==--==--==--==--==--==--==--==-
    bool SameMuons( const EventCfg & other ) const {
      if( MUON_PT_CUT != other.MUON_PT_CUT ) return false;
      if( MUON_ETA_CUT != other.MUON_ETA_CUT ) return false;
      if( MUON_ISOID_CUT != other.MUON_ISOID_CUT ) return false;
      if( MUON_N_MAX != other.MUON_N_MAX ) return false;
      if( FLASHGG_DIPHOTON_INDEX != other.FLASHGG_DIPHOTON_INDEX ) return false;
      return true;
    }
    // -==--==--==--==--==--==--==--==--==-
    bool SameJets( const EventCfg & other ) const {
      if( JET_PT_CUT != other.JET_PT_CUT ) return false;
      if( JET_ETA_CUT != other.JET_ETA_CUT ) return false;
      if( JET_ID_CUT != other.JET_ID_CUT ) return false;
      if( JET_BTAGGER != other.JET_BTAGGER ) return false;
      if( JET_BTAGGER_ID != other.JET_BTAGGER_ID ) return false;
      if( JET_JER != other.JET_JER ) return false;
      if( JET_JEC_TYPE != other.JET_JEC_TYPE ) return false;
      if( JET_JEC_DIR != other.JET_JEC_DIR ) return false;
      if( JET_PUJID_CUT!= other.JET_PUJID_CUT ) return false;
      if( JET_LTAG_N_MAX!= other.JET_LTAG_N_MAX ) return false;
      if( JET_BTAG_N_MAX!= other.JET_BTAG_N_MAX ) return false;
      return true;
    }

    /*
def make_comp():
  for line in lines.split("\n"):
    var_name = line.split()[1].split(";")[0]
    print "if(",var_name,"!= other."+ var_name,") return false;"
  print "return true;"
    */

    map<std::string, TH1*> hists;
    void SetHist(const std::string & hist_name, TH1* hist){ hists[ hist_name ] = hist; }
    TH1* GetHist(const std::string hist_name) const {
      auto it = hists.find( hist_name );
      if( it == hists.end() ){
        msg("hzura::ObjectPreselector.GetHist(): can't find histogram", hist_name, "in config", name, "; available hists:");
        for(auto iter = hists.begin(); iter != hists.end(); ++it) msg( it->first );
        return nullptr;
      }
      return it->second;
    }
  };

  EventCfg copy_cfg(const EventCfg & other, std::string name){
    EventCfg answer = other;
    answer.name = name;
    return answer;
  }

};

#endif



