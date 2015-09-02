#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/StGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TopGenEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtFullLeptonicEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiLeptonicEvent.h"
#include "AnalysisDataFormats/TopObjects/interface/TtFullHadronicEvent.h"


#include "AnalysisDataFormats/TopObjects/interface/StEvtSolution.h"
#include "AnalysisDataFormats/TopObjects/interface/TtDilepEvtSolution.h"
#include "AnalysisDataFormats/TopObjects/interface/TtHadEvtSolution.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"

#include "DataFormats/Common/interface/Wrapper.h"
#include "TString.h"

namespace AnalysisDataFormats_TopObjects {
  struct dictionary {
    TtGenEvent ttgen;
    StGenEvent stgen;
    TopGenEvent topgen;
    TtEvent ttevt;
    TtFullLeptonicEvent ttfulllepevt;
    TtSemiLeptonicEvent ttsemilepevt;
    TtFullHadronicEvent ttfullhadevt;
    edm::Wrapper<TtGenEvent> w_ttgen;
    edm::Wrapper<StGenEvent> w_stgen;
    edm::Wrapper<TopGenEvent> w_topgen;
    edm::Wrapper<TtFullLeptonicEvent> w_ttfulllepevt;
    edm::Wrapper<TtSemiLeptonicEvent> w_ttsemilepevt;
    edm::Wrapper<TtFullHadronicEvent> w_ttfullhadevt;
    edm::Wrapper<reco::CompositeCandidate> ttcompcand;

    edm::RefProd<TtGenEvent> rp_ttgen;
    edm::RefProd<StGenEvent> rp_stgen;
    edm::RefProd<TopGenEvent> rp_topgen;
    edm::RefProd<TtFullLeptonicEvent> rp_ttfulllepevt;
    edm::RefProd<TtSemiLeptonicEvent> rp_ttsemilepevt;
    edm::RefProd<TtFullHadronicEvent> rp_ttfullhadevt;

    std::pair<WDecay::LeptonType, WDecay::LeptonType> p_lepdecay_lepdecay;
    std::pair<reco::CompositeCandidate, std::vector<int> > p_compcand_vint;
    std::vector<std::pair<reco::CompositeCandidate, std::vector<int> > > v_p_compcand_vint;
    edm::Wrapper<std::vector<std::pair<reco::CompositeCandidate, std::vector<int> > > > w_v_p_compcand_vint;

    std::map<TtEvent::HypoClassKey, int> m_key_int;
    std::map<TtEvent::HypoClassKey, std::vector<std::pair<reco::CompositeCandidate, std::vector<int> > > > m_key_v_p_compcand_vint;

    TtDilepEvtSolution ttdilep;
    TtSemiEvtSolution ttsemi;
    TtHadEvtSolution tthad;
    StEvtSolution st;
    std::vector<TtDilepEvtSolution> v_ttdilep;
    std::vector<TtSemiEvtSolution> v_ttsemi;
    std::vector<TtHadEvtSolution> v_tthad;
    std::vector<StEvtSolution> v_st;
    edm::Wrapper<std::vector<TtDilepEvtSolution> > w_v_ttdilep;
    edm::Wrapper<std::vector<TtSemiEvtSolution> > w_v_ttsemi;
    edm::Wrapper<std::vector<TtHadEvtSolution> > w_v_tthad;
    edm::Wrapper<std::vector<StEvtSolution> > w_v_st;   

  };
}
