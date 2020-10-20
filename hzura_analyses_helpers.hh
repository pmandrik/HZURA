
#ifndef hzura_analyses_helpers_h
#define hzura_analyses_helpers_h 1

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

  // get leading index, Pt
  template <typename A> int get_leading_by_pt(const std::vector<A> & inp_1){
    double leading_pt = -999;
    int answer = -1;
    for(int i = 0; i < inp_1.size(); i++){
      const A & obj = inp_1[i];
      if( leading_pt > obj.tlv.Pt() ) continue;
      leading_pt = obj.tlv.Pt();
      answer     = i;
    }
    return answer;
  }

  vector<int> select_genparticles(const std::vector<hzura::GenParticle> & particles, const int & id, const int & status){
    vector<int> answer;
    for(int i = 0, i_max = particles.size(); i < i_max; i++){
      if( id != -1 and abs(particles[i].pdg_id) != id ) continue;
      if( status != -1 and particles[i].status != status ) continue;
      answer.push_back( i );
    }
    return answer;
  }

  vector<int> select_quarks(const std::vector<hzura::GenParticle> & particles, const int & status){
    vector<int> answer;
    for(int i = 0, i_max = particles.size(); i < i_max; i++){
      if( abs(particles[i].pdg_id) > 6 ) continue;
      if( status != -1 and particles[i].status != status ) continue;
      answer.push_back( i );
    }
    return answer;
  }

  vector<int> select_neitrinos(const std::vector<hzura::GenParticle> & particles, const int & status){
    vector<int> answer;
    for(int i = 0, i_max = particles.size(); i < i_max; i++){
      if( abs(particles[i].pdg_id) != 12 and abs(particles[i].pdg_id) != 14 and abs(particles[i].pdg_id) != 16 ) continue;
      if( status != -1 and particles[i].status != status ) continue;
      answer.push_back( i );
    }
    return answer;
  }

  vector<int> select_charged_leptons(const std::vector<hzura::GenParticle> & particles, const int & status){
    vector<int> answer;
    for(int i = 0, i_max = particles.size(); i < i_max; i++){
      if( abs(particles[i].pdg_id) != 11 and abs(particles[i].pdg_id) != 13 and abs(particles[i].pdg_id) != 15 ) continue;
      if( status != -1 and particles[i].status != status ) continue;
      answer.push_back( i );
    }
    return answer;
  }

  template <typename A> vector<A> get_particles_by_indexes(const vector<A> & input, const vector<int> & indexes){
    vector<A> answer;
    for(auto index : indexes) answer.emplace_back( input.at( index ) );
    return answer;
  }

  // HHWWgg related tools ===============================================================================================
  bool set_HHWWgg_genparticles(const std::vector<hzura::GenParticle> & genparticles,
                              TLorentzVector & H1f, TLorentzVector & H2f, TLorentzVector & H1i, TLorentzVector & H2i,
                              TLorentzVector & pho1, TLorentzVector & pho2,
                              TLorentzVector & w1, TLorentzVector & w2,
                              TLorentzVector & nu, TLorentzVector & j1, TLorentzVector & j2, hzura::GenParticle & lep
  ){
    vector<int> gen_higgs_i  = select_genparticles(genparticles, 25, 22);
    vector<int> gen_higgs_f  = select_genparticles(genparticles, 25, 62);
    vector<int> gen_photons  = select_genparticles(genparticles, 22, -1);
    vector<int> gen_ws       = select_genparticles(genparticles, 24, 22);
    vector<int> gen_quarks   = select_quarks(genparticles, 23);
    vector<int> gen_nu       = select_neitrinos(genparticles, -1);
    vector<int> gen_el       = select_charged_leptons(genparticles, -1);

    // photons -=-=-=-
    if(  gen_photons.size() < 2){
      msg_err("set_HHWWgg_genparticles(): gen_photons.size() = ",  gen_photons.size(), " < 2"); 
      return false;
    }

    vector<hzura::GenParticle> photons = get_particles_by_indexes(genparticles, gen_photons);
    sort(photons.begin(), photons.end(), [](const hzura::GenParticle & a, const hzura::GenParticle & b) -> bool { return a.tlv.Pt() > b.tlv.Pt(); });
    // msg( photons[0].tlv.Pt(), photons[1].tlv.Pt() );
    pho1 = photons[0].tlv;
    pho2 = photons[1].tlv;

    // higgs -=-=-=-
    if( gen_higgs_i.size() != 2 or gen_higgs_f.size() != 2){
      msg_err("set_HHWWgg_genparticles(): gen_higgs_initial.size(), gen_higgs_final.size() = ", gen_higgs_i.size(), gen_higgs_f.size(), " not equal to 2"); 
      return false;
    }
    H1i = genparticles[gen_higgs_i[0]].tlv;
    H2i = genparticles[gen_higgs_i[1]].tlv;
    H1f = genparticles[gen_higgs_f[0]].tlv;
    H2f = genparticles[gen_higgs_f[1]].tlv;

    if( (pho1 + pho2).DeltaR(H1i) > (pho1 + pho2).DeltaR(H2i) ) swap( H1i, H2i );
    if( (pho1 + pho2).DeltaR(H1f) > (pho1 + pho2).DeltaR(H2f) ) swap( H1f, H2f );
    
    // nu -=-=-=-
    if( gen_nu.size() == 0 ){
      msg_err("set_HHWWgg_genparticles(): gen_nu.size() = ",  gen_ws.size(), " < 2"); 
      return false;
    }
    vector<hzura::GenParticle> nus = get_particles_by_indexes(genparticles, gen_nu);
    sort(nus.begin(), nus.end(), [](const hzura::GenParticle & a, const hzura::GenParticle & b) -> bool { return a.tlv.Pt() > b.tlv.Pt(); });
    nu = nus[0].tlv;

    // gen_quarks -=-=-=-
    vector<hzura::GenParticle> qs = get_particles_by_indexes(genparticles, gen_quarks);
    sort(qs.begin(), qs.end(), [](const hzura::GenParticle & a, const hzura::GenParticle & b) -> bool { return a.tlv.Pt() > b.tlv.Pt(); });
    if( qs.size() < 2 ){
      msg_err("set_HHWWgg_genparticles(): quarks.size() = ",  qs.size(), " < 2"); 
      return false;
    }
    // for(auto q : qs) msg( q.pdg_id, q.status, q.tlv.Pt() );
    j1 = qs[0].tlv;
    j2 = qs[1].tlv;

    // ws -=-=-=-
    if( gen_ws.size() != 2 ){
      msg_err("set_HHWWgg_genparticles(): gen_ws.size() = ",  gen_ws.size(), " < 2"); 
      return false;
    }
    w1 = genparticles[gen_ws[0]].tlv;;
    w2 = genparticles[gen_ws[1]].tlv;;

    if( (j1 + j2).DeltaR( w2 ) < (j1 + j2).DeltaR( w2 ) ) swap(w1, w2);

    // el -=-=-=-
    if( gen_el.size() < 1 ){
        msg_err("set_HHWWgg_genparticles(): gen_ws.size() = ",  gen_ws.size(), " < 2"); 
      return false;
    }

    vector<hzura::GenParticle> charged_leptons = get_particles_by_indexes(genparticles, gen_el);
    sort(charged_leptons.begin(), charged_leptons.end(), [](const hzura::GenParticle & a, const hzura::GenParticle & b) -> bool { return a.tlv.Pt() > b.tlv.Pt(); });
    lep = charged_leptons[0];

    return true;
  }

  // reconstruct Higgs from H->WW->jj nu l decay
  bool reconstruct_H_from_WW(const TLorentzVector & P_wl, const TLorentzVector & P_n, TLorentzVector & P_n_answer, TLorentzVector & P_H_answer) {
    double m_h = 125;
		double C = (m_h - P_wl.M()) * (m_h + P_wl.M()) * 0.5 + P_wl.Px() * P_n.Px() + P_wl.Py() * P_n.Py();
		double a = P_wl.E() * P_wl.E() - P_wl.Pz() * P_wl.Pz();
		double b = -2 * C * P_wl.Pz();
		double c = P_wl.E() * P_wl.E() * P_n.Pt() * P_n.Pt() - C * C;

		if(a < 0.00000001) return false;
		b /= a;
		c /= a;
		double D = b * b - 4 * c;
		double x = -b / 2;

		if(D > 0) {
			double k = sqrt(D) / 2;
			
			TLorentzVector p1, p2;
			p1.SetXYZM(P_n.Px(), P_n.Py(), x - k, P_n.M());
			p2.SetXYZM(P_n.Px(), P_n.Py(), x + k, P_n.M());

      TLorentzVector P_H1 = p1 + P_wl;
      TLorentzVector P_H2 = p2 + P_wl;

      // if( P_H1.M() < P_H2.M() ) P_n_answer = p1;
      // else                      P_n_answer = p2;

      if( TMath::Abs( p1.Pz() ) < TMath::Abs( p2.Pz() ) ) P_n_answer = p1;
      else                                                P_n_answer = p2;
		}
		else{
      P_n_answer.SetXYZM(P_n.Px(), P_n.Py(), x, P_n.M());
    }

    P_H_answer = P_n_answer + P_wl;
    return true;
  }

  // create systematic configs ==========================================================
    void add_systematic_cfgs_def(const EventCfg & cfg, std::vector<hzura::EventCfg> & analyses_configs){
      EventCfg cfg_uncorrected = cfg;
      cfg_uncorrected.name = "Uncorrected";
      cfg_uncorrected.ELECTRON_ENERGY_CORRECTION_TYPE = "";
      cfg_uncorrected.PHOTON_ENERGY_CORRECTION_TYPE   = "";

      EventCfg cfg_zero = EventCfg();
      cfg_zero.name = "Zero selections";

      EventCfg cfg_JERUp   = copy_cfg( cfg, "JERUp" );
      cfg_JERUp.JET_JER = "up";

      EventCfg cfg_JERDown   = copy_cfg( cfg, "JERDown" );
      cfg_JERDown.JET_JER = "down";

      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECUncertaintySources#Run_2_reduced_set_of_uncertainty
      std::vector<std::string> jec_uncertanties;
      if(hzura::glob::year_era == "2016") jec_uncertanties = {"Absolute", "Absolute_2016", "BBEC1", "BBEC1_2016", "EC2", "EC2_2016", "FlavorQCD", "HF", "HF_2016", "RelativeBal", "RelativeSample_2016"};
      if(hzura::glob::year_era == "2017") jec_uncertanties = {"Absolute", "Absolute_2017", "BBEC1", "BBEC1_2017", "EC2", "EC2_2017", "FlavorQCD", "HF", "HF_2017", "RelativeBal", "RelativeSample_2017"};
      if(hzura::glob::year_era == "2018") jec_uncertanties = {"Absolute", "Absolute_2018", "BBEC1", "BBEC1_2018", "EC2", "EC2_2018", "FlavorQCD", "HF", "HF_2018", "RelativeBal", "RelativeSample_2018"};

      for(auto jec_uncertantie : jec_uncertanties){
        EventCfg cfg_JECUp   = copy_cfg( cfg, "JEC_" + jec_uncertantie + "Up" );
        cfg_JECUp.JET_JEC_TYPE = jec_uncertantie;
        cfg_JECUp.JET_JEC_DIR = true;

        EventCfg cfg_JECDown = copy_cfg( cfg, "JEC_" + jec_uncertantie + "Down" );
        cfg_JECDown.JET_JEC_TYPE = jec_uncertantie;
        cfg_JECDown.JET_JEC_DIR = false;

        analyses_configs.push_back( cfg_JECUp );
        analyses_configs.push_back( cfg_JECDown );
      }
      /*
      EventCfg cfg_JECUp   = copy_cfg( cfg, "JECUp" );
      cfg_JECUp.JET_JEC_TYPE = "Total";
      cfg_JECUp.JET_JEC_DIR = true;

      EventCfg cfg_JECDown = copy_cfg( cfg, "JECDown" );
      cfg_JECDown.JET_JEC_TYPE = "Total";
      cfg_JECDown.JET_JEC_DIR = false;
      */

      EventCfg cfg_METUp   = copy_cfg( cfg, "UnclusteredMETUp" );
      cfg_METUp.MET_SYS  = "UnclusteredEnUp";

      EventCfg cfg_METDown = copy_cfg( cfg, "UnclusteredMETDown" );
      cfg_METDown.MET_SYS  = "UnclusteredEnDown";

      EventCfg cfg_NO_MET_XY = copy_cfg( cfg, "NoMETXYcorr" );
      cfg_NO_MET_XY.MET_XYCORR  = false;

      EventCfg cfg_PUJID = copy_cfg( cfg, "PUJID_none" );
      cfg_PUJID.JET_PUJID_CUT  = hzura::id_names::none;

      analyses_configs.push_back( cfg_JERUp );
      analyses_configs.push_back( cfg_JERDown );
      analyses_configs.push_back( cfg_METUp );
      analyses_configs.push_back( cfg_METDown );

      // analyses_configs.push_back( cfg_NO_MET_XY );
      analyses_configs.push_back( cfg_PUJID );
    }

    void add_systematic_cfgs_flashgg(const EventCfg & cfg, std::vector<hzura::EventCfg> & analyses_configs){
      std::vector<hzura::EventCfg> cfgs = {  };
      std::vector<std::string> flashgg_sys = { "FNUFEBDown", "FNUFEBUp", "FNUFEEDown", "FNUFEEUp", "MCScaleGain1EBDown", "MCScaleGain1EBUp", "MCScaleGain6EBDown", "MCScaleGain6EBUp", "MCScaleHighR9EBDown", "MCScaleHighR9EBUp", "MCScaleHighR9EEDown", "MCScaleHighR9EEUp", "MCScaleLowR9EBDown", "MCScaleLowR9EBUp", "MCScaleLowR9EEDown", "MCScaleLowR9EEUp", "MCSmearHighR9EBPhiDown", "MCSmearHighR9EBPhiUp", "MCSmearHighR9EBRhoDown", "MCSmearHighR9EBRhoUp", "MCSmearHighR9EEPhiDown", "MCSmearHighR9EEPhiUp", "MCSmearHighR9EERhoDown", "MCSmearHighR9EERhoUp", "MCSmearLowR9EBPhiDown", "MCSmearLowR9EBPhiUp", "MCSmearLowR9EBRhoDown", "MCSmearLowR9EBRhoUp", "MCSmearLowR9EEPhiDown", "MCSmearLowR9EEPhiUp", "MCSmearLowR9EERhoDown", "MCSmearLowR9EERhoUp", "MaterialCentralBarrelDown", "MaterialCentralBarrelUp", "MaterialForwardDown", "MaterialForwardUp", "MaterialOuterBarrelDown", "MaterialOuterBarrelUp", "MvaShiftDown", "MvaShiftUp", "ShowerShapeHighR9EBDown", "ShowerShapeHighR9EBUp", "ShowerShapeHighR9EEDown", "ShowerShapeHighR9EEUp", "ShowerShapeLowR9EBDown", "ShowerShapeLowR9EBUp", "ShowerShapeLowR9EEDown", "ShowerShapeLowR9EEUp", "SigmaEOverEShiftDown", "SigmaEOverEShiftUp" };

      for(int i = 0; i < flashgg_sys.size(); i++){
        EventCfg new_cfg = cfg;
        new_cfg.name = flashgg_sys.at( i );
        new_cfg.FLASHGG_DIPHOTON_INDEX = 1 + i;
        analyses_configs.push_back( new_cfg );
      }
    }

  // flashgg/MicroAOD/python/flashggPDFWeightObject_cfi.py
  void calc_flashgg_lhe_uncertanties(Float_t & PDF_up, Float_t & PDF_dn, Float_t & muR_up, Float_t & muR_dn, Float_t & muF_up, Float_t & muF_dn, Float_t & muRmuF_up, Float_t & muRmuF_dn, Float_t & alpha_s_dn, Float_t & alpha_s_up){
    std::vector<Float_t> * weights = & hzura::glob::event->event->flashgg_mc_weights;
    int N_weights = weights->size();

    int N_pdf_weights = weights->at( N_weights-2 );
    int N_alpha_weights = weights->at( N_weights-1 );
    int N_scale_weights = weights->at( N_weights-3 );

    // https://arxiv.org/pdf/1510.03865.pdf
    double PDF_nominal = 1.;
    double PDF_sum     = 0;
    double PDF_average = 0;
    for( int i = N_alpha_weights+N_scale_weights; i < weights->size() - 3 - N_alpha_weights; i++){
      PDF_sum     += TMath::Power( PDF_nominal - weights->at(i), 2 ) ;
      PDF_average += weights->at(i) ;
      // cout << i << " ->" << weights->at(i) << endl;
    }
    double PDF_error_hessian = TMath::Sqrt( PDF_sum );
    // msg( PDF_sum );

    PDF_average = PDF_average / N_pdf_weights;
    PDF_sum     = 0;
    for( int i = N_alpha_weights+N_scale_weights; i < weights->size()-3; i++)
      PDF_sum += TMath::Power( weights->at(i) - PDF_average , 2);
    double PDF_error_mc = TMath::Sqrt( PDF_sum / (N_pdf_weights - 1) );

    // msg(PDF_error_hessian, PDF_error_mc, PDF_average , PDF_sum );

    /*
    <weight id="1001"> muR=1 muF=1 hdamp=mt=272.7225 </weight>
    <weight id="1002"> muR=1 muF=2 hdamp=mt=272.7225 </weight>
    <weight id="1003"> muR=1 muF=0.5 hdamp=mt=272.7225 </weight>
    <weight id="1004"> muR=2 muF=1 hdamp=mt=272.7225 </weight>
    <weight id="1005"> muR=2 muF=2 hdamp=mt=272.7225 </weight>
    <weight id="1006"> muR=2 muF=0.5 hdamp=mt=272.7225 </weight>
    <weight id="1007"> muR=0.5 muF=1 hdamp=mt=272.7225 </weight>
    <weight id="1008"> muR=0.5 muF=2 hdamp=mt=272.7225 </weight>
    <weight id="1009"> muR=0.5 muF=0.5 hdamp=mt=272.7225 </weight>

    <weightgroup combine="hessian" name="NNPDF31_nnlo_as_0118_nf_4"> ----- default 
    <weight MUF="1" MUR="1" PDF="320900" id="474"> Member 0 of sets NNPDF31_nnlo_as_0118_nf_4</weight>
    */

    double muRmuF_nominal = weights->at( 0 );
    muR_up         = weights->at( 3 ) / muRmuF_nominal;
    muR_dn         = weights->at( 6 ) / muRmuF_nominal;
    muF_up         = weights->at( 1 ) / muRmuF_nominal;
    muF_dn         = weights->at( 2 ) / muRmuF_nominal;
    muRmuF_up      = weights->at( 4 ) / muRmuF_nominal;
    muRmuF_dn      = weights->at( 8 ) / muRmuF_nominal;

    alpha_s_dn = -1;
    alpha_s_up = -1;
    if(N_alpha_weights == 2){
      alpha_s_dn = weights->at(weights->size()-2 ) / muRmuF_nominal ;
      alpha_s_up = weights->at(weights->size()-1 ) / muRmuF_nominal ;
    } else {
      msg("calc_flashgg_lhe_uncertanties(): No alpha_S from flashgg lhe weight collection :C ... ");
    }

    PDF_up = PDF_nominal + PDF_error_hessian ;
    PDF_dn = PDF_nominal - PDF_error_hessian ;

    return;
    for(int i = 0; i < weights->size(); i++) cout << weights->at(i) << endl;
    msg( "N_weights, N_pdf_weights, N_alpha_weights, N_scale_weights = ", N_weights, N_pdf_weights, N_alpha_weights, N_scale_weights );
    msg( "PDF_nominal, PDF_up, PDF_dn = ", PDF_nominal, PDF_up, PDF_dn );
    msg( "muR_up, muR_dn, muF_up, muF_dn, muRmuF_up, muRmuF_dn = ", muR_up, muR_dn, muF_up, muF_dn, muRmuF_up, muRmuF_dn );

    const grinder::Event * event = hzura::glob::event->event;
    msg( "event->PdfXs1, event->PdfXs2, event->PdfQScale, event->PdfId1, event->PdfId2 = ", event->PdfXs1, event->PdfXs2, event->PdfQScale, event->PdfId1, event->PdfId2 );
    msg( "event->pdf1, event->pdf2 = ", event->pdf1, event->pdf2 );
    msg( "event->flashgg_weight = ", event->flashgg_weight );
    msg( "event->weight = ", event->weight );
    msg( "event->originalXWGTUP = ", event->originalXWGTUP );
  }

  void recalc_pdf_uncertanties( Float_t & PDF_central, Float_t & PDF_up, Float_t & PDF_dn, bool hessian=true ){
    // event->PdfXs1, event->PdfXs2
    // event->PdfQScale, 
    // event->PdfId1, event->PdfId2
    double pdfId1 = hzura::glob::event->event->PdfId1;
    double pdfX1  = hzura::glob::event->event->PdfXs1;
    double pdfQ   = hzura::glob::event->event->PdfQScale;
    double pdfId2 = hzura::glob::event->event->PdfId2;
    double pdfX2  = hzura::glob::event->event->PdfXs2;

    LHAPDF::PDF * pdf_gen = hzura::glob::PDFs_gen.at(0);

    vector<LHAPDF::PDF*> & pdfs = hzura::glob::PDFs_target;
    int N_pds                   = hzura::glob::PDFs_target_n_replics;

    double w0 = pdf_gen->xfxQ(pdfId1, pdfX1, pdfQ) * pdf_gen->xfxQ(pdfId2, pdfX2, pdfQ);
    
    double PDF_nominal = pdfs.at(0)->xfxQ(pdfId1, pdfX1, pdfQ) * pdfs.at(0)->xfxQ(pdfId2, pdfX2, pdfQ);
    double PDF_sum     = 0;
    double PDF_average = 0;
    for(int i = 0; i < N_pds; i++){
      double pdf_value = pdfs.at(i)->xfxQ(pdfId1, pdfX1, pdfQ) * pdfs.at(i)->xfxQ(pdfId2, pdfX2, pdfQ);
      PDF_sum     += TMath::Power( PDF_nominal - pdf_value, 2 ) ;
      PDF_average += pdf_value ;

      return;

      msg( "event->PdfXs1, event->PdfXs2, event->PdfQScale, event->PdfId1, event->PdfId2 = ", hzura::glob::event->event->PdfXs1, hzura::glob::event->event->PdfXs2, hzura::glob::event->event->PdfQScale, hzura::glob::event->event->PdfId1, hzura::glob::event->event->PdfId2 );
    }

    double PDF_error_hessian = TMath::Sqrt( PDF_sum );
    if(hessian){
      PDF_up = (PDF_nominal + PDF_error_hessian) / w0;
      PDF_dn = (PDF_nominal - PDF_error_hessian) / w0;
      PDF_central = PDF_nominal / w0;

      return;

      msg( "pdf1, pdf2 = ", pdf_gen->xfxQ(pdfId1, pdfX1, pdfQ), pdf_gen->xfxQ(pdfId2, pdfX2, pdfQ) );
      // msg( "PDF_sum, PDF_average, pdf_value = ", PDF_sum, PDF_average, pdf_value );
      msg( "PDF_central, PDF_nominal, PDF_up, PDF_dn, PDF_old = ", PDF_central, PDF_nominal, PDF_up, PDF_dn, w0 );
    }

    PDF_average = PDF_average / pdfs.size();
    PDF_sum     = 0;
    for(int i = 0; i < N_pds; i++){
      double pdf_value = pdfs.at(i)->xfxQ(pdfId1, pdfX1, pdfQ) * pdfs.at(i)->xfxQ(pdfId2, pdfX2, pdfQ);
      PDF_sum += TMath::Power( pdf_value - PDF_average , 2);
    }
    double PDF_error_mc = TMath::Sqrt( PDF_sum / (pdfs.size() - 1) );
    PDF_up = (PDF_nominal + PDF_error_mc) / w0 ;
    PDF_dn = (PDF_nominal - PDF_error_mc) / w0 ;

    return;
    msg( "PDF_nominal, PDF_up, PDF_dn = ", PDF_nominal, PDF_up, PDF_dn );
  }


};

#endif






