
#ifndef hzura_selection_helpers_h
#define hzura_selection_helpers_h 1

namespace hzura {

  void calc_btag_variables(hzura::Jet * j){
    Events * e = hzura::glob::event;
    const int & i = j->index;

    // b-tagging
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2017#B_tagging
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    // DeepCSV
    // jet.pfDeepCSVJetTags_probb    = j.bDiscriminator("pfDeepCSVJetTags:probb");
    // jet.pfDeepCSVJetTags_probbb   = j.bDiscriminator("pfDeepCSVJetTags:probbb");
    // jet.pfDeepCSVJetTags_probc    = j.bDiscriminator("pfDeepCSVJetTags_probc");
    // jet.pfDeepCSVJetTags_probudsg = j.bDiscriminator("pfDeepCSVJetTags:probudsg");

    // pfDeepCSVJetTags:probb + pfDeepCSVJetTags:probbb 
    j->btag_DeepCSV_val = e->Jets_pfDeepCSVJetTags_probb[i] + e->Jets_pfDeepCSVJetTags_probbb[i];
    j->btag_DeepCSV_isLoose  = (j->btag_DeepCSV_val > 0.1241) ? true : false;
    j->btag_DeepCSV_isMedium = (j->btag_DeepCSV_val > 0.4184) ? true : false;
    j->btag_DeepCSV_isTight  = (j->btag_DeepCSV_val > 0.7527) ? true : false;

    // DeepJet
    // jet.pfDeepFlavourJetTags_probb    = j.bDiscriminator("pfDeepFlavourJetTags_probb");
    // jet.pfDeepFlavourJetTags_probbb   = j.bDiscriminator("pfDeepFlavourJetTags_probbb");
    // jet.pfDeepFlavourJetTags_problepb = j.bDiscriminator("pfDeepFlavourJetTags_problepb");
    // jet.pfDeepFlavourJetTags_probc    = j.bDiscriminator("pfDeepFlavourJetTags:probc");
    // jet.pfDeepFlavourJetTags_probuds  = j.bDiscriminator("pfDeepFlavourJetTags:probuds");
    // jet.pfDeepFlavourJetTags_probg    = j.bDiscriminator("pfDeepFlavourJetTags:probg");

    // pfDeepFlavourJetTags:probb + pfDeepFlavourJetTags:probbb + pfDeepFlavourJetTags:problepb
    j->btag_DeepFlavour_val = e->Jets_pfDeepFlavourJetTags_probb[i] + e->Jets_pfDeepFlavourJetTags_probbb[i] + e->Jets_pfDeepFlavourJetTags_problepb[i];
    j->btag_DeepFlavour_isLoose  = (j->btag_DeepFlavour_val > 0.0494) ? true : false;
    j->btag_DeepFlavour_isMedium = (j->btag_DeepFlavour_val > 0.2770) ? true : false;
    j->btag_DeepFlavour_isTight  = (j->btag_DeepFlavour_val > 0.7264) ? true : false;
  };

  void calc_photon_iso(hzura::Photon * p){
    // ???
    // https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolationRun2
    // https://arxiv.org/pdf/1502.02702.pdf
    // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2

    const Float_t & pt                 = hzura::glob::event->Photons_pt[ p->index ];
    const Float_t & eta                = hzura::glob::event->Photons_eta[ p->index ];

    const Float_t & sumChargedHadronPt = hzura::glob::event->Photons_sumChargedHadronPt[ p->index ]; 
    const Float_t & sumNeutralHadronEt = hzura::glob::event->Photons_sumNeutralHadronEt[ p->index ]; 
    const Float_t & sumPhotonEt        = hzura::glob::event->Photons_sumPhotonEt[ p->index ]; 

    if( hzura::glob::year_era == "2016" ){
    } else 
    if( hzura::glob::year_era == "2017" ){
    } else
    if( hzura::glob::year_era == "2018" ){
    }
  };

};

#endif

