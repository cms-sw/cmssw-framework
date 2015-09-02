import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMStore_cfg import *
from CondCore.DBCommon.CondDBSetup_cfi import *

#CondDBSetup.DBParameters.authenticationPath = cms.untracked.string('/build/diguida/conddb')

ReferenceRetrieval = cms.ESSource("PoolDBESSource",
                                  CondDBSetup,
                                  connect = cms.string('sqlite_file:ROOTFILE_Test.db'),
                                  BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
                                  messageLevel = cms.untracked.int32(1), #3 for high verbosity
                                  timetype = cms.string('runnumber'),
                                  toGet = cms.VPSet(cms.PSet(record = cms.string('DQMReferenceHistogramRootFileRcd'),
                                                             tag = cms.string('ROOTFILE_Test')
                                                             )
                                                    )
                                  )

## RecordDataGetter = cms.EDAnalyzer("EventSetupRecordDataGetter",
##                              toGet = cms.VPSet(cms.PSet(record = cms.string('DQMReferenceHistogramRootFileRcd'),
##                                                         data = cms.vstring('DQM_Cosmics_prompt')
##                                                         )
##                                                ),
##                              verbose = cms.untracked.bool(False)
##                              )
