import FWCore.ParameterSet.Config as cms

l1csctpconf = cms.ESProducer("L1CSCTriggerPrimitivesConfigProducer",

    # Defines the set of parameters used in MTCC.
    isMTCC = cms.bool(False),

    # Defines the set of parameters used in the minus-endcap slice test with
    # the new TMB07 firmware.
    isTMB07 = cms.bool(True),

    # Parameters for ALCT processors: default
    alctParam = cms.PSet(
        alctFifoTbins              = cms.uint32(16),
        alctFifoPretrig            = cms.uint32(10),
        alctDriftDelay             = cms.uint32(3),
        alctNplanesHitPretrig      = cms.uint32(2),
        alctNplanesHitPattern      = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctTrigMode               = cms.uint32(3),
        alctAccelMode              = cms.uint32(1),
        alctL1aWindowWidth         = cms.uint32(5)
    ),

    # Parameters for ALCT processors: MTCC-II and 2007 tests of new firmware
    alctParamMTCC2 = cms.PSet(
        alctFifoTbins              = cms.uint32(16),
        alctFifoPretrig            = cms.uint32(10),
        alctDriftDelay             = cms.uint32(2),
        alctNplanesHitPretrig      = cms.uint32(3),
        alctNplanesHitPattern      = cms.uint32(4),
        alctNplanesHitAccelPretrig = cms.uint32(3),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctTrigMode               = cms.uint32(2),
        alctAccelMode              = cms.uint32(0),
        alctL1aWindowWidth         = cms.uint32(7)
    ),

    # Parameters for CLCT processors: default and 2007 tests of new firmware
    clctParam = cms.PSet(
        clctFifoTbins   = cms.uint32(12),
        clctFifoPretrig = cms.uint32(7),
        clctHitPersist  = cms.uint32(4),
        clctDriftDelay  = cms.uint32(2),
        clctNplanesHitPretrig = cms.uint32(3),
        clctNplanesHitPattern = cms.uint32(4),
        clctPidThreshPretrig  = cms.uint32(2),
        clctMinSeparation     = cms.uint32(10)
    ),

    # Parameters for CLCT processors: MTCC-II
    clctParamMTCC2 = cms.PSet(
        clctFifoTbins   = cms.uint32(12),
        clctFifoPretrig = cms.uint32(7),
        clctHitPersist  = cms.uint32(6),
        clctDriftDelay  = cms.uint32(2),
        clctNplanesHitPretrig = cms.uint32(4),
        clctNplanesHitPattern = cms.uint32(1),
        clctPidThreshPretrig  = cms.uint32(2),
        clctMinSeparation     = cms.uint32(10)
    ),

    # Parameters for TMB
    tmbParam = cms.PSet(
        tmbMpcBlockMe1a        = cms.uint32(0),
        tmbAlctTrigEnable      = cms.uint32(0),
        tmbClctTrigEnable      = cms.uint32(0),
        tmbMatchTrigEnable     = cms.uint32(1),
        tmbMatchTrigWindowSize = cms.uint32(7),
        tmbTmbL1aWindowSize    = cms.uint32(7)
    )
)
