import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'SimG4CMS/CherenkovAnalysis/data/singleDREAM.xml'),
    rootNodeName = cms.string('singleDREAM:DREAM')
)


