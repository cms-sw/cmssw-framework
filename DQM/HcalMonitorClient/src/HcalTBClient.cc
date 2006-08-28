#include <DQM/HcalMonitorClient/interface/HcalTBClient.h>
#include <DQM/HcalMonitorClient/interface/HcalClientUtils.h>

HcalTBClient::HcalTBClient(const ParameterSet& ps, MonitorUserInterface* mui){
  dqmReportMapErr_.clear(); dqmReportMapWarn_.clear(); dqmReportMapOther_.clear();
  dqmQtests_.clear();

  mui_ = mui;
  for(int i=0; i<3; i++){
    CHK[i] = 0;
    TOFT_S[i] = 0;
    TOFT_J[i] = 0;
  }
  for(int i=0; i<4; i++) DT[i]=0;
  for(int i=0; i<8; i++){
    WC[i]=0;
    WCX[i]=0;
    WCY[i]=0;
  }
  TOFQ[0] = 0;  TOFQ[1] = 0;

  // cloneME switch
  cloneME_ = ps.getUntrackedParameter<bool>("cloneME", true);
  
  // verbosity switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);

  // DQM default process name
  process_ = ps.getUntrackedParameter<string>("processName", "HcalMonitor");
}

HcalTBClient::~HcalTBClient(){

  this->cleanup();

}

void HcalTBClient::beginJob(void){

  if ( verbose_ ) cout << "HcalTBClient: beginJob" << endl;

  ievt_ = 0;
  jevt_ = 0;
  this->setup();
  this->subscribe();
  this->resetME();
  return;
}

void HcalTBClient::beginRun(void){

  if ( verbose_ ) cout << "HcalTBClient: beginRun" << endl;

  jevt_ = 0;
  this->setup();
  this->subscribe();
  this->resetME();
  return;
}

void HcalTBClient::endJob(void) {

  if ( verbose_ ) cout << "HcalTBClient: endJob, ievt = " << ievt_ << endl;

  this->cleanup();
  return;
}

void HcalTBClient::endRun(void) {

  if ( verbose_ ) cout << "HcalTBClient: endRun, jevt = " << jevt_ << endl;
  //  this->resetME();
  //  this->unsubscribe();
  this->cleanup();
  return;
}

void HcalTBClient::setup(void) {
  return;
}

void HcalTBClient::cleanup(void) {

  if( cloneME_ ){
    for(int i=0; i<3; i++){
      if ( CHK[i] )  delete CHK[i];
      if ( TOFT_S[i] )  delete TOFT_S[i];
      if ( TOFT_J[i] )  delete TOFT_J[i];
    }    
    for(int i=0; i<4; i++) if ( DT[i] )  delete DT[i];
    for(int i=0; i<8; i++){
      if ( WC[i] ) delete WC[i];
      if ( WCX[i] ) delete WCX[i];
      if ( WCY[i] ) delete WCY[i];
    }    
    if(TOFQ[0]) delete TOFQ[0];
    if(TOFQ[1]) delete TOFQ[1];
  }
  
  for(int i=0; i<3; i++){
    CHK[i] = 0;
    TOFT_S[i] = 0;
    TOFT_J[i] = 0;
  }    
  for(int i=0; i<4; i++) DT[i] = 0;
  for(int i=0; i<8; i++){
    WC[i]=0;
    WCX[i]=0;
    WCY[i]=0;
  }   
  TOFQ[0] = 0; TOFQ[1]=0;
  
  dqmReportMapErr_.clear(); dqmReportMapWarn_.clear(); dqmReportMapOther_.clear();
  dqmQtests_.clear();
  
  return;
}

void HcalTBClient::subscribe(void){

  if ( verbose_ ) cout << "HcalTBClient: subscribe" << endl;
  mui_->subscribe("*/TBMonitor/*");
  mui_->subscribe("*/TBMonitor/QADCMonitor/*");
  mui_->subscribe("*/TBMonitor/TimingMonitor/*");
  mui_->subscribe("*/TBMonitor/EventPositionMonitor/*");
  return;
}

void HcalTBClient::subscribeNew(void){
  mui_->subscribeNew("*/TBMonitor/*");
  mui_->subscribeNew("*/TBMonitor/QADCMonitor/*");
  mui_->subscribeNew("*/TBMonitor/TimingMonitor/*");
  mui_->subscribeNew("*/TBMonitor/EventPositionMonitor/*");
  return;
}

void HcalTBClient::unsubscribe(void){

  if ( verbose_ ) cout << "HcalTBClient: unsubscribe" << endl;
  mui_->unsubscribe("*/TBMonitor/*");
  mui_->unsubscribe("*/TBMonitor/QADCMonitor/*");
  mui_->unsubscribe("*/TBMonitor/TimingMonitor/*");
  mui_->unsubscribe("*/TBMonitor/EventPositionMonitor/*");
  return;
}

void HcalTBClient::errorOutput(){
  
  dqmReportMapErr_.clear(); dqmReportMapWarn_.clear(); dqmReportMapOther_.clear();
  
  for (map<string, string>::iterator testsMap=dqmQtests_.begin(); testsMap!=dqmQtests_.end();testsMap++){
    string testName = testsMap->first;
    string meName = testsMap->second;
    MonitorElement* me = mui_->get(meName);
    if(me){
      if (me->hasError()){
	vector<QReport*> report =  me->getQErrors();
	dqmReportMapErr_[meName] = report;
      }
      if (me->hasWarning()){
	vector<QReport*> report =  me->getQWarnings();
	dqmReportMapWarn_[meName] = report;
      }
      if(me->hasOtherReport()){
	vector<QReport*> report= me->getQOthers();
	dqmReportMapOther_[meName] = report;
      }
    }
  }
  printf("Test Beam Task: %d errs, %d warnings, %d others\n",dqmReportMapErr_.size(),dqmReportMapWarn_.size(),dqmReportMapOther_.size());

  return;
}

void HcalTBClient::getErrors(map<string, vector<QReport*> > outE, map<string, vector<QReport*> > outW, map<string, vector<QReport*> > outO){

  this->errorOutput();
  outE.clear(); outW.clear(); outO.clear();

  for(map<string, vector<QReport*> >::iterator i=dqmReportMapErr_.begin(); i!=dqmReportMapErr_.end(); i++){
    outE[i->first] = i->second;
  }
  for(map<string, vector<QReport*> >::iterator i=dqmReportMapWarn_.begin(); i!=dqmReportMapWarn_.end(); i++){
    outW[i->first] = i->second;
  }
  for(map<string, vector<QReport*> >::iterator i=dqmReportMapOther_.begin(); i!=dqmReportMapOther_.end(); i++){
    outO[i->first] = i->second;
  }

  return;
}

void HcalTBClient::analyze(void){

  jevt_++;
  int updates = mui_->getNumUpdates();
  if ( updates % 10 == 0 ) {
    if ( verbose_ ) cout << "HcalTBClient: " << updates << " updates" << endl;
  }
  return;
}

void HcalTBClient::getHistograms(){
  char name[150];     
  MonitorElement* me=0;
  for(int i=0; i<3; i++){
    sprintf(name,"Collector/%s/TBMonitor/QADCMonitor/Cherenkov QADC %d", process_.c_str(),i+1);
    me = mui_->get(name);
    CHK[i] = getHisto(me, verbose_,cloneME_);
  }
  for(int i=0; i<2; i++){
    sprintf(name,"Collector/%s/TBMonitor/QADCMonitor/TOF QADC %d", process_.c_str(),i+1);
    me = mui_->get(name);
    TOFQ[i] = getHisto(me, verbose_,cloneME_);
                                                   
    sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF TDC %d - Saleve", process_.c_str(),i+1);
    me = mui_->get(name);
    TOFT_S[i] = getHisto(me, verbose_,cloneME_);

    sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF TDC %d - Jura", process_.c_str(),i+1);
    me = mui_->get(name);
    TOFT_J[i] = getHisto(me, verbose_,cloneME_);

  }
  
  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF Time - Saleve", process_.c_str());
  me = mui_->get(name);
  TOFT_S[2] = getHisto(me, verbose_,cloneME_);

  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF Time - Jura", process_.c_str());
  me = mui_->get(name);
  TOFT_J[2] = getHisto(me, verbose_,cloneME_);

  int i = 0;
  for(char c = 'A'; c<='H'; c++){
    sprintf(name,"Collector/%s/TBMonitor/EventPositionMonitor/Wire Chamber %c Hits", process_.c_str(),c);
    me = mui_->get(name);
    WC[i] = getHisto2(me, verbose_,cloneME_);

    sprintf(name,"Collector/%s/TBMonitor/EventPositionMonitor/Wire Chamber %c X Hits", process_.c_str(),c);
    me = mui_->get(name);
    WCX[i] = getHisto(me, verbose_,cloneME_);

    sprintf(name,"Collector/%s/TBMonitor/EventPositionMonitor/Wire Chamber %c Y Hits", process_.c_str(),c);
    me = mui_->get(name);
    WCY[i] = getHisto(me, verbose_,cloneME_);
    i++;
  }
  
  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/L1A-BC Phase", process_.c_str());
  me = mui_->get(name);
  DT[0] = getHisto(me, verbose_,cloneME_);

  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF TDC - Delta 1", process_.c_str());
  me = mui_->get(name);
  DT[1] = getHisto(me, verbose_,cloneME_);

  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF TDC - Delta 2", process_.c_str());
  me = mui_->get(name);
  DT[2] = getHisto(me, verbose_,cloneME_);

  sprintf(name,"Collector/%s/TBMonitor/TimingMonitor/TOF Time - Delta", process_.c_str());
  me = mui_->get(name);
  DT[3] = getHisto(me, verbose_,cloneME_);

  return;
}

void HcalTBClient::report(){

  if ( verbose_ ) cout << "HcalTBClient: report" << endl;
  this->setup();
  
  char name[256];
  sprintf(name, "Collector/%s/TBMonitor/EventPositionMonitor/Event Position Event Number",process_.c_str());
  MonitorElement* me = mui_->get(name);
  if ( me ) {
    string s = me->valueString();
    ievt_ = -1;
    sscanf((s.substr(2,s.length()-2)).c_str(), "%d", &ievt_);
    if ( verbose_ ) cout << "Found '" << name << "'" << endl;
  }
  
  getHistograms();

  return;
}

void HcalTBClient::resetME(){
  
  //  MonitorElement* me;
  //  char name[150];     
  for(int i=0; i<3; i++){
    /*
    sprintf(name,"Collector/%s/HcalMonitor/TBMonitor/%s Data Format Error Words",process_.c_str(),type.c_str());
    me = mui_->get(name);
    if(me) mui_->softReset(me);
    */
  }
  return;
}

void HcalTBClient::htmlOutput(int run, string htmlDir, string htmlName){

  cout << "Preparing HcalTBClient html output ..." << endl;
  string client = "TBMonitor";
  htmlErrors(htmlDir,client,process_,mui_,dqmReportMapErr_,dqmReportMapWarn_,dqmReportMapOther_);

  ofstream htmlFile;
  htmlFile.open((htmlDir + htmlName).c_str());

  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
  htmlFile << " http-equiv=\"content-type\">  " << endl;
  htmlFile << "  <title>Monitor: Test Beam Task output</title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<br>  " << endl;
  htmlFile << "<h2>Run:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << run << "</span></h2>" << endl;
  htmlFile << "<h2>Monitoring task:&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">Test Beam</span></h2> " << endl;
  htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << ievt_ << "</span></h2>" << endl;
  htmlFile << "<hr>" << endl;
  htmlFile << "<table border=1><tr>" << endl;
  if(hasErrors())htmlFile << "<td bgcolor=red><a href=\"TBMonitorErrors.html\">Errors in this task</a></td>" << endl;
  else htmlFile << "<td bgcolor=lime>No Errors</td>" << endl;
  if(hasWarnings()) htmlFile << "<td bgcolor=yellow><a href=\"TBMonitorWarnings.html\">Warnings in this task</a></td>" << endl;
  else htmlFile << "<td bgcolor=lime>No Warnings</td>" << endl;
  if(hasOther()) htmlFile << "<td bgcolor=aqua><a href=\"TBMonitorMessages.html\">Messages in this task</a></td>" << endl;
  else htmlFile << "<td bgcolor=lime>No Messages</td>" << endl;
  htmlFile << "</tr></table>" << endl;
  htmlFile << "<hr>" << endl;
  
  htmlFile << "<h2><strong>Test Beam Histograms</strong></h2>" << endl;
  
  htmlFile << "<table border=\"0\" cellspacing=\"0\" " << endl;
  htmlFile << "cellpadding=\"10\"> " << endl;
  
  this->evtposHTML(htmlDir, "TB_EvtPos.html");
  htmlFile << "<h3><a href=\"TB_EvtPos.html\">Event Position Monitor</a></h3>" << endl;
  this->timingHTML(htmlDir, "TB_Time.html");
  htmlFile << "<h3><a href=\"TB_Time.html\">Timing Monitor</a></h3>" << endl;
  this->qadcHTML(htmlDir, "TB_QADC.html");
  htmlFile << "<h3><a href=\"TB_QADC.html\">QADC Monitor</a></h3>" << endl;
  
  // html page footer
  htmlFile << "</body> " << endl;
  htmlFile << "</html> " << endl;
  
  htmlFile.close();
  return;
}

void HcalTBClient::evtposHTML(string htmlDir, string htmlName){
  
  ofstream htmlFile;
  htmlFile.open((htmlDir + htmlName).c_str());

  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
  htmlFile << " http-equiv=\"content-type\">  " << endl;
  htmlFile << "  <title>Monitor: Event Position Monitor output</title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<br>  " << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << "<h2>Monitoring task:&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">Event Position Monitor</span></h2> " << endl;
  htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << ievt_ << "</span></h2>" << endl;
  htmlFile << "<hr>" << endl;
  
  htmlFile << "<h2><strong>Event Position Histograms</strong></h2>" << endl;
  
  htmlFile << "<table border=\"0\" cellspacing=\"0\" " << endl;
  htmlFile << "cellpadding=\"10\"> " << endl;

  htmlFile << "<td>&nbsp;&nbsp;&nbsp;<h3>Wire Chamber Histograms</h3></td></tr>" << endl;
  htmlFile << "<tr align=\"left\">" << endl;
  histoHTML2(WC[0],"X","Y", 92, htmlFile,htmlDir);
  histoHTML2(WC[1],"X","Y", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML2(WC[2],"X","Y", 92, htmlFile,htmlDir);
  histoHTML2(WC[3],"X","Y", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML2(WC[4],"X","Y", 92, htmlFile,htmlDir);
  histoHTML2(WC[5],"X","Y", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML2(WC[6],"X","Y", 92, htmlFile,htmlDir);
  histoHTML2(WC[7],"X","Y", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  
  histoHTML(WCX[0],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[0],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[1],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[1],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[2],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[2],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[3],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[3],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[4],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[4],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[5],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[5],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[6],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[6],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(WCX[7],"Hit Position","", 92, htmlFile,htmlDir);
  histoHTML(WCY[7],"Hit Position","", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;


  // html page footer
  htmlFile << "</body> " << endl;
  htmlFile << "</html> " << endl;
  
  htmlFile.close();
  return;

}

void HcalTBClient::qadcHTML(string htmlDir, string htmlName){
  
  ofstream htmlFile;
  htmlFile.open((htmlDir + htmlName).c_str());

  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
  htmlFile << " http-equiv=\"content-type\">  " << endl;
  htmlFile << "  <title>Monitor: QADC Monitor output</title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<br>  " << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << "<h2>Monitoring task:&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">QADC Monitor</span></h2> " << endl;
  htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << ievt_ << "</span></h2>" << endl;
  htmlFile << "<hr>" << endl;
  
  htmlFile << "<h2><strong>Cherenkov Histograms</strong></h2>" << endl;
  
  htmlFile << "<table border=\"0\" cellspacing=\"0\" " << endl;
  htmlFile << "cellpadding=\"10\"> " << endl;

  htmlFile << "<td>&nbsp;&nbsp;&nbsp;<h3>Cherenkov Histograms</h3></td></tr>" << endl;
  htmlFile << "<tr align=\"left\">" << endl;
  histoHTML(CHK[0],"ADC","Hits/ADC", 92, htmlFile,htmlDir);
  histoHTML(CHK[1],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(CHK[2],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;

  // html page footer
  htmlFile << "</body> " << endl;
  htmlFile << "</html> " << endl;
  
  htmlFile.close();
  return;

}

void HcalTBClient::timingHTML(string htmlDir, string htmlName){
  
  ofstream htmlFile;
  htmlFile.open((htmlDir + htmlName).c_str());

  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
  htmlFile << " http-equiv=\"content-type\">  " << endl;
  htmlFile << "  <title>Monitor: Timing Monitor output</title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<br>  " << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << "<h2>Monitoring task:&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">Timing Monitor</span></h2> " << endl;
  htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
  htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << ievt_ << "</span></h2>" << endl;
  htmlFile << "<hr>" << endl;
  
  htmlFile << "<h2><strong>TOF Histograms</strong></h2>" << endl;
  
  htmlFile << "<table border=\"0\" cellspacing=\"0\" " << endl;
  htmlFile << "cellpadding=\"10\"> " << endl;

  htmlFile << "<td>&nbsp;&nbsp;&nbsp;<h3>Timing Histograms</h3></td></tr>" << endl;
  htmlFile << "<tr align=\"left\">" << endl;
  histoHTML(TOFT_S[0],"ADC","Hits/ADC", 92, htmlFile,htmlDir);
  histoHTML(TOFT_S[1],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(TOFT_J[0],"ADC","Hits/ADC", 92, htmlFile,htmlDir);
  histoHTML(TOFT_J[1],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(TOFT_S[2],"Time","Hits/nS", 92, htmlFile,htmlDir);
  histoHTML(TOFT_J[2],"Time","Hits/nS", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(DT[1],"ADC","Hits/ADC", 92, htmlFile,htmlDir);
  histoHTML(DT[2],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;
  histoHTML(DT[0],"ADC","Hits/ADC", 92, htmlFile,htmlDir);
  histoHTML(DT[3],"ADC","Hits/ADC", 100, htmlFile,htmlDir);
  htmlFile << "</tr>" << endl;

  // html page footer
  htmlFile << "</body> " << endl;
  htmlFile << "</html> " << endl;
  
  htmlFile.close();
  return;

}

void HcalTBClient::createTests(){
  //  char meTitle[250], name[250];    
  //  vector<string> params;
  
  printf("There are no Test Beam quality tests yet...\n");

  return;
}

