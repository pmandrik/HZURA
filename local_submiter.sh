
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.22.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh

export LHAPDF_DATA_PATH=/cvmfs/cms.cern.ch/slc6_amd64_gcc700/external/lhapdf/6.2.1/share/LHAPDF/

runmode="$1"

run_anal(){
  input=$1
  output=$2
  type=$3
  other_params=$4

  # root -l -b -q "hzura_make_interface.cc(\""$input"\")"
  echo "gInterpreter->AddIncludePath(\"-I/cvmfs/cms.cern.ch/slc7_amd64_gcc493/external/lhapdf/6.1.5/include/\"); gInterpreter->Load(\"/cvmfs/cms.cern.ch/slc7_amd64_gcc493/external/lhapdf/6.1.5/lib/libLHAPDF.so\"); gROOT->LoadMacro( \"hzura.cc\" ); main_root_wrapper(\"$input\", \"$output\", \"$type\", \"$runmode\", \"$other_params\");" | root -l -b 
}

#exit
#inpdir=/eos/user/p/pmandrik/HHWWgg/samples_2017_fnal_v0
#outdir=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_check
#mkdir -p $outdir
#postfix=v0
#run_anal $inpdir/output_data_2017.root                                 $outdir/data.root D "ERA=2017"
#exit

inpdir_2016=/eos/user/p/pmandrik/HHWWgg/samples_2016_fnal_v0
outdir_2016=/eos/user/p/pmandrik/HHWWgg_hzura/output_2016_v2_SYS # NEW PHOTON_ID & NEW REWEIGHTING & DY
mkdir -p $outdir_2016

inpdir_2017=/eos/user/p/pmandrik/HHWWgg/samples_2017_fnal_v2
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v2_SYS
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v2_nj1
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v5_SYS # NEW PHOTON_ID & NEW REWEIGHTING & DY
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v5_noSYS # EXTRA BENCHMARKS
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v5_noSYS_test # EXTRA BENCHMARKS
mkdir -p $outdir_2017

inpdir_2018=/eos/user/p/pmandrik/HHWWgg/samples_2018_fnal_v1
outdir_2018=/eos/user/p/pmandrik/HHWWgg_hzura/output_2018_v5_SYS   # NEW PHOTON_ID & NEW REWEIGHTING & DY
outdir_2018=/eos/user/p/pmandrik/HHWWgg_hzura/output_2018_v5_noSYS # ISR and FSR weights
mkdir -p $outdir_2018

postfix=v2
year=2016

# run 2016 Slo - Snlo - Bhh - B - D - WIP
# run 2017 Slo - Snlo - Bhh - B - D -
# run 2018 Slo - Snlo - Bhh - B - D -

# FIXME 2016 g+jets qcd

######################## 2016 ######################## 
if [ "$year" = "2016" ]; then   
  run_anal $inpdir_2016/cHHH0.root       $outdir_2016/cHHH0.root S "ERA=2016,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2016/cHHH2p45.root    $outdir_2016/cHHH2.root S "ERA=2016,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2016/cHHH5.root       $outdir_2016/cHHH5.root S "ERA=2016,GEN_PDF_HESSIAN=1"
  exit

  run_anal $inpdir_2016/cHHH1.root      $outdir_2016/cHHH1.root S "ERA=2016,GEN_PDF_HESSIAN=1,EFT_NODE=sm" 
  run_anal $inpdir_2016/sm.root         $outdir_2016/sm.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=sm"  
  run_anal $inpdir_2016/1.root          $outdir_2016/1.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=1"    
  run_anal $inpdir_2016/2.root          $outdir_2016/2.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=2"      
  run_anal $inpdir_2016/3.root          $outdir_2016/3.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=3"     
  run_anal $inpdir_2016/4.root          $outdir_2016/4.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=4"     
  run_anal $inpdir_2016/5.root          $outdir_2016/5.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=5"     
  run_anal $inpdir_2016/6.root          $outdir_2016/6.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=6"     
  run_anal $inpdir_2016/7.root          $outdir_2016/7.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=7"
  run_anal $inpdir_2016/8.root          $outdir_2016/7.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=7"     
  run_anal $inpdir_2016/9.root          $outdir_2016/9.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=9"      
  run_anal $inpdir_2016/10.root         $outdir_2016/10.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=10"      
  run_anal $inpdir_2016/11.root         $outdir_2016/11.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=11"      
  run_anal $inpdir_2016/12.root         $outdir_2016/12.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=12"

  exit
  run_anal $inpdir_2016/tth.root        $outdir_2016/ttH.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  
  run_anal $inpdir_2016/data_2016.root  $outdir_2016/data_2016.root D "ERA=2016"

  run_anal $inpdir_2016/ggh.root        $outdir_2016/ggh.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/tth.root        $outdir_2016/ttH.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vbf.root        $outdir_2016/vbf.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vh.root         $outdir_2016/vh.root    B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  run_anal $inpdir_2016/dy.root         $outdir_2016/dy.root          B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/zgjets.root     $outdir_2016/zgjets.root      B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/wgjets.root     $outdir_2016/wgjets.root      B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/ttjets.root     $outdir_2016/ttjets.root      B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/ttgjets.root    $outdir_2016/ttgjets.root     B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/ttgg.root       $outdir_2016/ttgg.root        B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/ggjets.root     $outdir_2016/ggjets.root      B "ERA=2016,NOSYS=True"
  run_anal $inpdir_2016/gjets.root      $outdir_2016/gjets.root        B "ERA=2016,NOSYS=True"

  exit
  run_anal $inpdir_2016/$2.root         $outdir_2016/$2.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=$2"  
  exit
fi;

######################## 2017 ######################## 
if [ "$year" = "2017" ]; then 
  run_anal $inpdir_2017/1.root         $outdir_2017/1.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=1"    

  exit
  run_anal $inpdir_2017/1.root         $outdir_2017/1.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=1"    
  run_anal $inpdir_2017/2.root         $outdir_2017/2.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=2"      
  run_anal $inpdir_2017/3.root         $outdir_2017/3.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=3"     
  run_anal $inpdir_2017/4.root         $outdir_2017/4.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=4"     
  run_anal $inpdir_2017/5.root         $outdir_2017/5.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=5"     
  run_anal $inpdir_2017/6.root         $outdir_2017/6.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=6"     
  run_anal $inpdir_2017/7.root         $outdir_2017/7.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=7"     
  run_anal $inpdir_2017/9.root         $outdir_2017/9.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=9"      
  run_anal $inpdir_2017/10.root        $outdir_2017/10.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=10"      
  run_anal $inpdir_2017/11.root        $outdir_2017/11.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=11"      
  run_anal $inpdir_2017/12.root        $outdir_2017/12.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=12"

  run_anal $inpdir_2017/ggh.root      $outdir_2017/ggh.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/ttH.root      $outdir_2017/ttH.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vbf.root      $outdir_2017/vbf.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vh.root       $outdir_2017/vh.root    B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  run_anal $inpdir_2017/data_2017_re0.root $outdir_2017/data_2017.root     D "ERA=2017"

  run_anal $inpdir_2017/dy.root      $outdir_2017/dy.root             B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/qcd.root     $outdir_2017/qcd.root            B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/wgg.root     $outdir_2017/wgg.root            B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/zgjets.root  $outdir_2017/zgjets.root      B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/wgjets.root  $outdir_2017/wgjets.root      B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/ttjets.root  $outdir_2017/ttjets.root      B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/ttgjets.root $outdir_2017/ttgjets.root     B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/ttgg.root    $outdir_2017/ttgg.root        B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/ggjets.root     $outdir_2017/ggjets.root   B "ERA=2017,NOSYS=True"
  run_anal $inpdir_2017/gjets.root      $outdir_2017/gjets.root    B "ERA=2017,NOSYS=True"

  exit
  run_anal $inpdir_2017/cHHH0.root       $outdir_2017/cHHH0.root S "ERA=2017,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2017/cHHH1.root       $outdir_2017/cHHH1.root S "ERA=2017,GEN_PDF_HESSIAN=1,EFT_NODE=sm" 
  run_anal $inpdir_2017/cHHH2.root       $outdir_2017/cHHH2.root S "ERA=2017,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2017/cHHH5.root       $outdir_2017/cHHH5.root S "ERA=2017,GEN_PDF_HESSIAN=1" 

  run_anal $inpdir_2017/sm.root        $outdir_2017/sm.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=sm"  


  exit

  run_anal $inpdir_2017/$2.root          $outdir_2017/$2.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=$2"  
  exit

  run_anal $inpdir_2017/data_2017_re0.root $outdir_2017/data_2017_plusF.root     D "ERA=2017"
  run_anal $inpdir_2017/data_2017.root  $outdir_2017/data_2017.root              D "ERA=2017"
fi;

######################## 2018 ######################## 
if [ "$year" = "2018" ]; then 
  # /eos/user/p/pmandrik/HHWWgg/samples_2018_fnal_v1
  # /eos/user/p/pmandrik/HHWWgg_hzura/output_2018_v5_SYS
  inpdir=$inpdir_2018
  outdir=$outdir_2018

  run_anal $inpdir/SM.root          $outdir/sm.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=sm"   
  run_anal $inpdir/10.root          $outdir/10.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=10"   
  exit

  run_anal $inpdir/1.root           $outdir/1.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=1"  
  run_anal $inpdir/2.root           $outdir/2.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=2"  
  run_anal $inpdir/3.root           $outdir/3.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=3"  
  run_anal $inpdir/4.root           $outdir/4.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=4"   
  run_anal $inpdir/5.root           $outdir/5.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=5"  
  run_anal $inpdir/6.root           $outdir/6.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=6"   
  run_anal $inpdir/7.root           $outdir/7.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=7"   
  run_anal $inpdir/8.root           $outdir/8.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=8"   
  run_anal $inpdir/9.root           $outdir/9.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=9" 
  run_anal $inpdir/10.root          $outdir/10.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=10"   
  run_anal $inpdir/11.root          $outdir/11.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=11"   
  run_anal $inpdir/12.root          $outdir/12.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=12"

  run_anal $inpdir/cHHH0.root       $outdir_2018/cHHH0.root S "ERA=2018,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH1.root       $outdir_2018/cHHH1.root S "ERA=2018,GEN_PDF_HESSIAN=1,EFT_NODE=sm" 
  run_anal $inpdir/cHHH2.root       $outdir_2018/cHHH2.root S "ERA=2018,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH5.root       $outdir_2018/cHHH5.root S "ERA=2018,GEN_PDF_HESSIAN=1" 

  run_anal $inpdir/ggh.root         $outdir_2018/ggh.root       B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/vh.root          $outdir_2018/vh.root        B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/vbf.root         $outdir_2018/vbf.root      B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/tth.root         $outdir_2018/ttH.root       B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  run_anal $inpdir_2018/zg.root     $outdir_2018/zgjets.root      B "ERA=2018"
  run_anal $inpdir_2018/wg.root     $outdir_2018/wgjets.root      B "ERA=2018"
  run_anal $inpdir_2018/tt.root     $outdir_2018/ttjets.root      B "ERA=2018"
  run_anal $inpdir_2018/ttg.root    $outdir_2018/ttgjets.root     B "ERA=2018"
  run_anal $inpdir_2018/ttgg.root   $outdir_2018/ttgg.root        B "ERA=2018"
  run_anal $inpdir_2018/ggjets.root $outdir_2018/ggjets.root      B "ERA=2018"
  run_anal $inpdir_2018/gjets.root  $outdir_2018/gjets.root       B "ERA=2018"
  run_anal $inpdir_2018/dy.root     $outdir_2018/dy.root         B "ERA=2017,NOSYS=True"
  exit

  run_anal $inpdir/data.root        $outdir/data_2018.root      D "ERA=2018"
  exit

  run_anal $inpdir/$2.root          $outdir/$2.root S "GEN_PDF_HESSIAN=0,EFT_NODE=$2"
  exit
fi;


