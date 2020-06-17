
#ifndef hzura_analyse_configuration_helpers_h
#define hzura_analyse_configuration_helpers_h 1

namespace hzura {

  // btag SF ===============================================================================================
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

  // muons, electrons, photons SF ===============================================================================================
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
    if(hzura::glob::year_era == "2017"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronLoose.root" );
      SFFileReader *sf_m  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronMedium.root" );
      SFFileReader *sf_t  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronTight.root" );

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

    if(hzura::glob::year_era == "2017"){
      SFFileReader *sf_id  = new SFFileReader("data/muons_sf_2017/RunBCDEF_SF_ID.root");
      SFFileReader *sf_iso = new SFFileReader("data/muons_sf_2017/RunBCDEF_SF_ISO.root");

      SFFileReader *sf_id_err  = new SFFileReader("data/muons_sf_2017/RunBCDEF_SF_ID_syst.root");
      SFFileReader *sf_iso_err = new SFFileReader("data/muons_sf_2017/RunBCDEF_SF_ISO_syst.root");

      answer.AddReader( "id",  sf_id  );
      answer.AddReader( "iso", sf_iso );
      answer.AddReader( "id_err",  sf_id_err  );
      answer.AddReader( "iso_err", sf_iso_err );

      //answer.GetSF_id = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id", hname) + unc * answer.GetErr(eta, pt, "id_err", hname); };
      //answer.GetSF_iso  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "iso", hname) + unc * answer.GetErr(eta, pt, "iso_err", hname); };
      answer.GetSF_id = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id_err", hname) + unc * answer.GetErr(eta, pt, "id_err", hname); };
      answer.GetSF_iso  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id_err", hname) + unc * answer.GetErr(eta, pt, "id_err", hname); };

      answer.GetSF_id_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_LooseID_DEN_genTracks_pt_abseta");  };
      answer.GetSF_id_medium = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_MediumID_DEN_genTracks_pt_abseta"); };
      answer.GetSF_id_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_TightID_DEN_genTracks_pt_abseta");  }; 

      answer.GetSF_iso_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_LooseRelIso_DEN_LooseID_pt_abseta"); };
      // answer.GetSF_iso_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_iso_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta"); }; 
    }
  
    return answer;
  }

  // PileUp reweighting reader ===============================================================================================
  //  definition of loaded data =-
  PileUpSFReader get_pileup_sf_reader(){
    PileUpSFReader answer;
    if(hzura::glob::year_era == "2016") answer.Init("data/pileup/puweights_2016.root");
    if(hzura::glob::year_era == "2017") answer.Init("data/pileup/puweights_2017.root");
    if(hzura::glob::year_era == "2018") answer.Init("data/pileup/puweights_2018.root");
    
    return answer;
  }

  // Definition of JEC uncertanties  ===============================================================================================
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
      unc_inp_file = "data/JEC/Fall17_17Nov2017_V32_MC/Fall17_17Nov2017_V32_MC_UncertaintySources_AK4PF.txt";
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






