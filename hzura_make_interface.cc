
void hzura_make_interface( string input_file = string("grinder_out.root") ){

  TFile * file = TFile::Open( input_file.c_str() );
  gDirectory->cd("GrinderflashggHHWWggTag");

  TTree * t1 = (TTree*) gDirectory->Get("Events");
  TTree * t2 = (TTree*) gDirectory->Get("EventsMeta");

  for( TTree * t : {t1, t2} ){
    t->MakeClass();
    t->MakeProxy( (string("Proxy") + t->GetName()).c_str(), "dummu.c" );
  }
}


