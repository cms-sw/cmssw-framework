import FWCore.ParameterSet.Config as cms

discriminantConfiguration = cms.PSet(

    FlightPathSignificance = cms.PSet(
        plugin = cms.string("RecoTauDiscriminantFromDiscriminator"),
        discSrc = cms.InputTag('hpsTancTausDiscriminationByFlightPath'),
        minValue = cms.double(-25),
        maxValue = cms.double(25),
    ),

    InvariantOpeningAngle = cms.PSet(
        plugin = cms.string("RecoTauDiscriminantInvariantWidth"),
        decayModes = cms.VPSet(
            cms.PSet(
                nCharged = cms.uint32(1),
                nPiZeros = cms.uint32(1),
                mean = cms.string("5.0e-3 + 0.43/max(pt, 1.0)"),
                rms = cms.string("2.7e-3 + 0.23/max(pt, 1.0)"),
            ),
            cms.PSet(
                nCharged = cms.uint32(1),
                nPiZeros = cms.uint32(2),
                mean = cms.string("4.7e-3 + 0.9/max(pt, 1.0)"),
                rms = cms.string("7.5e-3 + 0.3/max(pt, 1.0)"),
            ),
            cms.PSet(
                nCharged = cms.uint32(3),
                nPiZeros = cms.uint32(0),
                mean = cms.string("0.87/max(pt, 1.0)"),
                rms = cms.string("0.38/max(pt, 1.0)"),
            ),
        ),
        # These shouldn't happen.
        defaultMean = cms.string("max(0.87/max(pt, 1.0), 0.005)"),
        defaultRMS = cms.string("max(0.3/max(pt, 1.0), 0.005)"),
    ),

    # Binned isolation plugins are defined in
    # RecoTauTag/RecoTau/plugins/RecoTauIsolationDiscriminantPlugins.cc
    BinnedTrackIsolation = cms.PSet(
        plugin = cms.string("RecoTauDiscriminationBinnedTrackIsolation"),
        vtxSource = cms.InputTag("recoTauPileUpVertices"),
        binning = cms.VPSet(
            cms.PSet(
                nPUVtx = cms.int32(0),
                binLowEdges = cms.vdouble(0.50, 0.86, 1.87)
            ),
            cms.PSet(
                nPUVtx = cms.int32(1),
                binLowEdges = cms.vdouble(0.51, 0.86, 1.87)
            ),
            cms.PSet(
                nPUVtx = cms.int32(2),
                binLowEdges = cms.vdouble(0.51, 0.86, 1.87)
            ),
            cms.PSet(
                nPUVtx = cms.int32(3),
                binLowEdges = cms.vdouble(0.52, 0.86, 1.87)
            ),
            cms.PSet(
                nPUVtx = cms.int32(4),
                binLowEdges = cms.vdouble(0.52, 0.86, 1.87)
            ),
        ),
        defaultBinning = cms.vdouble(0.52, 0.86, 1.87)
    ),

    BinnedMaskedEcalIsolation = cms.PSet(
        plugin = cms.string("RecoTauDiscriminationBinnedMaskedECALIsolation"),
        vtxSource = cms.InputTag("recoTauPileUpVertices"),
        mask = cms.PSet(
            ecalCone = cms.double(0.15),
            hcalCone = cms.double(0.3),
            finalHcalCone = cms.double(0.08),
            maxSigmas = cms.double(2)
        ),
        binning = cms.VPSet(
            cms.PSet(
                nPUVtx = cms.int32(0),
                binLowEdges = cms.vdouble(0.50, 0.88, 2.53)
            ),
            cms.PSet(
                nPUVtx = cms.int32(1),
                binLowEdges = cms.vdouble(0.65, 0.98, 2.53)
            ),
            cms.PSet(
                nPUVtx = cms.int32(2),
                binLowEdges = cms.vdouble(0.74, 1.05, 2.54)
            ),
            cms.PSet(
                nPUVtx = cms.int32(3),
                binLowEdges = cms.vdouble(0.80, 1.11, 2.54)
            ),
            cms.PSet(
                nPUVtx = cms.int32(4),
                binLowEdges = cms.vdouble(0.86, 1.15, 2.54)
            ),
            cms.PSet(
                nPUVtx = cms.int32(5),
                binLowEdges = cms.vdouble(0.90, 1.19, 2.55)
            ),
            cms.PSet(
                nPUVtx = cms.int32(6),
                binLowEdges = cms.vdouble(0.94, 1.22, 2.55)
            ),
            cms.PSet(
                nPUVtx = cms.int32(7),
                binLowEdges = cms.vdouble(0.97, 1.25, 2.55)
            ),
            cms.PSet(
                nPUVtx = cms.int32(8),
                binLowEdges = cms.vdouble(1.00, 1.27, 2.56)
            ),
            cms.PSet(
                nPUVtx = cms.int32(9),
                binLowEdges = cms.vdouble(1.03, 1.30, 2.56)
            ),
        ),
        defaultBinning = cms.vdouble(1.03, 1.30, 2.56)
    ),

    BinnedMaskedHcalIsolation = cms.PSet(
        plugin = cms.string("RecoTauDiscriminationBinnedMaskedHCALIsolation"),
        vtxSource = cms.InputTag("recoTauPileUpVertices"),
        mask = cms.PSet(
            ecalCone = cms.double(0.15),
            hcalCone = cms.double(0.3),
            finalHcalCone = cms.double(0.08),
            maxSigmas = cms.double(2)
        ),
        binning = cms.VPSet(
            cms.PSet(
                nPUVtx = cms.int32(0),
                binLowEdges = cms.vdouble(1.00, 1.79, 4.03)
            ),
            cms.PSet(
                nPUVtx = cms.int32(1),
                binLowEdges = cms.vdouble(1.15, 1.80, 4.03)
            ),
            cms.PSet(
                nPUVtx = cms.int32(2),
                binLowEdges = cms.vdouble(1.22, 1.81, 4.03)
            ),
            cms.PSet(
                nPUVtx = cms.int32(3),
                binLowEdges = cms.vdouble(1.27, 1.83, 4.03)
            ),
            cms.PSet(
                nPUVtx = cms.int32(4),
                binLowEdges = cms.vdouble(1.31, 1.84, 4.03)
            ),
        ),
        defaultBinning = cms.vdouble(1.31, 1.84, 4.03)
    ),
)
