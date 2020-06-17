


file=/afs/cern.ch/work/p/pmandrik/dihiggs/2_cmssw_repack/CMSSW_10_2_18/src/Analysis/GRINDER/test/crab_test_run_data2016D_v4/results/grinder_out_39.root
file=grinder_out.root
file=/eos/user/p/pmandrik/HHWWgg/output_GluGluToHHTo_WWgg_qqlnu_node9_0.root

echo "gROOT->LoadMacro( \"hzura.cc\" ); main_root_wrapper(\""$file"\" );" | root -l -b 

