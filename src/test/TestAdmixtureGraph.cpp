//
//  TestAdmixtureGraph.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TestAdmixtureGraph.h"


#include "AdmixtureEdgeCountPerBranch.h"
#include "AdmixtureConstantBirthDeathProcess.h"
#include "AdmixtureEdgeAdd.h"
#include "AdmixtureEdgeRemove.h"
#include "AdmixtureEdgeReweight.h"
#include "AdmixtureEdgeSlide.h"
#include "AdmixtureFixedNodeheightPruneRegraft.h"
#include "AdmixtureNearestNeighborInterchange.h"
#include "AdmixtureNodeTimeSlideUniform.h"
#include "AdmixtureCPPRateScaleMove.h"
#include "AdmixtureDelayDecrement.h"
#include "AdmixtureTree.h"
#include "BrownianMotionAdmixtureGraph.h"
#include "BrownianMotionAdmixtureGraphResiduals.h"
#include "CharacterData.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "ContinuousCharacterState.h"
#include "DeterministicNode.h"
#include "DynamicNode.h"
#include "ExponentialDistribution.h"
#include "ExtendedNewickTreeMonitor.h"
#include "ExtendedNewickAdmixtureTreeMonitor.h"
#include "FileMonitor.h"
#include "GammaDistribution.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "PathSampleMonitor.h"
#include "PathResampleMove.h"
#include "PathValueScalingMove.h"
#include "PopulationDataReader.h"
#include "RbFileManager.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "SnpData.h"
#include "StochasticNode.h"
#include "TreeLengthStatistic.h"
#include "TreeAdmixtureEventCount.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

using namespace RevBayesCore;

TestAdmixtureGraph::TestAdmixtureGraph(const std::string &sfn, int gen) : snpFilename( sfn ), mcmcGenerations( gen )
{
    ;
}

TestAdmixtureGraph::~TestAdmixtureGraph(void)
{
    ;
}

bool TestAdmixtureGraph::run(void) {
    
    // MODEL GRAPH
    
    // read in data
   // SnpData* snps = PopulationDataReader().readSnpData(snpFilename);
    SnpData* snps = PopulationDataReader().readSnpData2(snpFilename);
    size_t numTaxa = snps->getNumPopulations();
    size_t numNodes = 2 * numTaxa - 1;
    size_t numBranches = numNodes - 1;
    size_t numSites = snps->getNumSnps();
    // std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
       
    // BM diffusion rate
    ConstantNode<double>* a = new ConstantNode<double>( "a", new double(pow(10,0)));
    StochasticNode<double>* diffusionRate = new StochasticNode<double> ("diffusionRate", new ExponentialDistribution(a));
    diffusionRate->setValue(new double(0.01));
    
    // admixture CPP rate
//    ConstantNode<double>*
    ConstantNode<double>* c = new ConstantNode<double>( "c", new double(pow(10,6)));//1.0 / snps->getNumPopulations())); // pow(10,0))); // admixture rate prior
    StochasticNode<double>* admixtureRate = new StochasticNode<double> ("admixtureRate", new ExponentialDistribution(c));
  //  StochasticNode<double>* admixtureRate = new StochasticNode<double> ("admixtureRate", new UniformDistribution(new ConstantNode<double>("ar_lower", new double(0.0)), new ConstantNode<double>("ar_upper", new double(5.0))));
    admixtureRate->setValue(new double(1.0E-7));
    
    // birth-death process for ultrametric tree
    StochasticNode<double>* diversificationRate = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(100.0)) ));
    ConstantNode<double>* turnover = new ConstantNode<double>("turnover", new double(0.0));
    diversificationRate->setValue(new double(1.0));
    
    // tree node
    StochasticNode<AdmixtureTree>* tau = new StochasticNode<AdmixtureTree>( "tau", new AdmixtureConstantBirthDeathProcess(diversificationRate, turnover, numTaxa, snps->getPopulationNames()) );
    //  tau->touch();
    //  tau->keep();
    
    // tree length (for verifying CPP)
    DeterministicNode<double>* treeLength = new DeterministicNode<double>("TreeLength", new TreeLengthStatistic<AdmixtureTree>(tau) );
    DeterministicNode<size_t>* treeAdmixtureEventCount = new DeterministicNode<size_t>("TreeAdmixtureEventCount", new TreeAdmixtureEventCount(tau) );
    
    // branch multipliers (mutation rate is clocklike, but population sizes are not)
	std::vector<const TypedDagNode<double> *> branchRates;
    std::vector< ContinuousStochasticNode *> branchRates_nonConst;
    ConstantNode<double>* branchRateA = new ConstantNode<double>( "branchRateA", new double(4.0));
    ConstantNode<double>* branchRateB = new ConstantNode<double>( "branchRateB", new double(4.0));
	for( int i=0; i<numBranches; i++){
        std::ostringstream br_name;
        br_name << "br(" << i << ")";
        ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new GammaDistribution(branchRateA, branchRateB));
        tmp_branch_rate->setValue(new double(1.0));
		branchRates.push_back( tmp_branch_rate );
        branchRates_nonConst.push_back( tmp_branch_rate );
	}
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchRates ) );


    // tracks residuals
    
    /*
    DeterministicNode<std::vector<double> >* residuals = new DeterministicNode< std::vector<std::vector<double> > >( );
    */
    
    // tracks admixture event statistics
    size_t numResults = 10;
    int delay = 1000;
    ConstantNode<int>* delayTimer = new ConstantNode<int>( "delayTimer", new int(delay));
    
    DeterministicNode<std::map<std::vector<bool>, std::map<std::vector<bool>, std::vector<AdmixtureNode*> > > >* ae;
    ae = new DeterministicNode<std::map<std::vector<bool>, std::map<std::vector<bool>, std::vector<AdmixtureNode*> > > >( "ae_crown", new AdmixtureEdgeCountPerBranch(tau, delayTimer, numResults) );
    
    // model node
    BrownianMotionAdmixtureGraph* bmag = new BrownianMotionAdmixtureGraph( tau, diffusionRate, admixtureRate, br_vector, snps );
    StochasticNode<CharacterData<ContinuousCharacterState> >* admixtureModel;
    admixtureModel = new StochasticNode<CharacterData<ContinuousCharacterState> >("AdmixtureGraph", bmag);
    
    // have to clamp to distinguish likelihood from prior (incidentally calls setValue(), but this is handled otherwise)
   // admixtureModel->clamp( new CharacterData<ContinuousCharacterState>() ); // does it event matter how it's clamped?
    
    
//    BrownianMotionAdmixtureGraph* b = static_cast<BrownianMotionAdmixtureGraph*>(&admixtureModel->getDistribution());
    
    // residuals
//    DeterministicNode<std::vector<double> >* res2 = new DeterministicNode<std::vector<double> >("res2", new BrownianMotionAdmixtureGraphResiduals(admixtureModel));
    DeterministicNode<std::vector<double> >* residuals = new DeterministicNode<std::vector<double> >("residuals", new BrownianMotionAdmixtureGraphResiduals(admixtureModel));
    //res2->addParent(admixtureModel);
    
    // MOVES
    std::cout << "Adding moves\n";
    
    // moves vector
    std::vector<Move*> moves;
    
    // model parameters
    moves.push_back( new ScaleMove(diffusionRate, 1.0, true, 5.0) );
    moves.push_back( new ScaleMove(diversificationRate, 1.0, true, 2.0) );
    
    for( int i=0; i<numBranches; i++){
        moves.push_back( new ScaleMove(branchRates_nonConst[i], 1.0, true, 1.0) );
	}
    
    // non-admixture tree updates
    moves.push_back( new AdmixtureNearestNeighborInterchange( tau, 15.0 ) );
    moves.push_back( new AdmixtureNodeTimeSlideUniform( tau, 15.0 ) );

    // admixture tree updates
    moves.push_back( new AdmixtureDelayDecrement( delayTimer, 1.0) );
    moves.push_back( new AdmixtureEdgeRemove( tau, admixtureRate, delayTimer, 1.0) );
    moves.push_back( new AdmixtureCPPRateScaleMove( tau, admixtureRate, 1.0, true, 2.0));
    moves.push_back( new AdmixtureEdgeAdd( tau, admixtureRate, delayTimer, 1.0) );
    moves.push_back( new AdmixtureEdgeReweight( tau, 5.0, 1.0) );
    //moves.push_back( new AdmixtureEdgeSlide( tau, 10.0, 1.0) );


    // moves to implement
    // subtree rescale (constrained by admixture events)
    // beta-slide of admixture event along treeheight
    // reverse admixture event polarity
    
    // ... can remove
    // moves.push_back( new AdmixtureTreeScale( tau, 1.0, true, 2.0 ) );
    // moves.push_back( new AdmixtureRootTimeSlide( tau, 1.0, true, 5.0 ) );
    
    // MONITORS
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;
    
    // parameter monitor
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( diffusionRate );
    monitoredNodes.insert( admixtureRate );
    monitoredNodes.insert( diversificationRate );
    monitoredNodes.insert( treeLength );
    monitoredNodes.insert( treeAdmixtureEventCount ) ;
    for( int i=0; i<numBranches; i++){
        ;    monitoredNodes.insert( branchRates_nonConst[i] );
	}
    monitors.push_back( new FileMonitor( monitoredNodes, 1, "/Users/mlandis/data/admix/output/TestAdmixtureGraphParameters5.txt", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 1, "\t" ) );
    
    // tree monitor
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 1, "/Users/mlandis/data/admix/output/TestAdmixtureGraphTree5.tree", "\t", false, false, false ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes2, 1, "\t" ) );
    
    // monitors to implement
  //  std::set<DagNode*> monitoredNodes3;
  //ext  monitoredNodes3.insert( tau );
    monitors.push_back( new ExtendedNewickAdmixtureTreeMonitor( tau, br_vector, 1, "/Users/mlandis/data/admix/output/TestAdmixtureGraphFull5.tree", "\t", false, false, false ) );
    
    
    // MODEL
    std::cout << "Calling model\n";
    
    Model myModel = Model(admixtureRate);
    
    
    
    // MCMC
    std::cout << "Calling mcmc\n";
    
    Mcmc myMcmc = Mcmc(myModel, moves, monitors);
    myMcmc.run(100000);//000);//mcmcGenerations);
    
    std::cout << "All done...\n";
    
    // OBJECT CLEANUP
    delete snps;
    delete a;
    delete c;
    delete tau;
    delete diversificationRate;
    delete turnover;
    delete treeLength;
    delete diffusionRate;
    delete admixtureRate;
    
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
    
    return true;
}