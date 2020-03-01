
# wget --no-check-certificate --content-disposition https://raw.githubusercontent.com/pmandrik/GRINDER/master/interface/Event.hh
# mv Event.hh hzura_grinder_event.hh

wdir=`pwd`

### src files =================
wget --no-check-certificate -N --content-disposition https://raw.githubusercontent.com/pmandrik/PMANDRIK_LIBRARY/master/pmlib_msg.hh

### analyses files =================
mkdir data
cd data

### Muons SF
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs2016LegacyRereco
cd $wdir/data
mkdir muons_sf_2016; cd muons_sf_2016
wget --no-check-certificate -N --content-disposition https://gitlab.cern.ch/cms-muonPOG/MuonReferenceEfficiencies/raw/master/EfficienciesStudies/2016_legacy_rereco/systematic/RunBCDEF_SF_ID.root
wget --no-check-certificate -N --content-disposition https://gitlab.cern.ch/cms-muonPOG/MuonReferenceEfficiencies/raw/master/EfficienciesStudies/2016_legacy_rereco/systematic/RunBCDEF_SF_ISO.root
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs2017
cd $wdir/data
mkdir muons_sf_2017; cd muons_sf_2017
wget --no-check-certificate -N --content-disposition https://twiki.cern.ch/twiki/pub/CMS/MuonReferenceEffs2017/RunBCDEF_SF_ID_syst.root
wget --no-check-certificate -N --content-disposition https://twiki.cern.ch/twiki/pub/CMS/MuonReferenceEffs2017/RunBCDEF_SF_ISO_syst.root
# https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
cd $wdir/data
mkdir muons_sf_2018; cd muons_sf_2018
wget --no-check-certificate -N --content-disposition https://gitlab.cern.ch/cms-muonPOG/MuonReferenceEfficiencies/raw/master/EfficienciesStudies/2018/rootfiles/RunABCD_SF_ID.root
wget --no-check-certificate -N --content-disposition https://gitlab.cern.ch/cms-muonPOG/MuonReferenceEfficiencies/raw/master/EfficienciesStudies/2018/rootfiles/RunABCD_SF_ISO.root

cd $wdir
