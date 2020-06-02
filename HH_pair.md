## flashgg microaod creation  
#### Preparation:
export SCRAM_ARCH=slc7_amd64_gcc700   
cmsrel CMSSW_10_6_2   
cd CMSSW_10_6_2/src  
cmsenv  
git cms-init  
git clone -b HHWWgg_Crab https://github.com/atishelmanch/flashgg  
source flashgg/setup_flashgg.sh  
scram b  
  
#### Minimal running command:  
cmsRun MicroAOD/test/microAODstd.py processType=data conditionsJSON=Era2017_09Aug2019_UL2017.json  

## flashgg run over microaod  
cmsRun ../Analysis/GRINDER/python/runCfg_flashgg.py 
