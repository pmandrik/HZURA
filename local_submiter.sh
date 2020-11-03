
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
#run_anal $inpdir/output_data_2017.root                                 $outdir/hzura_2017_data_$postfix.root D "ERA=2017"
#exit
#

inpdir_2016=/eos/user/p/pmandrik/HHWWgg/samples_2016_fnal_v0
outdir_2016=/eos/user/p/pmandrik/HHWWgg_hzura/output_2016_v2_SYS
mkdir -p $outdir_2016

inpdir_2017=/eos/user/p/pmandrik/HHWWgg/samples_2017_fnal_v2
outdir_2017=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_v2_SYS
mkdir -p $outdir_2017

inpdir_2018=/eos/user/p/pmandrik/HHWWgg/samples_2018_fnal_v1
outdir_2018=/eos/user/p/pmandrik/HHWWgg_hzura/output_2018_v3_noSYS
mkdir -p $outdir_2018

postfix=v2
year=2018

######################## 2016 ######################## 
if [ "$year" = "2016" ]; then 
  run_anal $inpdir_2016/tth.root      $outdir_2016/ttH_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  exit

  #run_anal $inpdir_2016/sm.root          $outdir_2017/test.root S #"ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=$2,PRINT_WEIGHTS=1"  
  #exit

  run_anal $inpdir_2016/cHHH0.root       $outdir_2016/hzura_cHHH0_$postfix.root S "ERA=2016,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2016/cHHH2p45.root    $outdir_2016/hzura_cHHH2_$postfix.root S "ERA=2016,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir_2016/cHHH5.root       $outdir_2016/hzura_cHHH5_$postfix.root S "ERA=2016,GEN_PDF_HESSIAN=1" 
  exit

  run_anal $inpdir_2016/$2.root          $outdir_2016/hzura_$2_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=$2"  
  exit

  run_anal $inpdir_2016/vh.root       $outdir_2016/vh_2017_$postfix.root    B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/ggh.root      $outdir_2016/ggh_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vbf.root      $outdir_2016/vbf_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vh.root       $outdir_2016/vh_2017_$postfix.root    B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  run_anal $inpdir_2016/ggjets.root     $outdir_2016/ggjets_$postfix.root    B "ERA=2016"

  run_anal $inpdir_2016/data_2016.root  $outdir_2016/data_2016_$postfix.root D "ERA=2016"

  exit
  run_anal $inpdir_2016/sm.root          $outdir_2016/hzura_sm_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=sm"  
  run_anal $inpdir_2016/1.root          $outdir_2016/hzura_1_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=1"    
  run_anal $inpdir_2016/2.root          $outdir_2016/hzura_2_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=2"      
  run_anal $inpdir_2016/3.root          $outdir_2016/hzura_3_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=3"     
  run_anal $inpdir_2016/4.root          $outdir_2016/hzura_4_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=4"     
  run_anal $inpdir_2016/5.root          $outdir_2016/hzura_5_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=5"     
  run_anal $inpdir_2016/6.root          $outdir_2016/hzura_6_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=6"     
  run_anal $inpdir_2016/7.root          $outdir_2016/hzura_7_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=7"     
  run_anal $inpdir_2016/8.root          $outdir_2016/hzura_7_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=7"     
  run_anal $inpdir_2016/9.root          $outdir_2016/hzura_9_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=9"      
  run_anal $inpdir_2016/10.root        $outdir_2016/hzura_10_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=10"      
  run_anal $inpdir_2016/11.root        $outdir_2016/hzura_11_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=11"      
  run_anal $inpdir_2016/12.root        $outdir_2016/hzura_12_$postfix.root S "ERA=2016,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF30_nlo_nf_4_pdfas,MADGRAPH=True,RECALC_ALPHA_S=True,EFT_NODE=12"   

  run_anal $inpdir_2016/ggh.root      $outdir_2016/ggh_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/ttH.root      $outdir_2016/ttH_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vbf.root      $outdir_2016/vbf_2017_$postfix.root   B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2016/vh.root       $outdir_2016/vh_2017_$postfix.root    B "ERA=2016,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  exit

  run_anal $inpdir_2016/ggjets.root     $outdir_2016/ggjets_$postfix.root   B "ERA=2016"

  run_anal $inpdir_2016/data_2016.root  $outdir_2016/data_2016_$postfix.root     D "ERA=2016"
  exit
fi;

######################## 2017 ######################## 
if [ "$year" = "2017" ]; then 
  run_anal $inpdir_2017/$2.root          $outdir_2017/hzura_2017_$2_fsim_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=$2"  
  exit

  run_anal $inpdir_2017/ggh.root      $outdir_2017/ggh_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/ttH.root      $outdir_2017/ttH_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vbf.root      $outdir_2017/vbf_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vh.root       $outdir_2017/vh_2017_$postfix.root    B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  exit

  run_anal $inpdir_2017/sm.root          $outdir_2017/hzura_2017_sm_fsim_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=sm"  
  run_anal $inpdir_2017/1.root          $outdir_2017/hzura_2017_1_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=1"    
  run_anal $inpdir_2017/2.root          $outdir_2017/hzura_2017_2_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=2"      
  run_anal $inpdir_2017/3.root          $outdir_2017/hzura_2017_3_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=3"     
  run_anal $inpdir_2017/4.root          $outdir_2017/hzura_2017_4_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=4"     
  run_anal $inpdir_2017/5.root          $outdir_2017/hzura_2017_5_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=5"     
  run_anal $inpdir_2017/6.root          $outdir_2017/hzura_2017_6_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=6"     
  run_anal $inpdir_2017/7.root          $outdir_2017/hzura_2017_7_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=7"     
  run_anal $inpdir_2017/9.root          $outdir_2017/hzura_2017_9_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=9"      
  run_anal $inpdir_2017/10.root        $outdir_2017/hzura_2017_10_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=10"      
  run_anal $inpdir_2017/11.root        $outdir_2017/hzura_2017_11_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=11"      
  run_anal $inpdir_2017/12.root        $outdir_2017/hzura_2017_12_$postfix.root S "ERA=2017,TARGET_PDF=NNPDF30_nlo_nf_4_pdfas,TARGET_PDF_SETSIZE=100,TARGET_PDF_HESSIAN=False,GEN_PDF=NNPDF31_nnlo_as_0118_nf_4,RECALC_ALPHA_S=True,EFT_NODE=12"   

  run_anal $inpdir_2017/data_2017.root  $outdir_2017/data_2017_$postfix.root     D "ERA=2017"
  run_anal $inpdir_2017/ggjets.root     $outdir_2017/ggjets_2017_$postfix.root   B "ERA=2017"
  run_anal $inpdir_2017/gjets.root      $outdir_2017/gjets_2017_$postfix.root    B "ERA=2017"

  run_anal $inpdir_2017/ggh.root      $outdir_2017/ggh_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/ttH.root      $outdir_2017/ttH_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vbf.root      $outdir_2017/vbf_2017_$postfix.root   B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir_2017/vh.root       $outdir_2017/vh_2017_$postfix.root    B "ERA=2017,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  exit


  run_anal $inpdir_2017/sm.root          $outdir_2018/hzura_2017_SM_fsim_$postfix.root S "GEN_PDF_HESSIAN=0,ERA=2017"  
  run_anal $inpdir_2018/sm.root          $outdir_2018/hzura_2018_SM_fsim_$postfix.root S "GEN_PDF_HESSIAN=0,ERA=2018"  
  exit

  run_anal $inpdir_2017/data_2017.root                                                            $outdir_2017/data_2017_$postfix.root      D "ERA=2017"
  run_anal $inpdir_2018/data_2018.root                                                            $outdir_2018/data_2018_$postfix.root      D "ERA=2018"
  exit
fi;

######################## 2018 ######################## 
if [ "$year" = "2018" ]; then 
  inpdir=$inpdir_2018
  outdir=$outdir_2018
  run_anal $inpdir/SM.root          $outdir/hzura_2018_sm_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=sm"   
  run_anal $inpdir/1.root           $outdir/hzura_2018_1_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=1"  
  run_anal $inpdir/2.root           $outdir/hzura_2018_2_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=2"  
  run_anal $inpdir/3.root           $outdir/hzura_2018_3_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=3"  
  run_anal $inpdir/4.root           $outdir/hzura_2018_4_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=4"   
  run_anal $inpdir/5.root           $outdir/hzura_2018_5_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=5"  
  run_anal $inpdir/6.root           $outdir/hzura_2018_6_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=6"   
  run_anal $inpdir/7.root           $outdir/hzura_2018_7_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=7"   
  run_anal $inpdir/8.root           $outdir/hzura_2018_8_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=8"   
  run_anal $inpdir/9.root           $outdir/hzura_2018_9_fsim_$postfix.root  S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=9" 
  run_anal $inpdir/10.root          $outdir/hzura_2018_10_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=10"   
  run_anal $inpdir/11.root          $outdir/hzura_2018_11_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=11"   
  run_anal $inpdir/12.root          $outdir/hzura_2018_12_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=0,EFT_NODE=12"

  run_anal $inpdir/cHHH0.root       $outdir/hzura_2018_cHHH0_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH1.root       $outdir/hzura_2018_cHHH1_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=1,EFT_NODE=sm" 
  run_anal $inpdir/cHHH2.root       $outdir/hzura_2018_cHHH2_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH5.root       $outdir/hzura_2018_cHHH5_fsim_$postfix.root S "ERA=2018,GEN_PDF_HESSIAN=1" 

  run_anal $inpdir/ggh.root         $outdir/2018_ggH_$postfix.root       B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/vh.root          $outdir/2018_VH_$postfix.root        B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/vbf.root         $outdir/2018_VBFH_$postfix.root      B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/tth.root         $outdir/2018_ttH_$postfix.root       B "ERA=2018,TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"

  run_anal $inpdir/ggjets.root      $outdir/2018_ggjets_$postfix.root    B "ERA=2018"
  run_anal $inpdir/gjets.root       $outdir/2018_gjets1_$postfix.root    B "ERA=2018"

  run_anal $inpdir/data.root        $outdir/data_2018_$postfix.root      D "ERA=2018"

  exit # OLD RELEASE

  run_anal $inpdir/data_2018.root                                                            $outdir/data_2018_$postfix.root      D "ERA=2018"
  exit

  run_anal $inpdir/cHHH0.root       $outdir/hzura_2018_cHHH0_fsim_$postfix.root S "GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH1.root       $outdir/hzura_2018_cHHH1_fsim_$postfix.root S "GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH2.root       $outdir/hzura_2018_cHHH2_fsim_$postfix.root S "GEN_PDF_HESSIAN=1" 
  run_anal $inpdir/cHHH5.root       $outdir/hzura_2018_cHHH5_fsim_$postfix.root S "GEN_PDF_HESSIAN=1" 
  exit

  run_anal $inpdir/GluGluHToGG_M125_TuneCP5_13TeV-amcatnloFXFX-pythia8.root                  $outdir/2018_ggH_$postfix.root       B "TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root                       $outdir/2018_VH_$postfix.root        B "TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/VBFHToGG_M125_13TeV_amcatnlo_pythia8.root                                 $outdir/2018_VBFH_$postfix.root      B "TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  run_anal $inpdir/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root                   $outdir/2018_ttH_$postfix.root       B "TARGET_PDF=NNPDF31_nnlo_hessian_pdfas,TARGET_PDF_HESSIAN=True,TARGET_PDF_SETSIZE=100,GEN_PDF=NNPDF31_nnlo_hessian_pdfas"
  exit

  run_anal $inpdir/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.root                             $outdir/2018_ggjets_$postfix.root    B
  run_anal $inpdir/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root    $outdir/2018_gjets1_$postfix.root    B
  run_anal $inpdir/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root   $outdir/2018_gjets2_$postfix.root    B
  exit

  run_anal $inpdir/data_2018.root                                                            $outdir/data_2018_$postfix.root      D
  exit

  run_anal $inpdir/$2.root          $outdir/hzura_2018_$2_fsim_$postfix.root S "GEN_PDF_HESSIAN=0,EFT_NODE=$2"
  exit

  run_anal $inpdir/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa.root                             $outdir/2018_ggjets_$postfix.root    B
  run_anal $inpdir/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root    $outdir/2018_gjets1_$postfix.root    B
  run_anal $inpdir/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root   $outdir/2018_gjets2_$postfix.root    B
  exit

  run_anal $inpdir/sm.root          $outdir/hzura_2018_SM_fsim_$postfix.root S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/1.root           $outdir/hzura_2018_1_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/2.root           $outdir/hzura_2018_2_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/3.root           $outdir/hzura_2018_3_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/4.root           $outdir/hzura_2018_4_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/5.root           $outdir/hzura_2018_5_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/6.root           $outdir/hzura_2018_6_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/7.root           $outdir/hzura_2018_7_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/8.root           $outdir/hzura_2018_8_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/9.root           $outdir/hzura_2018_9_fsim_$postfix.root  S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/10.root          $outdir/hzura_2018_10_fsim_$postfix.root S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/11.root          $outdir/hzura_2018_11_fsim_$postfix.root S "GEN_PDF_HESSIAN=0"   
  run_anal $inpdir/12.root          $outdir/hzura_2018_12_fsim_$postfix.root S "GEN_PDF_HESSIAN=0" 
  exit
fi;

exit
######################## 2017 ######################## 
inpdir=/eos/user/p/pmandrik/HHWWgg/samples_2017_fnal_v0
outdir=/eos/user/p/pmandrik/HHWWgg_hzura/output_2017_c2
mkdir -p $outdir
postfix=v0

run_anal $inpdir/output_GluGluHToGG.root                                         $outdir/hzura_2017_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_$postfix.root          B                  
run_anal $inpdir/output_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root  $outdir/hzura_2017_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_$postfix.root   B
run_anal $inpdir/output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root                $outdir/hzura_2017_VBFHToGG_M125_13TeV_amcatnlo_pythia8_$postfix.root                 B
run_anal $inpdir/output_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root      $outdir/hzura_2017_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_$postfix.root       B

exit
run_anal $inpdir/$2.root          $outdir/hzura_2017_$2_fsim_$postfix.root S

exit
run_anal $inpdir/SM.root          $outdir/hzura_2017_SM_fsim_$postfix.root S
run_anal $inpdir/1.root           $outdir/hzura_2017_1_fsim_$postfix.root S
run_anal $inpdir/2.root           $outdir/hzura_2017_2_fsim_$postfix.root S
run_anal $inpdir/3.root           $outdir/hzura_2017_3_fsim_$postfix.root S
run_anal $inpdir/4.root           $outdir/hzura_2017_4_fsim_$postfix.root S
run_anal $inpdir/5.root           $outdir/hzura_2017_5_fsim_$postfix.root S
run_anal $inpdir/6.root           $outdir/hzura_2017_6_fsim_$postfix.root S
run_anal $inpdir/7.root           $outdir/hzura_2017_7_fsim_$postfix.root S
# run_anal $inpdir/8.root           $outdir/hzura_2017_8_fsim_$postfix.root S
run_anal $inpdir/9.root           $outdir/hzura_2017_9_fsim_$postfix.root S
run_anal $inpdir/10.root          $outdir/hzura_2017_10_fsim_$postfix.root S
run_anal $inpdir/11.root          $outdir/hzura_2017_11_fsim_$postfix.root S
run_anal $inpdir/12.root          $outdir/hzura_2017_12_fsim_$postfix.root S
exit

run_anal $inpdir/output_sm_fsim.root                                   $outdir/hzura_2017_SM_fsim_$postfix.root S
run_anal $inpdir/output_eft2.root                                      $outdir/hzura_2017_EFT2_$postfix.root S
run_anal $inpdir/output_eft9.root                                      $outdir/hzura_2017_EFT9_$postfix.root S

run_anal $inpdir/output_data_2017.root                                 $outdir/hzura_2017_data_$postfix.root D

run_anal $inpdir/output_GluGluHToGG.root                                         $outdir/hzura_2017_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_$postfix.root          B                       
run_anal $inpdir/output_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root  $outdir/hzura_2017_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_$postfix.root   B
run_anal $inpdir/output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root                $outdir/hzura_2017_VBFHToGG_M125_13TeV_amcatnlo_pythia8_$postfix.root                 B
run_anal $inpdir/output_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root      $outdir/hzura_2017_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_$postfix.root       B

run_anal $inpdir/output_DiPhotonJetsBox_MGG_80toInf_13TeV_Sherpa.root                            $outdir/hzura_2017_ggjets_mgg80toInf_$postfix.root   BL
run_anal $inpdir/output_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root   $outdir/hzura_2017_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_$postfix.root   BL
run_anal $inpdir/output_GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root  $outdir/hzura_2017_GJet_Pt_40toInf_DoubleEMEnriched_MGG_80toInf_$postfix.root   BL

exit
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v5/output_GluGluToHHTo_WWgg_qqlnu_nodeSM.root       $outdir/hzura_2017_SM_v6.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ggjets_mgg80toInf.root                                      $outdir/hzura_2017_ggjets_mgg80toInf_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_WGToLNuG_01J.root $outdir/hzura_2017_WGToLNuG_01J_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ZGToLLG_01J.root  $outdir/hzura_2017_ZGToLLG_01J_v6.root    B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_QCD_Pt_30to40_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root  $outdir/hzura_2017_QCD_Pt_30to40_DoubleEMEnriched_MGG_80toInf_v6.root    B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_QCD_Pt_40toInf_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root $outdir/hzura_2017_QCD_Pt_40toInf_DoubleEMEnriched_MGG_80toInf_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root $outdir/hzura_2017_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root                $outdir/hzura_2017_TTGJets_v6.root      B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root                 $outdir/hzura_2017_TTGG_0Jets_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ggjets_mgg80toInf.root                                      $outdir/hzura_2017_ggjets_mgg80toInf_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_gjet_mgg80toInf.root                                        $outdir/hzura_2017_gjet_mgg80toInf_v6.root     B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft2.root                                                   $outdir/hzura_2017_EFT2_v6.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft9.root                                                   $outdir/hzura_2017_EFT9_v6.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_data_2017.root                                              $outdir/hzura_2017_data_v6.root D
### SMALL CONTRIBUTIONS
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_DYJetsToLL_M_50_TuneCP5_13TeV_amcatnloFXFX_pythia8.root $outdir/hzura_2017_DYJetsToLL_M_50_TuneCP5_13TeV_amcatnloFXFX_pythia8_v6.root    B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTJets_TuneCP5_13TeV_amcatnloFXFX_pythia8.root           $outdir/hzura_2017_TTJets_TuneCP5_13TeV_amcatnloFXFX_pythia8_v6.root            B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root         $outdir/hzura_2017_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_v6.root          B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root  $outdir/hzura_2017_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v6.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root                $outdir/hzura_2017_VBFHToGG_M125_13TeV_amcatnlo_pythia8_v6.root                 B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root      $outdir/hzura_2017_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v6.root       B


exit
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ggjets_mgg80toInf.root                                      $outdir/hzura_2017_ggjets_mgg80toInf_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_WGToLNuG_01J.root $outdir/hzura_2017_WGToLNuG_01J_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ZGToLLG_01J.root  $outdir/hzura_2017_ZGToLLG_01J_v5.root    B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_QCD_Pt_30to40_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root  $outdir/hzura_2017_QCD_Pt_30to40_DoubleEMEnriched_MGG_80toInf_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_QCD_Pt_40toInf_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root $outdir/hzura_2017_QCD_Pt_40toInf_DoubleEMEnriched_MGG_80toInf_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_TuneCP5_13TeV_Pythia8.root $outdir/hzura_2017_GJet_Pt_20to40_DoubleEMEnriched_MGG_80toInf_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root                $outdir/hzura_2017_TTGJets_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root                 $outdir/hzura_2017_TTGG_0Jets_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ggjets_mgg80toInf.root                                      $outdir/hzura_2017_ggjets_mgg80toInf_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_gjet_mgg80toInf.root                                        $outdir/hzura_2017_gjet_mgg80toInf_v5.root     B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft2.root                                                   $outdir/hzura_2017_EFT2_v5.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft9.root                                                   $outdir/hzura_2017_EFT9_v5.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_data_2017.root                                              $outdir/hzura_2017_data_v5.root D
### SMALL CONTRIBUTIONS
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_DYJetsToLL_M_50_TuneCP5_13TeV_amcatnloFXFX_pythia8.root $outdir/hzura_2017_DYJetsToLL_M_50_TuneCP5_13TeV_amcatnloFXFX_pythia8_v5.root    B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_TTJets_TuneCP5_13TeV_amcatnloFXFX_pythia8.root           $outdir/hzura_2017_TTJets_TuneCP5_13TeV_amcatnloFXFX_pythia8_v5.root            B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root         $outdir/hzura_2017_GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_v5.root          B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root  $outdir/hzura_2017_ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v5.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_VBFHToGG_M125_13TeV_amcatnlo_pythia8.root                $outdir/hzura_2017_VBFHToGG_M125_13TeV_amcatnlo_pythia8_v5.root                 B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root      $outdir/hzura_2017_VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v5.root       B

exit
### OLD
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_eft2.root                                                   $outdir/hzura_2017_EFT2_v3.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_eft9.root                                                   $outdir/hzura_2017_EFT9_v3.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/data_2017.root                                                     $outdir/hzura_2017_data_v3.root               D
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToLL.root                                       $outdir/hzura_2017_ggZH_HToGG_ZToLL_v3.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToNuNu.root                                     $outdir/hzura_2017_ggZH_HToGG_ZToNuNu_v3.root B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToQQ.root                                       $outdir/hzura_2017_ggZH_HToGG_ZToQQ_v3.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_GluGluHToGG.root                                            $outdir/hzura_2017_GluGluHToGG_v3.root        B
