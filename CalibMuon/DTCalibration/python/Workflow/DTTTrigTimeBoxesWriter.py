from tools import loadCmsProcess,writeCfg
from addPoolDBESSource import addPoolDBESSource
from CmsswTask import CmsswTask
import os

class DTTTrigTimeBoxesWriter:
    def __init__(self, run, dir, result_dir, config):
        self.runnumber = int(run)
        self.config = config
        self.dir = dir
        self.result_dir = result_dir

        """
        self.configs = ['dtTTrigWriter_cfg.py',
                        'dumpDBToFile_first_cfg.py',
                        'dtTTrigCorrection_cfg.py',
                        'dumpDBToFile_second_cfg.py']

        self.pset_templates = {'dtTTrigWriter_cfg.py':'CalibMuon.DTCalibration.dtTTrigWriter_cfg',
                               'dumpDBToFile_first_cfg.py':'CalibMuon.DTCalibration.dumpDBToFile_ttrig_cfg',
                               'dtTTrigCorrection_cfg.py':'CalibMuon.DTCalibration.dtTTrigCorrection_cfg',
                               'dumpDBToFile_second_cfg.py':'CalibMuon.DTCalibration.dumpDBToFile_ttrig_cfg'}
        """
        self.configs = ['dtTTrigWriter_cfg.py',
                        'dtTTrigCorrection_cfg.py',
                        'dumpDBToFile_ttrig_cfg.py']

        self.pset_templates = {'dtTTrigWriter_cfg.py':'CalibMuon.DTCalibration.dtTTrigWriter_cfg',
                               'dtTTrigCorrection_cfg.py':'CalibMuon.DTCalibration.dtTTrigCorrection_cfg',
                               'dumpDBToFile_ttrig_cfg.py':'CalibMuon.DTCalibration.dumpDBToFile_ttrig_cfg'}
 

        self.initProcess()
        self.task = CmsswTask(self.dir,self.configs)

    def initProcess(self):
        timeBoxes = os.path.abspath( self.result_dir + '/' + 'DTTimeBoxes_' + str(self.runnumber) + '.root' )
        ttrig_uncorrected_db = os.path.abspath( self.result_dir + '/' + 'ttrig_uncorrected_' + str(self.runnumber) + '.db' )
        ttrig_timeboxes_db = os.path.abspath( self.result_dir + '/' + 'ttrig_timeboxes_' + str(self.runnumber) + '.db' )
        ttrig_timeboxes_txt = os.path.abspath( self.result_dir + '/' + 'ttrig_timeboxes_' + str(self.runnumber) + '.txt' )       
 
        self.process = {}
        # dtTTrigWriter_cfg.py
        self.process['dtTTrigWriter_cfg.py'] = loadCmsProcess(self.pset_templates['dtTTrigWriter_cfg.py'])
        self.process['dtTTrigWriter_cfg.py'].dtTTrigWriter.rootFileName = timeBoxes
        self.process['dtTTrigWriter_cfg.py'].PoolDBOutputService.connect = 'sqlite_file:%s' % ttrig_uncorrected_db

        # dtTTrigCorrection_cfg.py
        self.process['dtTTrigCorrection_cfg.py'] = loadCmsProcess(self.pset_templates['dtTTrigCorrection_cfg.py'])
        self.process['dtTTrigCorrection_cfg.py'].GlobalTag.globaltag = self.config.globaltag
        self.process['dtTTrigCorrection_cfg.py'].source.firstRun = self.runnumber
        addPoolDBESSource(process = self.process['dtTTrigCorrection_cfg.py'],
                          moduleName = 'calibDB',record = 'DTTtrigRcd',tag = 'ttrig',
                          connect = 'sqlite_file:%s' % ttrig_uncorrected_db)

        self.process['dtTTrigCorrection_cfg.py'].PoolDBOutputService.connect = 'sqlite_file:%s' % ttrig_timeboxes_db

        # dumpDBToFile_ttrig_cfg.py
        self.process['dumpDBToFile_ttrig_cfg.py'] = loadCmsProcess(self.pset_templates['dumpDBToFile_ttrig_cfg.py'])
        self.process['dumpDBToFile_ttrig_cfg.py'].calibDB.connect = 'sqlite_file:%s' % ttrig_timeboxes_db
        self.process['dumpDBToFile_ttrig_cfg.py'].dumpToFile.outputFileName = ttrig_timeboxes_txt
 
    def writeCfg(self):
        for cfg in self.configs:
            writeCfg(self.process[cfg],self.dir,cfg)
            #writeCfgPkl(self.process[cfg],self.dir,cfg)
           
    def run(self):
        self.task.run()
        return 
