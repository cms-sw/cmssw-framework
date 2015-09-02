import FWCore.ParameterSet.Config as cms

dtPreCalibTask = cms.EDAnalyzer("DTPreCalibrationTask",
    SaveFile = cms.untracked.bool(True),
    outputFileName = cms.untracked.string('DigiHistos.root'),
    digiLabel = cms.untracked.string('muonDTDigis'),
    minTriggerWidth = cms.untracked.int32(0),
    maxTriggerWidth = cms.untracked.int32(1600),
    folderName = cms.untracked.string('')
)
