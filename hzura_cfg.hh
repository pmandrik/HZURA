
#ifndef hzura_cfg_h
#define hzura_cfg_h 1

#include "Events.h"

namespace hzura {

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

};

#endif



