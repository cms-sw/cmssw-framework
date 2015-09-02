#!/bin/bash
# ~cmssw2/scritps/setup.sh
source /nfshome0/cmssw/setup/group_aliases.sh;
export VO_CMS_SW_DIR="/nfshome0/cmssw2";
export SCRAM_ARCH=slc5_amd64_gcc434
source /nfshome0/cmssw2/cmsset_default.sh;
export PATH="/nfshome0/cmssw2/scripts:${PATH}";

cd /nfshome0/hcallumipro/LumiDBUtil/exec/CMSSW_4_2_3/src/
eval `scramv1 runtime -sh`
export TNS_ADMIN=/home/lumidb

dbConnectionString="oracle://cms_orcon_prod/cms_lumi_prod"
dropboxDir="/dropbox/hcallumipro/"
lumiauthpath="/home/lumidb/auth/writer"
lumilogpath="/home/lumidb/log"
loaderconf="loader.cfg"
minrun=180250

cd /nfshome0/hcallumipro/LumiDBUtil/exec
date > "$lumilogpath/tmp.log"
sqlplus cms_runinfo_r@cms_orcon_prod/$1 < dumpfill.sql >> "$lumilogpath/tmp.log"
lumiDBFiller.py -c $dbConnectionString -d $dropboxDir -P $lumiauthpath -L $lumilogpath -f $loaderconf --minrun $minrun >> "$lumilogpath/tmp.log"
date >> "$lumilogpath/tmp.log"
myDate=`date +"%y-%m-%d-%H"`
mv "$lumilogpath/tmp.log" "$lumilogpath/lumiDBFiller-$myDate.log"
