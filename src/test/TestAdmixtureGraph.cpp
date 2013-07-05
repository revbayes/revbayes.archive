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
#include "AdmixtureEdgeAddResidualWeights.h"
#include "AdmixtureEdgeRemoveResidualWeights.h"
#include "AdmixtureEdgeReplaceResidualWeights.h"
#include "AdmixtureEdgeReversePolarity.h"
#include "AdmixtureEdgeReweight.h"
#include "AdmixtureEdgeSlide.h"
#include "AdmixtureFixedNodeheightPruneRegraft.h"
#include "AdmixtureNearestNeighborInterchangeAndRateShift.h"
#include "AdmixtureNodeTimeSlideBeta.h"
#include "AdmixtureCPPRateScaleMove.h"
#include "AdmixtureDelayDecrement.h"
#include "AdmixtureBipartitionMonitor.h"
#include "AdmixtureResidualsMonitor.h"
#include "AdmixtureShiftNodeAgeAndRate.h"
#include "AdmixtureNarrowExchange.h"
#include "AdmixtureEdgeDivergenceMerge.h"
#include "AdmixtureTree.h"
#include "AdmixtureTreeLengthStatistic.h"
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
#include "InverseGammaDistribution.h"
#include "LognormalDistribution.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "ParallelMcmcmc.h"
#include "PathSampleMonitor.h"
#include "PathResampleMove.h"
#include "PathValueScalingMove.h"
#include "PopulationDataReader.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbFileManager.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "SnpData.h"
#include "StochasticNode.h"
#include "TickFunction.h"

#include "TreeAdmixtureEventCount.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include <iomanip>

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
    SnpData* snps = PopulationDataReader().readSnpData2(snpFilename,1);
    size_t numTaxa = snps->getNumPopulations();
    size_t numNodes = 2 * numTaxa - 1;
    size_t numBranches = numNodes - 1;
    size_t numSites = snps->getNumSnps();
    int blockSize = 500;
    
    int delay = 4000;
    size_t numTreeResults = 500;
    size_t numAdmixtureResults = 500;
    int maxNumberOfAdmixtureEvents = 10;
    
    bool useWishart = true;             // if false, the composite likelihood function is used
    bool useBias = !true;               // if false, no covariance bias correction for small sample size is used
    bool useAdmixtureEdges = true;      // if false, no admixture moves or edges are used
    bool useBranchRates = true;         // if false, all populations are of the same size
    bool allowSisterAdmixture = true;   // if false, admixture events cannot be between internal lineages who share a divergence parent
    bool discardNonPosDefMtx = true;    // if false, round negative eigenvalues to positive eps
    bool useContrasts = false;          // nothing really, need to remove
    bool updateParameters = true;
    bool updateTree = true;
    
    bool useParallelMcmcmc = true;
    int numChains = 8;
    int numProcesses = numChains;
    int swapInterval = 10;
    double deltaTemp = 0.2;
    
    
    std::stringstream rndStr;
    rndStr << std::setw(6) << std::setfill('0') << std::floor(GLOBAL_RNG->uniform01()*1e6);
    std::string outName = "w_clovis." + rndStr.str();
    
    // std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
       
    // BM diffusion rate
    ConstantNode<double>* a_bm = new ConstantNode<double>( "bm_a", new double(3));
    ConstantNode<double>* b_bm = new ConstantNode<double>( "bm_b", new double(2));
    StochasticNode<double>* diffusionRate = new StochasticNode<double> ("rate_BM", new ExponentialDistribution(b_bm));
    
    // admixture CPP rate
    // possible formula for data-appropriate CPP prior
    // double cpp_prior = pow(10,2*numSites); // ... but really, probably impossible... better off using MCMCMC
    double cpp_prior = pow(10,-2);
    ConstantNode<double>* c = new ConstantNode<double>( "c", new double(cpp_prior)); // admixture rate prior
    StochasticNode<double>* admixtureRate = new StochasticNode<double> ("rate_CPP", new ExponentialDistribution(c));
    admixtureRate->setValue(new double(1.0 / cpp_prior));
    
    // birth-death process for ultrametric tree
    StochasticNode<double>* diversificationRate = new StochasticNode<double>("div", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(0.01)) ));
    StochasticNode<double>* turnover = new StochasticNode<double>("turnover", new UniformDistribution(new ConstantNode<double>("do_lower", new double(0.0)), new ConstantNode<double>("do_upper", new double(1.0)) ));
    
    //diversificationRate->setValue(new double(1.0));
    
    // tree node
    StochasticNode<AdmixtureTree>* tau = new StochasticNode<AdmixtureTree>( "tau", new AdmixtureConstantBirthDeathProcess(diversificationRate, turnover, numTaxa, snps->getPopulationNames(), snps->getOutgroup()) );
    //  tau->touch();
    //  tau->keep();
    
    // tree length (for verifying CPP)
    DeterministicNode<double>* treeLength = new DeterministicNode<double>("TreeLength", new AdmixtureTreeLengthStatistic(tau) );
    DeterministicNode<size_t>* treeAdmixtureEventCount = new DeterministicNode<size_t>("TreeAdmixtureEventCount", new TreeAdmixtureEventCount(tau) );
    
    // branch multipliers (mutation rate is clocklike, but population sizes are not)
	std::vector<const TypedDagNode<double> *> branchRates;
    std::vector< ContinuousStochasticNode *> branchRates_nonConst;
    ConstantNode<double>* branchRateA = new ConstantNode<double>( "branchRateA", new double(5));
    ConstantNode<double>* branchRateB = new ConstantNode<double>( "branchRateB", new double(4));
	for( int i=0; i<numBranches; i++){

        std::ostringstream br_name;
        br_name << "br_" << i;
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new ExponentialDistribution(branchRateA) );
        ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new InverseGammaDistribution(branchRateA, branchRateB));
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new GammaDistribution(branchRateA, branchRateB));
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode(br_name.str(), new LognormalDistribution(branchRateA, branchRateB));
        tmp_branch_rate->setValue(new double(1.0));
		branchRates.push_back( tmp_branch_rate );
        branchRates_nonConst.push_back( tmp_branch_rate );
	}
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchRates ) );

    // delay addition of admixture events by some n MCMC cycles
//    ConstantNode<int>* delayTimer = new ConstantNode<int>( "timer", new int(delay));


    // tracks admixture event statistics
    //DeterministicNode<std::map<std::vector<bool>, std::map<std::vector<bool>, std::vector<AdmixtureNode*> > > >* ae;
    //ae = new DeterministicNode<std::map<std::vector<bool>, std::map<std::vector<bool>, std::vector<AdmixtureNode*> > > >( "ae_crown", new AdmixtureEdgeCountPerBranch(tau, delayTimer, numTreeResults) );
    
    // model node
    BrownianMotionAdmixtureGraph* bmag = new BrownianMotionAdmixtureGraph( tau, diffusionRate, admixtureRate, br_vector, snps, useWishart, useContrasts, useBias, discardNonPosDefMtx, blockSize );
    StochasticNode<CharacterData<ContinuousCharacterState> >* admixtureModel;
    admixtureModel = new StochasticNode<CharacterData<ContinuousCharacterState> >("AdmixtureGraph", bmag);
    
    // have to clamp to distinguish likelihood from prior (incidentally calls setValue(), but this is handled otherwise)
    admixtureModel->clamp( new CharacterData<ContinuousCharacterState>() ); // does it event matter how it's clamped?
    
    // residuals
    DeterministicNode<std::vector<double> >* residuals = new DeterministicNode<std::vector<double> >("residuals", new BrownianMotionAdmixtureGraphResiduals(admixtureModel));
    ConstantNode<int>* delayTimer = new ConstantNode<int>( "timer", new int(delay));
    
    // MOVES
    std::cout << "Adding moves\n";
    
    // moves vector
    std::vector<Move*> moves;
   
    

    // model parameters
    if (updateParameters)
    {
        moves.push_back( new ScaleMove(diffusionRate, 1.0, true, 5.0) );
        moves.push_back( new ScaleMove(diversificationRate, 1.0, true, 5.0) );
        moves.push_back( new ScaleMove(turnover, 1.0, true, 5.0) );
    }
    
    
    // non-admixture tree updates
    if (updateTree)
    {
        moves.push_back( new AdmixtureNarrowExchange( tau, 1.0, numTaxa) );
        moves.push_back( new AdmixtureFixedNodeheightPruneRegraft(tau, numTaxa/2));
        for (size_t i = numTaxa; i < numNodes - 1; i++)
            moves.push_back( new AdmixtureNodeTimeSlideBeta( tau, (int)i, 1.0, false, 1.0 ) );
    }
    
    
    // branch rate updates
    if (useBranchRates)
    {
        // branch rate multipliers
        for( int i=0; i < numBranches; i++)
            moves.push_back( new ScaleMove(branchRates_nonConst[i], 1.0, true, 1.0) );
        
        // shift node age for branch rate
        for (size_t i = numTaxa; i < numNodes - 1; i++)
            ;//moves.push_back( new AdmixtureShiftNodeAgeAndRate(tau, branchRates_nonConst, (int)i, 1.0, true, 1.0) );
        
        // NNI with branch rate modifier (not working quite right, disabled)
        moves.push_back( new AdmixtureNearestNeighborInterchangeAndRateShift( tau, branchRates_nonConst, 3.0, false, numTaxa));
    }
    
    moves.push_back( new AdmixtureDelayDecrement( delayTimer, 1.0) );
    
    // admixture tree updates
    if (useAdmixtureEdges)
    {
        
        
        moves.push_back( new AdmixtureEdgeAddResidualWeights( tau, admixtureRate, residuals, delayTimer, maxNumberOfAdmixtureEvents, allowSisterAdmixture, 2.0) );
        moves.push_back( new AdmixtureEdgeRemoveResidualWeights( tau, admixtureRate, residuals, delayTimer, 2.0) );
        moves.push_back( new AdmixtureEdgeReplaceResidualWeights( tau, admixtureRate, residuals, delayTimer, allowSisterAdmixture, 10.0) );
        moves.push_back( new AdmixtureEdgeDivergenceMerge( tau, admixtureRate, residuals, delayTimer, allowSisterAdmixture, 5.0 ));
     
        moves.push_back( new AdmixtureEdgeReweight( tau, 2.0, 5.0) );
        moves.push_back( new AdmixtureEdgeReversePolarity( tau, 2.0, 5.0) );
        moves.push_back( new AdmixtureEdgeSlide( tau, allowSisterAdmixture, 2.0, 5.0) );
        moves.push_back( new AdmixtureCPPRateScaleMove( tau, admixtureRate, cpp_prior, 1.0, false, 2.0));
        
    }
    
    // dream moves...
    // scale branch rates & add/delete admixture edge
    // root rotate w/ subtree scale
    
    // MONITORS
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;
    
    // parameter monitor
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( diffusionRate );
    monitoredNodes.insert( admixtureRate );
    monitoredNodes.insert( diversificationRate );
    monitoredNodes.insert( turnover );
    monitoredNodes.insert( treeLength );
    monitoredNodes.insert( treeAdmixtureEventCount ) ;
    for( int i=0; i<numBranches; i++){
        monitoredNodes.insert( branchRates_nonConst[i] );
	}
    
   
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "/Users/mlandis/data/admix/output/" + outName + ".parameters.txt", "\t", true, true, true, true, true ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 1, "\t" ) );
 
    monitors.push_back( new ExtendedNewickAdmixtureTreeMonitor( tau, br_vector, true, 10, "/Users/mlandis/data/admix/output/" + outName + ".admixture_trees.txt", "\t", true, true, true, true ) );
    
    monitors.push_back( new AdmixtureBipartitionMonitor(tau, br_vector, delayTimer, numTreeResults, numAdmixtureResults, 10, "/Users/mlandis/data/admix/output/" + outName + ".bipartitions.txt", "\t", true, true, true, true ) );
    
    monitors.push_back( new AdmixtureResidualsMonitor(residuals, snps->getPopulationNames(), 10, "/Users/mlandis/data/admix/output/" + outName + ".residuals.txt", "\t", true, true, true, true ) );

    monitors.push_back( new ExtendedNewickAdmixtureTreeMonitor( tau, br_vector, false, 10, "/Users/mlandis/data/admix/output/" + outName + ".topology_trees.trees", "\t", true, true, true, true ) );
    
    std::set<DagNode*> mn2;
    mn2.insert(tau);
    monitors.push_back( new FileMonitor( mn2, 10,"/Users/mlandis/data/admix/output/" + outName + ".time_trees.trees", "\t", true, true, true, true ));
    
    
    // MODEL
    std::cout << "Calling model\n";
    std::set<const DagNode*> mset;
    mset.insert(admixtureRate);
    mset.insert(delayTimer);
    Model myModel = Model(mset);
    
    // MCMC
    std::cout << "Calling mcmc\n";
    
    if (!useParallelMcmcmc)
    {
        double scaleFactorConstant = 2000.0; // make this smaller to flatten the likelihood surface
        double chainHeat = scaleFactorConstant / numSites;
        Mcmc myMcmc = Mcmc(myModel, moves, monitors, true, chainHeat);
        myMcmc.run(100000);
        myMcmc.printOperatorSummary();
    }
    else
    {
        ParallelMcmcmc myPmc3(myModel, moves, monitors, numChains, numProcesses, swapInterval, deltaTemp);
        myPmc3.run(100000);
        myPmc3.printOperatorSummary();
    }

    std::cout << "All done...\n";
    
    // OBJECT CLEANUP
    delete snps;
    delete a_bm;
    delete b_bm;
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