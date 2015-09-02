#include "CondFormats/EcalObjects/interface/EcalDQMTowerStatus.h"
//#include "CondTools/Ecal/interface/EcalDQMTowerStatusXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalCondHeader.h"
#include "TROOT.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLine.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "CondCore/Utilities/interface/PayLoadInspector.h"
#include "CondCore/Utilities/interface/InspectorPythonWrapper.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <fstream>

#include "CondCore/EcalPlugins/plugins/EcalPyWrapperFunctions.h"

namespace cond {

  namespace ecalcond {

    typedef EcalDQMTowerStatus::Items  Items;
    typedef EcalDQMTowerStatus::value_type  value_type;

    enum How { singleChannel, bySuperModule, all};

    int bad(Items const & cont) {
      return  std::count_if(cont.begin(),cont.end(),
			    boost::bind(std::greater<int>(),
					boost::bind(&value_type::getStatusCode,_1),0)
			    );
    }

    void extractBarrel(EcalDQMTowerStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] =  bad(cont.barrelItems());
    }
    
    void extractEndcap(EcalDQMTowerStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] = bad(cont.endcapItems());
    }
    void extractAll(EcalDQMTowerStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] = bad(cont.barrelItems())+bad(cont.endcapItems());
    }

    void extractSuperModules(EcalDQMTowerStatus const & cont, std::vector<int> const & which,  std::vector<float> & result) {
      // bho...
    }

    void extractSingleChannel(EcalDQMTowerStatus const & cont, std::vector<int> const & which,  std::vector<float> & result) {
      result.reserve(which.size());
      for (unsigned int i=0; i<which.size();i++) {
	result.push_back(cont[which[i]].getStatusCode());
      }
    }

	typedef boost::function<void(EcalDQMTowerStatus const & cont, std::vector<int> const & which,  std::vector<float> & result)> CondExtractor;
  }  // namespace ecalcond

  template<>
  struct ExtractWhat<EcalDQMTowerStatus> {

    ecalcond::How m_how;
    std::vector<int> m_which;

    ecalcond::How const & how() const { return m_how;}
    std::vector<int> const & which() const { return m_which;}
 
    void set_how(ecalcond::How i) {m_how=i;}
    void set_which(std::vector<int> & i) { m_which.swap(i);}
  };


  template<>
  class ValueExtractor<EcalDQMTowerStatus>: public  BaseValueExtractor<EcalDQMTowerStatus> {
  public:

    static ecalcond::CondExtractor & extractor(ecalcond::How how) {
      static  ecalcond::CondExtractor fun[3] = { 
	ecalcond::CondExtractor(ecalcond::extractSingleChannel),
	ecalcond::CondExtractor(ecalcond::extractSuperModules),
	ecalcond::CondExtractor(ecalcond::extractAll)
      };
      return fun[how];
    }

    typedef EcalDQMTowerStatus Class;
    typedef ExtractWhat<Class> What;
    static What what() { return What();}

    ValueExtractor(){}
    ValueExtractor(What const & what)
      : m_what(what)
    {
      // here one can make stuff really complicated... 
    }

    void compute(Class const & it){
      std::vector<float> res;
      extractor(m_what.how())(it,m_what.which(),res);
      swap(res);
    }

  private:
    What  m_what;  
  };


  //template<>
  //std::string PayLoadInspector<EcalDQMTowerStatus>::dump() const {
  //  std::stringstream ss;
  //  EcalCondHeader h;
  //  ss << EcalDQMTowerStatusXMLTranslator::dumpXML(h,object());
  //  return ss.str();
  //}

 	class EcalDQMTowerStatusHelper: public EcalPyWrapperHelper<EcalDQMStatusCode>{
	public:
		//change me
		EcalDQMTowerStatusHelper():EcalPyWrapperHelper<EcalObject>(30, STATUS, "-Errors total: "){}
	protected:

		//change me
		typedef EcalDQMStatusCode EcalObject;

		type_vValues getValues( const std::vector<EcalObject> & vItems)
		{
			//change me
			//unsigned int totalValues = 2; 

			type_vValues vValues(total_values);

			std::stringstream ss;

			std::string valueNames[] = {
				"CH_ID_ERROR                          ",
				"CH_GAIN_ZERO_ERROR                   ",
				"CH_GAIN_SWITCH_ERROR                 ",
				"TT_ID_ERROR                          ",
				"TT_SIZE_ERROR                        ",
				"PEDESTAL_LOW_GAIN_MEAN_ERROR         ",
				"PEDESTAL_MIDDLE_GAIN_MEAN_ERROR      ",
				"PEDESTAL_HIGH_GAIN_MEAN_ERROR        ",
				"PEDESTAL_LOW_GAIN_RMS_ERROR          ",
				"PEDESTAL_MIDDLE_GAIN_RMS_ERROR       ",
				"PEDESTAL_HIGH_GAIN_RMS_ERROR         ",
				"PEDESTAL_ONLINE_HIGH_GAIN_MEAN_ERROR ",
				"PEDESTAL_ONLINE_HIGH_GAIN_RMS_ERROR  ",
				"TESTPULSE_LOW_GAIN_MEAN_ERROR        ",
				"TESTPULSE_MIDDLE_GAIN_MEAN_ERROR     ",
				"TESTPULSE_HIGH_GAIN_MEAN_ERROR       ",
				"TESTPULSE_LOW_GAIN_RMS_ERROR         ",
				"TESTPULSE_MIDDLE_GAIN_RMS_ERROR      ",
				"TESTPULSE_HIGH_GAIN_RMS_ERROR        ",
				"LASER_MEAN_ERROR                     ",
				"LASER_RMS_ERROR                      ",
				"LASER_TIMING_MEAN_ERROR              ",
				"LASER_TIMING_RMS_ERROR               ",
				"LED_MEAN_ERROR                       ",
				"LED_RMS_ERROR                        ",
				"LED_TIMING_MEAN_ERROR                ",
				"LED_TIMING_RMS_ERROR                 ",
				"STATUS_FLAG_ERROR                    ",
				"PHYSICS_BAD_CHANNEL_WARNING          ",
				"PHYSICS_BAD_CHANNEL_ERROR            "
			};
			for (unsigned int i = 0; i < total_values; ++i){
				ss.str(""); ss << "[" << i << "]" << valueNames[i];
				vValues[i].first = ss.str();
				vValues[i].second = .0;
			}
			
			//get info:
			unsigned int shift = 0, mask = 1;
			unsigned int statusCode;
			for(std::vector<EcalObject>::const_iterator iItems = vItems.begin(); iItems != vItems.end(); ++iItems){
				//change us
				statusCode = iItems->getStatusCode();
				for (shift = 0; shift < total_values; ++shift){
					mask = 1 << (shift);
					//std::cout << "; statuscode: " << statusCode;
					if (statusCode & mask){
						vValues[shift].second += 1;
					}
				}
			}
			return vValues;
		}
	};

   template<>
   std::string PayLoadInspector<EcalDQMTowerStatus>::summary() const {
	std::stringstream ss;
	EcalDQMTowerStatusHelper helper;
	ss << helper.printBarrelsEndcaps(object().barrelItems(), object().endcapItems());
	return ss.str();
   }

   // return the real name of the file including extension...
  template<>
  std::string PayLoadInspector<EcalDQMTowerStatus>::plot(std::string const & filename,
							 std::string const &, 
							 std::vector<int> const&, 
							 std::vector<float> const& ) const {
    //    std::string fname = filename + ".txt";
    //    EcalDAQTowerStatusXMLTranslator::plot(fname, object());
    //    return fname;
    TCanvas canvas("CC map","CC map",800,800);
    TPad* padb = new TPad("padb","padb", 0., 0.55, 1., 1.);
    padb->Draw();
    TPad* padem = new TPad("padem","padem", 0., 0., 0.45, 0.45);
    padem->Draw();
    TPad* padep = new TPad("padep","padep", 0.55, 0., 1., 0.45);
    padep->Draw();

    TH2F* barrel = new TH2F("EB","EB Tower Status", 72, 0, 72, 34, -17, 17);
    TH2F* endc_p = new TH2F("EE+","EE+ Tower Status",22, 0, 22, 22, 0, 22);
    TH2F* endc_m = new TH2F("EE-","EE- Tower Status",22, 0, 22, 22, 0, 22);
    for(uint cellid = 0;
	cellid < EcalTrigTowerDetId::kEBTotalTowers;
	++cellid) {
      EcalTrigTowerDetId rawid = EcalTrigTowerDetId::detIdFromDenseIndex(cellid);
      if (object().find(rawid) == object().end()) continue;
      int ieta = rawid.ieta();
      if(ieta > 0) ieta--;   // 1 to 17
      int iphi = rawid.iphi() - 1;  // 0 to 71
      barrel->Fill(iphi, ieta, object()[rawid].getStatusCode());
    }
    for(uint cellid = 0;
	cellid < EcalTrigTowerDetId::kEETotalTowers;
	++cellid) {
      if(EcalScDetId::validHashIndex(cellid)) {
	EcalScDetId rawid = EcalScDetId::unhashIndex(cellid); 
	int ix = rawid.ix();  // 1 to 20
	int iy = rawid.iy();  // 1 to 20
	int side = rawid.zside();
	if(side == -1)
	  endc_m->Fill(ix, iy, object()[rawid].getStatusCode());
	else
	  endc_p->Fill(ix, iy, object()[rawid].getStatusCode());
      }
    }
    TLine* l = new TLine(0., 0., 0., 0.);
    l->SetLineWidth(1);
    padb->cd();
    barrel->SetStats(0);
    //    barrel->SetMaximum(14);
    //    barrel->SetMinimum(0);
    //    barrel->Draw("colz");
    barrel->Draw("col");
    for(int i = 0; i <17; i++) {
      Double_t x = 4.+ (i * 4);
      l = new TLine(x, -17., x, 17.);
      l->Draw();
    }
    l = new TLine(0., 0., 72., 0.);
    l->Draw();

    int ixSectorsEE[136] = {
       8,14,14,17,17,18,18,19,19,20,20,21,21,20,20,19,19,18,18,17,
      17,14,14, 8, 8, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 2, 2, 3, 3, 4,
       4, 5, 5, 8, 8, 8, 9, 9,10,10,12,12,13,13,12,12,10,10, 9, 9,
      10,10, 0,11,11, 0,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 0,12,13,
      13,14,14,15,15,16,16,17,17, 0, 9, 8, 8, 3, 3, 1, 0,13,14,14,
      19,19,21, 0, 9, 8, 8, 7, 7, 5, 5, 3, 3, 2, 0,13,14,14,15,15,
      17,17,19,19,20, 0,14,14,13,13,12,12,0};
    int iySectorsEE[136] = {
       1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 8, 8,14,14,17,17,18,18,19,19,
      20,20,21,21,20,20,19,19,18,18,17,17,14,14, 8, 8, 5, 5, 4, 4,
       3, 3, 2, 2, 1, 4, 4, 7, 7, 9, 9,10,10,12,12,13,13,12,12,10,
      10, 9, 0,13,21, 0,13,13,14,14,15,15,16,16,18,18,19, 0,13,13,
      14,14,15,15,16,16,18,18,19, 0,11,11,12,12,13,13, 0,11,11,12,
      12,13,13, 0,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 0,10,10, 9, 9, 8,
       8, 7, 7, 6, 6, 0, 2, 4, 4, 7, 7, 9, 0} ;
    padem->cd();
    endc_m->SetStats(0);
    endc_m->Draw("col");
    for ( int i = 0; i < 136; i=i+1) {
      if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
	   (ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
	l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
		    ixSectorsEE[i+1], iySectorsEE[i+1]);
      }
    }

    padep->cd();
    endc_p->SetStats(0);
    endc_p->Draw("col");
    for ( int i = 0; i < 136; i=i+1) {
      if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
	   (ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
	l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
		    ixSectorsEE[i+1], iySectorsEE[i+1]);
      }
    }

    canvas.SaveAs(filename.c_str());
    return filename;
  }
}


namespace condPython {
  template<>
  void defineWhat<EcalDQMTowerStatus>() {
    enum_<cond::ecalcond::How>("How")
      .value("singleChannel",cond::ecalcond::singleChannel)
      .value("bySuperModule",cond::ecalcond::bySuperModule) 
      .value("all",cond::ecalcond::all)
      ;

    typedef cond::ExtractWhat<EcalDQMTowerStatus> What;
    class_<What>("What",init<>())
      .def("set_how",&What::set_how)
      .def("set_which",&What::set_which)
      .def("how",&What::how, return_value_policy<copy_const_reference>())
      .def("which",&What::which, return_value_policy<copy_const_reference>())
      ;
  }
}

PYTHON_WRAPPER(EcalDQMTowerStatus,EcalDQMTowerStatus);
