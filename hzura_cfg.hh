
#ifndef hzura_cfg_h
#define hzura_cfg_h 1

#include "Events.h"

namespace hzura {

  enum id_names {
    none = -1,
    loose = 0,
    medium,
    tight,
  };

  namespace glob {
  
    bool is_data;
    std::string year_era;
    int year;
    Events * event;
    TRandom3 randGen;

    void Init(const string & era, Events * event){

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
    // PHOTONS OPTIONS =- -= =- -= =- -= =- -=
    std::string PHOTON_ENERGY_CORRECTION_TYPE; // "ecalEnergyPreCorr" "ecalEnergyPostCorr" "energyScaleUp" "energyScaleDown" "energyScaleStatUp" "energyScaleStatDown" "energyScaleSystUp"
                                               // "energyScaleSystDown" "energyScaleGainUp" "energyScaleGainDown" "energyScaleEtUp" "energyScaleEtDown" 
                                               // "energySigmaUp" "energySigmaDown" "energySigmaPhiUp" "energySigmaPhiDown" "energySigmaRhoUp" "energySigmaRhoDown"
    Float_t PHOTON_PT_CUT;
    Float_t PHOTON_ETA_CUT;
    Float_t PHOTON_ETA_HOLE_CUT_START;
    Float_t PHOTON_ETA_HOLE_CUT_END;
    int PHOTON_ID_CUT;                         // id_names::loose id_names::medium id_names::tight
    // ELECTRONS OPTIONS =- -= =- -= =- -= =- -=
    std::string ELECTRON_ENERGY_CORRECTION_TYPE; // same as PHOTON_ENERGY_CORRECTION_TYPE
    Float_t ELECTRON_PT_CUT;
    Float_t ELECTRON_ETA_CUT;
    Float_t ELECTRON_ETA_HOLE_CUT_START;
    Float_t ELECTRON_ETA_HOLE_CUT_END;
    int ELECTRON_ID_CUT;                       // id_names::loose id_names::medium id_names::tight
    // MUONS OPTIONS =- -= =- -= =- -= =- -=
    Float_t MUON_PT_CUT;
    Float_t MUON_ETA_CUT;
    int MUON_ISOID_CUT;                        // id_names::loose id_names::medium id_names::tight
    // JETS OPTIONS =- -= =- -= =- -= =- -=
    Float_t JET_PT_CUT;
    Float_t JET_ETA_CUT;
    int JET_ID_CUT;                            // id_names::loose id_names::medium id_names::tight
    std::string JET_BTAGGER;                   // "DeepCSV"
    int JET_BTAGGER_ID;                        // id_names::loose id_names::medium id_names::tight
    std::string JET_JER;                       // "central" "up" "down"
    std::string JET_JEC_TYPE;                  // "Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"
    bool JET_JEC_DIR;
    // MET OPTIONS =- -= =- -= =- -= =- -=
    std::string MET_SYS;                       // "UnclusteredEnUp" "UnclusteredEnDown"
    bool MET_XYCORR;

    EventCfg(std::string name_ = ""){
      name = name_;

      PHOTON_PT_CUT = 0;
      PHOTON_ETA_CUT = 9999;
      PHOTON_ETA_HOLE_CUT_START = -999;
      PHOTON_ETA_HOLE_CUT_END   = -999;
      PHOTON_ID_CUT = id_names::none;
      PHOTON_ENERGY_CORRECTION_TYPE = "";
      // ELECTRONS OPTIONS =- -= =- -= =- -= =- -=
      ELECTRON_ENERGY_CORRECTION_TYPE = "";
      ELECTRON_PT_CUT = 0;
      ELECTRON_ETA_CUT = 9999;
      ELECTRON_ETA_HOLE_CUT_START = -999;
      ELECTRON_ETA_HOLE_CUT_END = -999;
      ELECTRON_ID_CUT = id_names::none;
      // MUONS OPTIONS =- -= =- -= =- -= =- -=
      MUON_PT_CUT = 0;
      MUON_ETA_CUT = 999;
      MUON_ISOID_CUT = id_names::none;
      // JETS OPTIONS =- -= =- -= =- -= =- -=
      JET_PT_CUT = 0;
      JET_ETA_CUT = 9999;
      JET_ID_CUT = id_names::none;
      JET_BTAGGER = "DeepCSV";
      JET_BTAGGER_ID = id_names::none;
      JET_JER = "central";
      JET_JEC_TYPE = "";
      bool JET_JEC_DIR = true;
      // MET OPTIONS =- -= =- -= =- -= =- -=
      std::string MET_SYS = "";
      bool MET_XYCORR = true;
    }

    // -==--==--==--==--==--==--==--==--==-
    bool SamePhotons( const EventCfg & other ) const {
      if( PHOTON_ENERGY_CORRECTION_TYPE != other.PHOTON_ENERGY_CORRECTION_TYPE ) return false;
      if( PHOTON_PT_CUT != other.PHOTON_PT_CUT ) return false;
      if( PHOTON_ETA_CUT != other.PHOTON_ETA_CUT ) return false;
      if( PHOTON_ETA_HOLE_CUT_START != other.PHOTON_ETA_HOLE_CUT_START ) return false;
      if( PHOTON_ETA_HOLE_CUT_END != other.PHOTON_ETA_HOLE_CUT_END ) return false;
      if( PHOTON_ID_CUT != other.PHOTON_ID_CUT ) return false;
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
      return true;
    }
    // -==--==--==--==--==--==--==--==--==-
    bool SameMuons( const EventCfg & other ) const {
      if( MUON_PT_CUT != other.MUON_PT_CUT ) return false;
      if( MUON_ETA_CUT != other.MUON_ETA_CUT ) return false;
      if( MUON_ISOID_CUT != other.MUON_ISOID_CUT ) return false;
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
      return true;
    }

    /*
def make_comp():
  for line in lines.split("\n"):
    var_name = line.split()[1].split(";")[0]
    print "if(",var_name,"!= other."+ var_name,") return false;"
  print "return true;"
    */

  };

};

#endif



