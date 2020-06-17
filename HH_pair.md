## flashgg def microaod creation  
#### Preparation:  
#### Minimal running command:  
## flashgg def run over microaod  
#### Preparation:  
#### Minimal running command:  

## flashgg UL microaod creation  
#### Preparation:
follow https://github.com/cms-analysis/flashgg  
  
#### Minimal running command:  
cmsRun MicroAOD/test/microAODstd.py processType=data conditionsJSON=Era2017_09Aug2019_UL2017.json datasetName=DoubleEG  
cmsRun MicroAOD/test/microAODstd.py processType=bkg conditionsJSON=Era2017_09Aug2019_UL2017.json  

## flashgg UL run over microaod  
#### Preparation:  
export SCRAM_ARCH=slc7_amd64_gcc700   
cmsrel CMSSW_10_6_2   
cd CMSSW_10_6_2/src  
cmsenv  
git cms-init  
mkdir Analysis; cd Analysis  
git clone -b flashgg git@github.com:pmandrik/GRINDER.git  
cd $CMSSW_BASE/src  
git clone -b HHWWgg_dev git@github.com:pmandrik/flashgg.git
source flashgg/setup_flashgg.sh  
scram b
#### Minimal running command:  
cmsRun ../Analysis/GRINDER/python/runCfg_flashgg.py metaConditions=Era2017_09Aug2019_UL2017.json  
cmsRun ../Analysis/GRINDER/python/runCfg_flashgg.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json
