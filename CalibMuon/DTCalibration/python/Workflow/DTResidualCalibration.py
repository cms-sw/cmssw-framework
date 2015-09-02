from tools import loadCmsProcess,loadCrabCfg,loadCrabDefault,addCrabInputFile,writeCfg,prependPaths
from addPoolDBESSource import addPoolDBESSource
from CrabTask import *
import os

class DTResidualCalibration:
    def __init__(self, run, dir, input_db, config):
        self.pset_name = 'dtResidualCalibration_cfg.py'
        self.outputfile = 'residuals.root'
        self.config = config
        self.dir = dir
        self.inputdb = input_db

        self.pset_template = 'CalibMuon.DTCalibration.dtResidualCalibration_cfg'
        if hasattr(self.config,'runOnCosmics') and self.config.runOnCosmics:
            self.pset_template = 'CalibMuon.DTCalibration.dtResidualCalibration_cosmics_cfg'

        self.process = None  
        self.crab_cfg = None
        self.initProcess()
        self.initCrab()
        self.task = CrabTask(self.dir,self.crab_cfg)

    def initProcess(self):
        self.process = loadCmsProcess(self.pset_template)
        self.process.GlobalTag.globaltag = self.config.globaltag
        self.process.dtResidualCalibration.rootFileName = self.outputfile 
        # Update Event Setup
	if hasattr(self.config,'inputDBTag') and self.config.inputDBTag:
	    tag = self.config.inputDBTag
	    record = self.config.inputDBRcd
	    connect = self.config.connectStrDBTag
	    moduleName = 'customDB%s' % record 
	    addPoolDBESSource(process = self.process,
			      moduleName = moduleName,record = record,tag = tag,
			      connect = connect)

        if hasattr(self.config,'inputVDriftDB') and self.config.inputVDriftDB:
            addPoolDBESSource(process = self.process,
                              moduleName = 'vDriftDB',record = 'DTMtimeRcd',tag = 'vDrift',
                              connect = 'sqlite_file:%s' % os.path.basename(self.config.inputVDriftDB))

        if hasattr(self.config,'inputT0DB') and self.config.inputT0DB:
            addPoolDBESSource(process = self.process,
                              moduleName = 't0DB',record = 'DTT0Rcd',tag = 't0',
                              connect = 'sqlite_file:%s' % os.path.basename(self.config.inputT0DB))

        if(self.inputdb):
            label = ''
            if hasattr(self.config,'runOnCosmics') and self.config.runOnCosmics: label = 'cosmics'
            addPoolDBESSource(process = self.process,
                              moduleName = 'calibDB',record = 'DTTtrigRcd',tag = 'ttrig',label = label,
                              connect = 'sqlite_file:%s' % os.path.basename(self.inputdb))

        # Update sequences
        if hasattr(self.config,'runOnRAW') and self.config.runOnRAW:
            if hasattr(self.config,'runOnMC') and self.config.runOnMC:
                getattr(self.process,self.config.digilabel).inputLabel = 'rawDataCollector' 
            prependPaths(self.process,self.config.digilabel)
 
        if hasattr(self.config,'preselection') and self.config.preselection:
            pathsequence = self.config.preselection.split(':')[0]
            seqname = self.config.preselection.split(':')[1]
            self.process.load(pathsequence)
            prependPaths(self.process,seqname)

    def initCrab(self):
        crab_cfg_parser = loadCrabCfg()
        loadCrabDefault(crab_cfg_parser,self.config)
        crab_cfg_parser.set('CMSSW','pset',self.pset_name)
        crab_cfg_parser.set('CMSSW','output_file',self.outputfile)
        crab_cfg_parser.remove_option('USER','additional_input_files')
        if self.inputdb:
            addCrabInputFile(crab_cfg_parser,self.inputdb)

        if hasattr(self.config,'inputVDriftDB') and self.config.inputVDriftDB:
            addCrabInputFile(crab_cfg_parser,self.config.inputVDriftDB)

        if hasattr(self.config,'inputT0DB') and self.config.inputT0DB:
            addCrabInputFile(crab_cfg_parser,self.config.inputT0DB)

        self.crab_cfg = crab_cfg_parser

    def writeCfg(self):
        writeCfg(self.process,self.dir,self.pset_name)
        #writeCfgPkl(self.process,self.dir,self.pset_name)

    def run(self):
        self.project = self.task.run()
        return self.project
