#!/bin/bash

#sed on Linux and OS X have different command line options
case `uname` in Darwin) SED_OPT="-i '' -E";;*) SED_OPT="-i -r";; esac ;

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u16_errors.mmlog u16_altWarnings.log u16_infos.mmlog u16_altDebugs.mmlog u16_default.log u16_job_report.mmxml u16_statistics.mslog 

cmsRun -e -j u16_job_report.mmxml -p $LOCAL_TEST_DIR/u16_cfg.py || exit $?
 
for file in u16_errors.mmlog u16_altWarnings.log u16_infos.mmlog u16_altDebugs.mmlog u16_default.log u16_job_report.mmxml u16_statistics.mslog  
do
  sed $SED_OPT -f $LOCAL_TEST_DIR/filter-timestamps.sed $file
  diff $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file  
  if [ $? -ne 0 ]  
  then
    echo The above discrepancies concern $file 
    status=1
  fi
done

popd

exit $status
