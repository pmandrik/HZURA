
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.22.00/x86_64-centos7-gcc48-opt/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh

run_anal(){
  input=$1
  output=$2
  type=$3

  # root -l -b -q "hzura_make_interface.cc(\""$input"\")"
  echo "gROOT->LoadMacro( \"hzura.cc\" ); main_root_wrapper(\"$input\", \"$output\", \"$type\");" | root -l -b 
}

run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft2.root                                                   ../output/hzura_2017_EFT2_v4.root S
exit
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_eft9.root                                                   ../output/hzura_2017_EFT9_v4.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v4/output_data_2017.root                                              ../output/hzura_2017_data_v4.root D
exit
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_eft2.root                                                   ../output/hzura_2017_EFT2_v3.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_eft9.root                                                   ../output/hzura_2017_EFT9_v3.root S
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/data_2017.root                                                     ../output/hzura_2017_data_v3.root               D
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToLL.root                                       ../output/hzura_2017_ggZH_HToGG_ZToLL_v3.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToNuNu.root                                     ../output/hzura_2017_ggZH_HToGG_ZToNuNu_v3.root B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_ggZH_HToGG_ZToQQ.root                                       ../output/hzura_2017_ggZH_HToGG_ZToQQ_v3.root   B
run_anal /eos/user/p/pmandrik/HHWWgg/samples_v3/output_GluGluHToGG.root                                            ../output/hzura_2017_GluGluHToGG_v3.root        B
