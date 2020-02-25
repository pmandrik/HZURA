


file=/afs/cern.ch/work/p/pmandrik/dihiggs/2_cmssw_repack/CMSSW_10_2_18/src/Analysis/GRINDER/test/crab_test_run_data2016D_v4/results/grinder_out_39.root

echo "gROOT->LoadMacro( \"hzura.cc\" ); main_root_wrapper(\""$file"\" );" | root -l -b 

