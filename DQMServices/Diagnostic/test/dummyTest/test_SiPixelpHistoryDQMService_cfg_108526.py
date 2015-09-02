import FWCore.ParameterSet.Config as cms

process = cms.Process("PWRITE")

#########################
# message logger
######################### 

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring('cout'),
#readFromFile_108526 = cms.untracked.PSet(threshold = cms.untracked.string('DEBUG')),
                                    debugModules = cms.untracked.vstring('*')
                                    )


#########################
# maxEvents ...
#########################

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1))

process.source = cms.Source("EmptySource",
                            timetype = cms.string("runnumber"),
                            firstRun = cms.untracked.uint32(1),
                            lastRun  = cms.untracked.uint32(1),
                            interval = cms.uint32(1)
                            )

#########################
# DQM services
#########################

process.load("DQMServices.Core.DQM_cfg")


########################
# DB parameters
########################

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
                                          DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(2),
    authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
                                          timetype = cms.untracked.string('runnumber'),
                                          connect = cms.string('sqlite_file:dbfile.db'),
                                          toPut = cms.VPSet(cms.PSet(
    record = cms.string("HDQMSummary"),
    tag = cms.string("HDQM_SiPixel")
    )),
                                          logconnect = cms.untracked.string("sqlite_file:log.db") 
                                          )

########################
# POPCON Application
########################
process.siPixelDQMHistoryPopCon = cms.EDAnalyzer("SiPixelDQMHistoryPopCon",
                                                 record = cms.string("HDQMSummary"),
                                                 loggingOn = cms.untracked.bool(True),
                                                 SinceAppendMode = cms.bool(True),
                                                 Source = cms.PSet(since = cms.untracked.uint32(108526),debug = cms.untracked.bool(False))
                                                 ) 


########################
# HistoricDQMService
########################

process.SiPixelHistoryDQMService = cms.Service("SiPixelHistoryDQMService",
    RunNb = cms.uint32(108526),
    accessDQMFile = cms.bool(True),
    FILE_NAME = cms.untracked.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/PromptReco/108/526/DQM_V0001_R000108526__Cosmics__CRAFT09-PromptReco-v1__RECO.root"),
    ME_DIR = cms.untracked.string("Run 108526"),
    histoList = cms.VPSet(

# quantities are 'stat', 'landau', 'gauss'
# where 
#'stat' includes entries, mean, rms
#'landau' includes
#'gauss' includes gaussMean, gaussSigma


# CKFTk
      cms.PSet( keyName = cms.untracked.string("SUMOFF_adc"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_charge_OffTrack"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_charge_OnTrack" ), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_nRecHits"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_nclusters_OffTrack"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_nclusters_OnTrack" ), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_ndigis"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_size_OffTrack"), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
      cms.PSet( keyName = cms.untracked.string("SUMOFF_size_OnTrack" ), quantitiesToExtract = cms.untracked.vstring("user_ymean") ),
#cms.PSet( keyName = cms.untracked.string("ntracks_rsWithMaterialTracksP5" ), quantitiesToExtract = cms.untracked.vstring("user_ymeanB") ), # pixel/All, FPix/BPix
      )
    )


# Schedule

process.p = cms.Path(process.siPixelDQMHistoryPopCon)




