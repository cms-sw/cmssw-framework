import FWCore.ParameterSet.Config as cms
from Configuration.AlCa.autoCond import autoCond

process = cms.Process("GeometryTest")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = autoCond['run1_mc']

process.source = cms.Source("EmptyIOVSource",
                            lastValue = cms.uint64(3),
                            timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )

process.GlobalTag.toGet = cms.VPSet(
    cms.PSet(record = cms.string("PHGCalRcd"),
             tag = cms.string("HGCALRECO_Geometry_Test01"),
             connect = cms.string("sqlite_file:myfile.db")
             )
    )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    categories = cms.untracked.vstring('HGCalGeom'),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        HGCalGeom = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.test = cms.EDAnalyzer("HGCalGeometryTester")

process.p1 = cms.Path(process.test)
