
#ifndef hzura_analyse_configuration_helpers_h
#define hzura_analyse_configuration_helpers_h 1

namespace hzura {

  // PUJID ===============================================================================================
  PUJIDReader get_pujid_reader(){
    PUJIDReader answer;
    // https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Working_points
    answer.eta_range = std::vector<double>( {2.5, 2.75, 3.0, 5.0} );
    answer.pt_range  = std::vector<double>( {10,    20,  30,  50} );

    // Tight Id
    answer.tight.push_back( std::vector<double>( {0.69, -0.35, -0.26, -0.21} ) );
    answer.tight.push_back( std::vector<double>( {0.69, -0.35, -0.26, -0.21} ) );
    answer.tight.push_back( std::vector<double>( {0.69, -0.35, -0.26, -0.21} ) );
    answer.tight.push_back( std::vector<double>( {0.86, -0.10, -0.05, -0.01} ) );
    // Medium Id
    answer.medium.push_back( std::vector<double>( {0.18, -0.55, -0.42, -0.36} ) );
    answer.medium.push_back( std::vector<double>( {0.18, -0.55, -0.42, -0.36} ) );
    answer.medium.push_back( std::vector<double>( {0.18, -0.55, -0.42, -0.36} ) );
    answer.medium.push_back( std::vector<double>( {0.61, -0.35, -0.23, -0.17} ) );
    // Loose Id
    answer.loose.push_back( std::vector<double>( {-0.97, -0.68, -0.53, -0.47} ) );
    answer.loose.push_back( std::vector<double>( {-0.97, -0.68, -0.53, -0.47} ) );
    answer.loose.push_back( std::vector<double>( {-0.97, -0.68, -0.53, -0.47} ) );
    answer.loose.push_back( std::vector<double>( {-0.89, -0.52, -0.38, -0.30} ) );

    // efficiency
    SFFileReader *sfs  = new SFFileReader( "data/PUJID/scalefactorsPUID_81Xtraining.root" );

    if(hzura::glob::year_era == "2017" or true){
      answer.AddReader( "lmt",  sfs );
      answer.GetSF_eff_loose    = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_eff_sf2017_L", "lmt", "h2_eff_sf2017_L_Systuncty");  };
      answer.GetSF_eff_medium   = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_eff_sf2017_M", "lmt", "h2_eff_sf2017_M_Systuncty");  };
      answer.GetSF_eff_tight    = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_eff_sf2017_T", "lmt", "h2_eff_sf2017_T_Systuncty");  };
      answer.GetSF_mistag_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_mistag_sf2017_L", "lmt", "h2_mistag_sf2017_L_Systuncty");  };
      answer.GetSF_mistag_medium = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_mistag_sf2017_L", "lmt", "h2_mistag_sf2017_L_Systuncty");  };
      answer.GetSF_mistag_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_alt(pt, eta, unc, "lmt", "h2_mistag_sf2017_L", "lmt", "h2_mistag_sf2017_L_Systuncty");  };
    }

    return answer;
  }

  // btag SF ===============================================================================================
  BTagSFReader get_btag_sf_reader(const std::string & working_point, const std::string & jet_flavour, std::string tagger = "DeepFlavour"){ // TODO 2017 2018
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    BTagSFReader answer(working_point, jet_flavour);

    if(hzura::glob::year_era == "2016"){
      if( tagger == "DeepCSV" )     answer.AddFile("DeepCSV", "data/jets_sf_2016/DeepCSV_2016LegacySF_WP_V1.csv"); // jets_sf_2016/DeepCSV_2016LegacySF_V1.csv
      if( tagger == "DeepFlavour" ) answer.AddFile("DeepFlavour", "data/jets_sf_2016/DeepJet_2016LegacySF_WP_V1.csv");
    }
    if(hzura::glob::year_era == "2017"){
      if( tagger == "DeepCSV" )     answer.AddFile("DeepCSV", "data/jets_sf_2017/DeepCSV_94XSF_WP_V4_B_F.csv");
      if( tagger == "DeepFlavour" ) answer.AddFile("DeepFlavour", "data/jets_sf_2017/DeepFlavour_94XSF_WP_V3_B_F.csv");
    }
    if(hzura::glob::year_era == "2018") {
      if( tagger == "DeepFlavour" ) answer.AddFile("DeepFlavour", "data/jets_sf_2018/DeepJet_102XSF_WP_V1.csv");
    }
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

    msg("hzura::get_electrons_sf_reader() ============================================================================================ start");
    if(hzura::glob::year_era == "2016"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronLoose_Fall17V2.root" , 1.0, false, 1.0);
      SFFileReader *sf_m  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronMedium_Fall17V2.root", 1.0, false, 1.0);
      SFFileReader *sf_t  = new SFFileReader( "data/electrons_sf_2016/2016LegacyReReco_ElectronTight_Fall17V2.root" , 1.0, false, 1.0);

      answer.AddReader( "l",  sf_l );
      answer.AddReader( "m",  sf_m );
      answer.AddReader( "t",  sf_t );

      answer.hname_loose  = "EGamma_SF2D";
      answer.hname_medium = "EGamma_SF2D";
      answer.hname_tight  = "EGamma_SF2D";
    }
    if(hzura::glob::year_era == "2017"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronLoose.root"  , 1.0, false, 1.0);
      SFFileReader *sf_m  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronMedium.root" , 1.0, false, 1.0);
      SFFileReader *sf_t  = new SFFileReader( "data/electrons_sf_2017/2017_ElectronTight.root"  , 1.0, false, 1.0);

      answer.AddReader( "l",  sf_l );
      answer.AddReader( "m",  sf_m );
      answer.AddReader( "t",  sf_t );

      answer.hname_loose  = "EGamma_SF2D";
      answer.hname_medium = "EGamma_SF2D";
      answer.hname_tight  = "EGamma_SF2D";
    }
    if(hzura::glob::year_era == "2018"){ // Fall17V2 2017 ID ???
      SFFileReader *sf_l  = new SFFileReader( "data/electrons_sf_2018/2018_ElectronLoose.root"  , 1.0, false, 1.0);
      SFFileReader *sf_m  = new SFFileReader( "data/electrons_sf_2018/2018_ElectronMedium.root" , 1.0, false, 1.0);
      SFFileReader *sf_t  = new SFFileReader( "data/electrons_sf_2018/2018_ElectronTight.root"  , 1.0, false, 1.0);

      answer.AddReader( "l",  sf_l );
      answer.AddReader( "m",  sf_m );
      answer.AddReader( "t",  sf_t );

      answer.hname_loose  = "EGamma_SF2D";
      answer.hname_medium = "EGamma_SF2D";
      answer.hname_tight  = "EGamma_SF2D";
    }
    msg("hzura::get_electrons_sf_reader() ============================================================================================ end");
    
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

      answer.GetSF_id = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id_err", hname) + unc * answer.GetErr(eta, pt, "id_err", hname); };
      answer.GetSF_iso  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "iso_err", hname) + unc * answer.GetErr(eta, pt, "iso_err", hname); };

      answer.GetSF_id_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_LooseID_DEN_genTracks_pt_abseta");  };
      answer.GetSF_id_medium = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_MediumID_DEN_genTracks_pt_abseta"); };
      answer.GetSF_id_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_TightID_DEN_genTracks_pt_abseta");  }; 

      answer.GetSF_iso_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_LooseRelIso_DEN_LooseID_pt_abseta"); };
   // answer.GetSF_iso_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_iso_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta"); }; 

      vector<SFFileReader *> readers = { sf_id, sf_iso, sf_id_err, sf_iso_err };
      for( auto reader : readers ){
        reader->msg_if_outside_range   = false;
        reader->value_if_outside_range = 1.0;
      }
    }

    if(hzura::glob::year_era == "2018"){
      // https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
      SFFileReader *sf_id  = new SFFileReader("data/muons_sf_2018/RunABCD_SF_ID.root");
      SFFileReader *sf_iso = new SFFileReader("data/muons_sf_2018/RunABCD_SF_ISO.root");

      answer.AddReader( "id",  sf_id  );
      answer.AddReader( "iso", sf_iso );

      answer.GetSF_id = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "id", hname) + unc * answer.GetErr(eta, pt, "id", hname); };
      answer.GetSF_iso  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc, const std::string & hname){ return answer.GetSF(eta, pt, "iso", hname) + unc * answer.GetErr(eta, pt, "iso", hname); };

      answer.GetSF_id_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_LooseID_DEN_TrackerMuons_pt_abseta");  };
      // answer.GetSF_id_medium = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_MediumID_DEN_genTracks_pt_abseta"); };
      answer.GetSF_id_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_id(pt, TMath::Abs(eta), unc, "NUM_TightID_DEN_TrackerMuons_pt_abseta");  }; 

      answer.GetSF_iso_loose  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_LooseRelIso_DEN_LooseID_pt_abseta"); };
      // answer.GetSF_iso_medium = [](const Float_t & eta, const Float_t & pt, const int & unc) { return GetSF_iso(eta, pt, unc, "NUM_MediumID_DEN_genTracks_eta_pt") };
      answer.GetSF_iso_tight  = [ answer ](const Float_t & eta, const Float_t & pt, const int & unc) { return answer.GetSF_iso(pt, TMath::Abs(eta), unc, "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta"); }; 

      vector<SFFileReader *> readers = { sf_id, sf_iso };
      for( auto reader : readers ){
        reader->msg_if_outside_range   = false;
        reader->value_if_outside_range = 1.0;
      }
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
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECUncertaintySources#Run_2_reduced_set_of_uncertainty
  JecReader get_jec_uncertanties(){
    // https://twiki.cern.ch/twiki/bin/view/CMS/JECDataMC
    std::vector<std::string> unc_names;
    std::string unc_inp_file;
    if(hzura::glob::year_era == "2016") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "data/JEC/Summer16_07Aug2017_V11_MC/Summer16_07Aug2017_V11_MC_UncertaintySources_AK4PF.txt";

      unc_names = {"Absolute", "Absolute_2016", "BBEC1", "BBEC1_2016", "EC2", "EC2_2016", "FlavorQCD", "HF", "HF_2016", "RelativeBal", "RelativeSample_2016"};
      unc_inp_file = "data/JEC/Regrouped_Summer16_07Aug2017_V11_MC_UncertaintySources_AK4PFchs.txt";
    }
    if(hzura::glob::year_era == "2017") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "data/JEC/Fall17_17Nov2017_V32_MC/Fall17_17Nov2017_V32_MC_UncertaintySources_AK4PF.txt";

      unc_names = {"Absolute", "Absolute_2017", "BBEC1", "BBEC1_2017", "EC2", "EC2_2017", "FlavorQCD", "HF", "HF_2017", "RelativeBal", "RelativeSample_2017"};
      unc_inp_file = "data/JEC/Regrouped_Fall17_17Nov2017_V32_MC_UncertaintySources_AK4PFchs.txt";
    }
    if(hzura::glob::year_era == "2018") {
      unc_names = {"Total", "SubTotalMC", "SubTotalAbsolute", "SubTotalScale", "SubTotalPt", "SubTotalRelative", "SubTotalPileUp", "FlavorQCD", "TimePtEta"};
      unc_inp_file = "";

      unc_names = {"Absolute", "Absolute_2018", "BBEC1", "BBEC1_2018", "EC2", "EC2_2018", "FlavorQCD", "HF", "HF_2018", "RelativeBal", "RelativeSample_2018"};
      unc_inp_file = "data/JEC/Regrouped_Autumn18_V19_MC_UncertaintySources_AK4PFchs.txt";
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






