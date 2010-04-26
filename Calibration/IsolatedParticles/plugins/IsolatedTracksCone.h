// -*- C++ -*-

#ifndef CalibrationIsolatedParticlesIsolatedTracksCone_h
#define CalibrationIsolatedParticlesIsolatedTracksCone_h

// system include files
#include <memory>
#include <cmath>
#include <string>
#include <map>
#include <vector>

// user include files
#include <Math/GenVector/VectorUtil.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
// muons and tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
// Vertices
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//L1 objects
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
// SimHit
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
//simtrack
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

// track associator
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

// ecal / hcal
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "Calibration/IsolatedParticles/interface/FindCaloHitCone.h"
#include "Calibration/IsolatedParticles/interface/FindCaloHit.h"

#include "Calibration/IsolatedParticles/interface/eECALMatrix.h"
#include "Calibration/IsolatedParticles/interface/eHCALMatrix.h"
#include "Calibration/IsolatedParticles/interface/eCone.h"
#include "Calibration/IsolatedParticles/interface/MatchingSimTrack.h"
#include "Calibration/IsolatedParticles/interface/CaloSimInfo.h"

// root objects
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TDirectory.h"
#include "TTree.h"

#include <cmath>

class IsolatedTracksCone : public edm::EDAnalyzer {
public:
  explicit IsolatedTracksCone(const edm::ParameterSet&);
  ~IsolatedTracksCone();
  
  //  static const int NEtaBins = 4;
  static const int NEtaBins = 4;
  static const int NPTBins  = 21;
  static const int NPBins   = 21;
  
  double genPartPBins[22], genPartEtaBins[5];
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void printTrack(const reco::Track* pTrack);

  //   void BookHistograms();
  void BuildTree();
  void clearTrackVectors();

  double DeltaPhi(double v1, double v2);
  double DeltaR(double eta1, double phi1, double eta2, double phi2);


  int conechargeIsolation(const GlobalPoint& hpoint1, 
			  const GlobalPoint& point2, 
			  const GlobalVector& trackMom,
			  double dR);


  double conechargeIsolation(const edm::Event& iEvent, 
			     const edm::EventSetup& iSetup, 
			     reco::TrackCollection::const_iterator trkItr, 
			     edm::Handle<reco::TrackCollection> trkCollection, 
			     int &nNearTRKs,
			     int &nLayers_maxNearP,
			     int &trkQual_maxNearP,
			     double &maxNearP_goodTrk,
			     const GlobalPoint& hpoint1,
			     const GlobalVector& trackMom,
			     double dR);
  
  
  int    debugTrks_;
  bool   printTrkHitPattern_;
  bool   doMC;
  int    myverbose_;
  bool   useJetTrigger_;
  double drLeadJetVeto_, ptMinLeadJet_;
  edm::InputTag _L1extraTauJetSource, _L1extraCenJetSource, _L1extraFwdJetSource;

  double minTrackP_, maxTrackEta_, maxNearTrackP_;
  
  int    debugEcalSimInfo_;

  bool   applyEcalIsolation_;

  // track associator to detector parameters 
  TrackAssociatorParameters parameters_;
  mutable TrackDetectorAssociator* trackAssociator_;

  TTree* ntp;

  TH1F* hRawPt ;
  TH1F* hRawP  ;
  TH1F* hRawEta;
  TH1F* hRawPhi;

  int nRawTRK            ;
  int nFailHighPurityQaul;
  int nFailPt            ;
  int nFailEta           ;
  int nMissEcal          ;
  int nMissHcal          ;
  int nEVT;
  int nEVT_failL1;
  int nTRK;
//   double hbScale;
//   double heScale;
  double leadL1JetPT;
  double leadL1JetEta;
  double leadL1JetPhi;
 
  std::vector<std::vector<int> >*    t_v_hnNearTRKs       ; 
  std::vector<std::vector<int> >*    t_v_hnLayers_maxNearP; 
  std::vector<std::vector<int> >*    t_v_htrkQual_maxNearP; 
  std::vector<std::vector<double> >* t_v_hmaxNearP_goodTrk;
  std::vector<std::vector<double> >* t_v_hmaxNearP        ;    

  std::vector<std::vector<int> >*    t_v_cone_hnNearTRKs       ; 
  std::vector<std::vector<int> >*    t_v_cone_hnLayers_maxNearP; 
  std::vector<std::vector<int> >*    t_v_cone_htrkQual_maxNearP; 
  std::vector<std::vector<double> >* t_v_cone_hmaxNearP_goodTrk;
  std::vector<std::vector<double> >* t_v_cone_hmaxNearP        ;    

  //  std::vector<double>* t_hScale           ;
  std::vector<double>* t_trkNOuterHits    ;
  std::vector<double>* t_trkNLayersCrossed;
  std::vector<double>* t_dtFromLeadJet    ;
  std::vector<double>* t_trkP             ;
  std::vector<double>* t_simP             ;
  std::vector<double>* t_trkPt            ;
  std::vector<double>* t_trkEta           ;
  std::vector<double>* t_trkPhi           ;
  std::vector<double>* t_e3x3             ;

  std::vector<std::vector<double> > *t_v_eDR;
  std::vector<std::vector<double> > *t_v_eMipDR;

  std::vector<double>* t_h3x3             ;
  std::vector<double>* t_h5x5             ;
  std::vector<double>* t_hsim3x3          ;
  std::vector<double>* t_hsim5x5          ;

  std::vector<double>* t_nRH_h3x3         ;
  std::vector<double>* t_nRH_h5x5         ;
  std::vector<double>* t_nRH_h3x3dR       ;
  std::vector<double>* t_nRH_h5x5dR       ;
  std::vector<double>* t_nRH_h7x7dR       ;
  std::vector<double>* t_nRH_h9x9dR       ;
  std::vector<double>* t_nRH_h40cm        ;

  std::vector<double>* t_nRH_hsim3x3dR    ;
  std::vector<double>* t_nRH_hsim5x5dR    ;
  std::vector<double>* t_nRH_hsim7x7dR    ;
  std::vector<double>* t_nRH_hsim9x9dR    ;
  std::vector<double>* t_nRH_hsim40cm     ;

  std::vector<double>* t_hsim3x3Matched   ;
  std::vector<double>* t_hsim5x5Matched   ;
  std::vector<double>* t_hsim3x3Rest      ;
  std::vector<double>* t_hsim5x5Rest      ;
  std::vector<double>* t_hsim3x3Photon    ;
  std::vector<double>* t_hsim5x5Photon    ;
  std::vector<double>* t_hsim3x3NeutHad   ;
  std::vector<double>* t_hsim5x5NeutHad   ;
  std::vector<double>* t_hsim3x3CharHad   ;
  std::vector<double>* t_hsim5x5CharHad   ;
  std::vector<double>* t_hsim3x3PdgMatched;
  std::vector<double>* t_hsim5x5PdgMatched;
  std::vector<double>* t_hsim3x3Total     ;
  std::vector<double>* t_hsim5x5Total     ;

  std::vector<int>* t_hsim3x3NMatched   ;
  std::vector<int>* t_hsim3x3NRest      ;
  std::vector<int>* t_hsim3x3NPhoton    ;
  std::vector<int>* t_hsim3x3NNeutHad   ;
  std::vector<int>* t_hsim3x3NCharHad   ;
  std::vector<int>* t_hsim3x3NTotal     ;

  std::vector<int>* t_hsim5x5NMatched   ;
  std::vector<int>* t_hsim5x5NRest      ;
  std::vector<int>* t_hsim5x5NPhoton    ;
  std::vector<int>* t_hsim5x5NNeutHad   ;
  std::vector<int>* t_hsim5x5NCharHad   ;
  std::vector<int>* t_hsim5x5NTotal     ;

  std::vector<double>* t_distFromHotCell_h3x3;
  std::vector<int>* t_ietaFromHotCell_h3x3;
  std::vector<int>* t_iphiFromHotCell_h3x3;
  std::vector<double>* t_distFromHotCell_h5x5;
  std::vector<int>* t_ietaFromHotCell_h5x5;
  std::vector<int>* t_iphiFromHotCell_h5x5;

  std::vector<double>* t_trkHcalEne       ;
  std::vector<double>* t_trkEcalEne       ;
  
  std::vector<std::vector<double> >* t_v_hsimInfoConeMatched   ;
  std::vector<std::vector<double> >* t_v_hsimInfoConeRest      ;
  std::vector<std::vector<double> >* t_v_hsimInfoConePhoton     ;
  std::vector<std::vector<double> >* t_v_hsimInfoConeNeutHad   ;
  std::vector<std::vector<double> >* t_v_hsimInfoConeCharHad   ;
  std::vector<std::vector<double> >* t_v_hsimInfoConePdgMatched;
  std::vector<std::vector<double> >* t_v_hsimInfoConeTotal     ;

  std::vector<std::vector<int> >* t_v_hsimInfoConeNMatched   ;
  std::vector<std::vector<int> >* t_v_hsimInfoConeNRest      ;
  std::vector<std::vector<int> >* t_v_hsimInfoConeNPhoton     ;
  std::vector<std::vector<int> >* t_v_hsimInfoConeNNeutHad   ;
  std::vector<std::vector<int> >* t_v_hsimInfoConeNCharHad   ;
  std::vector<std::vector<int> >* t_v_hsimInfoConeNTotal     ;

  std::vector<std::vector<double> >* t_v_hsimCone    ;
  std::vector<std::vector<double> >* t_v_hCone       ;
  std::vector<std::vector<int> >*    t_v_nRecHitsCone;
  std::vector<std::vector<int> >*    t_v_nSimHitsCone;

  std::vector<std::vector<double> >*    t_v_distFromHotCell;
  std::vector<std::vector<int> >*    t_v_ietaFromHotCell;
  std::vector<std::vector<int> >*    t_v_iphiFromHotCell;


  std::vector<std::vector<int> >*    t_v_hlTriggers;
  std::vector<int>* t_hltHB;
  std::vector<int>* t_hltHE;
  std::vector<int>* t_hltL1Jet15		    	;
  std::vector<int>* t_hltJet30		    	;
  std::vector<int>* t_hltJet50		    	;
  std::vector<int>* t_hltJet80		    	;
  std::vector<int>* t_hltJet110		    	;
  std::vector<int>* t_hltJet140		    	;
  std::vector<int>* t_hltJet180		    	;
  std::vector<int>* t_hltL1SingleEG5		;
  std::vector<int>* t_hltZeroBias		;
  std::vector<int>* t_hltMinBiasHcal		;
  std::vector<int>* t_hltMinBiasEcal		;
  std::vector<int>* t_hltMinBiasPixel	    	;
  std::vector<int>* t_hltSingleIsoTau30_Trk5	;
  std::vector<int>* t_hltDoubleLooseIsoTau15_Trk5;
  

  std::vector<std::vector<int> >*      t_v_RH_h3x3_ieta;
  std::vector<std::vector<int> >*      t_v_RH_h3x3_iphi;
  std::vector<std::vector<double> >*   t_v_RH_h3x3_ene ;
  std::vector<std::vector<int> >*      t_v_RH_h5x5_ieta;
  std::vector<std::vector<int> >*      t_v_RH_h5x5_iphi;
  std::vector<std::vector<double> >*   t_v_RH_h5x5_ene ;
  std::vector<std::vector<int> >*      t_v_RH_r26_ieta ;
  std::vector<std::vector<int> >*      t_v_RH_r26_iphi ;
  std::vector<std::vector<double> >*   t_v_RH_r26_ene  ;
  std::vector<std::vector<int> >*      t_v_RH_r44_ieta ;
  std::vector<std::vector<int> >*      t_v_RH_r44_iphi ;
  std::vector<std::vector<double> >*   t_v_RH_r44_ene  ;

  std::vector<unsigned int>* t_irun;
  std::vector<unsigned int>* t_ievt;
  std::vector<unsigned int>* t_ilum;
  

  edm::Service<TFileService> fs;

  

};


#endif
