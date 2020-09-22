
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.22.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh

runmode="$1"

run_anal(){
  input=$1
  output=$2
  type=$3

  # root -l -b -q "hzura_make_interface.cc(\""$input"\")"
  echo "gROOT->LoadMacro( \"hzura.cc\" ); main_root_wrapper(\"$input\", \"$output\", \"$type\", \"$runmode\");" | root -l -b 
}

outdir=../output_v7
postfix=v7

#run_anal /eos/user/p/pmandrik/HHWWgg/samples_v6/output_GluGluToHHTo_WWgg_qqlnu_nodeSM.root            $outdir/hzura_2017_SM_$postfix.root S
#run_anal /eos/user/p/pmandrik/HHWWgg/samples_v6/output_eft2.root                                      $outdir/hzura_2017_EFT2_$postfix.root S
#run_anal /eos/user/p/pmandrik/HHWWgg/samples_v6/output_eft9.root                                      $outdir/hzura_2017_EFT9_$postfix.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v6/output_DiPhotonJetsBox_MGG_80toInf_13TeV_Sherpa.root  $outdir/hzura_2017_ggjets_mgg80toInf_$postfix.root   B

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
