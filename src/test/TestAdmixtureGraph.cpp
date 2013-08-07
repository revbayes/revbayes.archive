//
//  TestAdmixtureGraph.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <algorithm>
#include <iomanip>
#include <set>

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
#include "AdmixtureShiftTreeRates.h"
#include "AdmixtureShiftNodeAgeAndRate.h"
#include "AdmixtureSubtreePruneRegraftAndRateShift.h"
#include "AdmixtureNarrowExchange.h"
#include "AdmixtureEdgeDivergenceMerge.h"
#include "AdmixtureSubtreePruneRegraft.h"
#include "AdmixtureTree.h"
#include "AdmixtureTreeLengthStatistic.h"
#include "AdmixtureRateAgeBetaShift.h"
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
#include "PoissonDistribution.h"
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
    std::vector<unsigned int> seed;
    //seed.push_back(2); seed.push_back(2); GLOBAL_RNG->setSeed(seed);
    
    // read in data
   // SnpData* snps = PopulationDataReader().readSnpData(snpFilename);
    SnpData* snps = PopulationDataReader().readSnpData2(snpFilename,1);
    size_t numTaxa = snps->getNumPopulations();
    size_t numNodes = 2 * numTaxa - 1;
    size_t numBranches = numNodes - 1;
    size_t numSites = snps->getNumSnps();
    int blockSize = 50;
    
    int delay = 300; //2000;
    int numTreeResults = 500;
    int numAdmixtureResults = 500;
    int maxNumberOfAdmixtureEvents = 4;
    
    bool useWishart = true;             // if false, the composite likelihood function is used
    bool useBias = true;               // if false, no covariance bias correction for small sample size is used
    bool useAdmixtureEdges = true;      // if false, no admixture moves or edges are used
    bool useBranchRates = true;         // if false, all populations are of the same size
    bool allowSisterAdmixture = true;   // if false, admixture events cannot be between internal lineages who share a divergence parent
    bool discardNonPosDefMtx = true;    // if false, round negative eigenvalues to positive eps
    bool useContrasts = false;          // nothing really, need to remove
    bool updateParameters = true;
    bool updateTree = true;
    
    bool useParallelMcmcmc = true;
    int numChains = 24;
    int numProcesses = numChains;
    if (numChains >= 24) numProcesses /= 2;
    if (numChains == 1) numProcesses = 1;
    int swapInterval = 10;
    double deltaTemp = .1;
    
    
    std::stringstream rndStr;
    rndStr << std::setw(9) << std::fixed << std::setprecision(0) << std::setfill('0') << std::floor(GLOBAL_RNG->uniform01()*1e9);
    std::string outName = "w_clovis." + rndStr.str();
    
    // BM diffusion rate
    ConstantNode<double>* a_bm = new ConstantNode<double>( "bm_a", new double(3));
    ConstantNode<double>* b_bm = new ConstantNode<double>( "bm_b", new double(10));
    StochasticNode<double>* diffusionRate = new StochasticNode<double> ("rate_BM", new ExponentialDistribution(b_bm));
    
    // admixture CPP rate

    
    //int branchPairs = (int)(numBranches * (numBranches-1)) / 2;
    //double cpp_data_prior_scaler = 1;
    //double cpp_data_prior =(double)(numSites * branchPairs) / cpp_data_prior_scaler;
    
    // This prior requires admixture events to improve lnL by N units
    double adm_th_lnL = 50;
    double rate_cpp_prior = exp(adm_th_lnL);
    
    // MJL 071713:  Flat Poisson prior cannot overpower model overfitting when lnL is large.
    //              Consider implementing Conway-Maxewell-Poisson distn instead.
    
    ConstantNode<double>* c = new ConstantNode<double>( "c", new double(rate_cpp_prior)); // admixture rate prior
    StochasticNode<double>* admixtureRate = new StochasticNode<double> ("rate_CPP", new ExponentialDistribution(c));
    StochasticNode<int>* admixtureCount = new StochasticNode<int> ("count_CPP", new PoissonDistribution(admixtureRate));
    admixtureCount->setValue(new int(0));
    if (!useAdmixtureEdges)
    {
        admixtureRate->clamp(new double(1.0/rate_cpp_prior));
        admixtureCount->clamp(new int(0));
    }
        
    std::cout << admixtureRate->getValue() << "\n";
    //admixtureRate->setValue(new double(1.0));// / cpp_prior));
    
    // birth-death process for ultrametric tree
    StochasticNode<double>* diversificationRate = new StochasticNode<double>("div", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(10.0)) ));
    StochasticNode<double>* turnover = new StochasticNode<double>("turnover", new UniformDistribution(new ConstantNode<double>("do_lower", new double(0.0)), new ConstantNode<double>("do_upper", new double(1.0)) ));
    //diversificationRate->setValue(new double(1.0));
    
    // tree node
    StochasticNode<AdmixtureTree>* tau = new StochasticNode<AdmixtureTree>( "tau", new AdmixtureConstantBirthDeathProcess(diversificationRate, turnover, (int)numTaxa, snps->getPopulationNames(), snps->getOutgroup()) );
    
    // tree length (for verifying CPP)
    DeterministicNode<double>* treeLength = new DeterministicNode<double>("TreeLength", new AdmixtureTreeLengthStatistic(tau) );
    DeterministicNode<size_t>* treeAdmixtureEventCount = new DeterministicNode<size_t>("TreeAdmixtureEventCount", new TreeAdmixtureEventCount(tau) );
    
    // branch multipliers (mutation rate is clocklike, but population sizes are not)
	std::vector<const TypedDagNode<double> *> branchRates;
    std::vector< ContinuousStochasticNode *> branchRates_nonConst;
    ConstantNode<double>* branchRateA = new ConstantNode<double>( "branchRateA", new double(2));
    ConstantNode<double>* branchRateB = new ConstantNode<double>( "branchRateB", new double(2));
	for( int i=0; i<numBranches; i++){

        std::ostringstream br_name;
        br_name << "br_" << i;
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new ExponentialDistribution(branchRateA) );
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new InverseGammaDistribution(branchRateA, branchRateB));
        ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new GammaDistribution(branchRateA, branchRateB));
        //ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode(br_name.str(), new LognormalDistribution(branchRateA, branchRateB));
        
		if (!useBranchRates)
        {
            tmp_branch_rate->clamp(new double(1.0));
        }
        
        branchRates.push_back( tmp_branch_rate );
        branchRates_nonConst.push_back( tmp_branch_rate );
        
        
    }
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchRates ) );

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
    //ConstantNode<int>* delayTimer = new ConstantNode<int>( "timer", new int(delay));
    
    // MOVES
    std::cout << "Adding moves\n";
    
    // moves vector
    std::vector<Move*> moves;
   
    

    // model parameters
    if (updateParameters)
    {
        moves.push_back( new ScaleMove(diffusionRate, 1.0, true, 4.0) );
        moves.push_back( new ScaleMove(diversificationRate, 1.0, true, 2.0) );
        moves.push_back( new ScaleMove(turnover, 1.0, true, 2.0) );
    }
    
    
    // non-admixture tree updates
    if (updateTree)
    {
        moves.push_back( new AdmixtureNarrowExchange( tau, 1.0, numTaxa/2) );
        moves.push_back( new AdmixtureSubtreePruneRegraft( tau, 1.0, numTaxa/2) );
        moves.push_back( new AdmixtureFixedNodeheightPruneRegraft(tau, numTaxa/2));
        for (size_t i = numTaxa; i < numNodes - 1; i++)
            moves.push_back( new AdmixtureNodeTimeSlideBeta( tau, (int)i, 1.0, false, 1.0 ) );
    }
    
    
    // branch rate updates
    if (useBranchRates)
    {
        // branch rate multipliers
        for( int i=0; i < numBranches; i++)
            moves.push_back( new ScaleMove(branchRates_nonConst[i], 1.0, false, 1.0) );
        
        // shift node age for branch rate
        for (size_t i = numTaxa; i < numNodes - 1; i++)
        {
            //std::cout << "age  " << tau->getValue().getNode(i).getAge() << "\n";
            moves.push_back( new AdmixtureShiftNodeAgeAndRate(tau, branchRates_nonConst, (int)i, 1.0, false, 1.0) );
            // MJL 071513: fixed, I think
        
            
            std::vector<DagNode*> pvec;
            pvec.push_back(tau);
            pvec.push_back(branchRates_nonConst[i]);
            //moves.push_back( new AdmixtureSubtreePruneRegraftAndRateShift(pvec, i, 2.0, 1.0) );
            // ... something wrong with how the lnProb is computed using the lnProb ratios...
            
        }
        
        // NNI with branch rate modifier (not working quite right, disabled)
        moves.push_back( new AdmixtureNearestNeighborInterchangeAndRateShift( tau, branchRates_nonConst, 1.0, false, numTaxa));
        
        // doesn't seem very useful...
        moves.push_back( new AdmixtureShiftTreeRates(diffusionRate, branchRates_nonConst, 1.0, false, 2));
    }
        
    // admixture tree updates
    if (useAdmixtureEdges)
    {
        
        moves.push_back( new AdmixtureEdgeAddResidualWeights( tau, admixtureRate, admixtureCount, residuals, delay, maxNumberOfAdmixtureEvents, allowSisterAdmixture, 2.0) );
        moves.push_back( new AdmixtureEdgeRemoveResidualWeights( tau, admixtureRate, admixtureCount, residuals, delay, 2.0) );
        moves.push_back( new AdmixtureEdgeReplaceResidualWeights( tau, admixtureRate, branchRates_nonConst, residuals, delay, allowSisterAdmixture, 10.0) );
        moves.push_back( new AdmixtureEdgeDivergenceMerge( tau, admixtureRate, branchRates_nonConst, admixtureCount, residuals, delay, allowSisterAdmixture, 5.0 ));

     
        moves.push_back( new AdmixtureEdgeReweight( tau, delay, 1.0, 5.0) );
        moves.push_back( new AdmixtureEdgeReversePolarity( tau, delay, 1.0, 5.0) );
        moves.push_back( new AdmixtureEdgeSlide( tau, branchRates_nonConst, delay, allowSisterAdmixture, 1.0, 20.0) );
        moves.push_back( new ScaleMove(admixtureRate, 1.0, false, 5.0));
      
        
        // moves.push_back( new AdmixtureCPPRateScaleMove( tau, admixtureRate, cpp_prior, 1.0, false, 2.0));
        
    }
    
    // dream moves...
    // scale branch rates & add/delete admixture edge
    // root rotate w/ subtree scale
    
    // MONITORS
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;
    
    // parameter monitor
    std::vector<DagNode*> monitoredNodes;
    monitoredNodes.push_back( diffusionRate );
    monitoredNodes.push_back( admixtureRate );
    monitoredNodes.push_back( diversificationRate );
    monitoredNodes.push_back( turnover );
    monitoredNodes.push_back( admixtureCount );
    //monitoredNodes.insert( treeLength );
    //monitoredNodes.insert( treeAdmixtureEventCount ) ;
    for( int i=0; i<numBranches; i++){
        monitoredNodes.push_back( branchRates_nonConst[i] );
	}
     
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "/Users/mlandis/data/admix/output/" + outName + ".parameters.txt", "\t", true, true, true, true, true ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 1, "\t" ) );
 
    monitors.push_back( new ExtendedNewickAdmixtureTreeMonitor( tau, br_vector, true, 10, "/Users/mlandis/data/admix/output/" + outName + ".admixture_trees.txt", "\t", true, true, true, true ) );
    
    monitors.push_back( new AdmixtureBipartitionMonitor(tau, br_vector, numTreeResults, numAdmixtureResults, 10, "/Users/mlandis/data/admix/output/" + outName + ".bipartitions.txt", "\t", true, true, true, true ) );
    
    monitors.push_back( new AdmixtureResidualsMonitor(residuals, snps->getPopulationNames(), 10, "/Users/mlandis/data/admix/output/" + outName + ".residuals.txt", "\t", true, true, true, true ) );

    monitors.push_back( new ExtendedNewickAdmixtureTreeMonitor( tau, br_vector, false, 1, "/Users/mlandis/data/admix/output/" + outName + ".topology_trees.trees", "\t", true, true, true, true ) );
    
    std::set<DagNode*> mn2;
    mn2.insert(tau);
    monitors.push_back( new FileMonitor( mn2, 10,"/Users/mlandis/data/admix/output/" + outName + ".time_trees.trees", "\t", true, true, true, true ));
    
    
    // MODEL
    std::cout << "Calling model\n";
    std::set<const DagNode*> mset;
    mset.insert(admixtureRate);
 //   mset.insert(delayTimer);
    Model myModel = Model(mset);
    
    // MCMC
    std::cout << "Calling mcmc\n";
    
    if (!useParallelMcmcmc)
    {
        double scaleFactorConstant = 2000.0; // make this smaller to flatten the likelihood surface
        double chainHeat = scaleFactorConstant / numSites;
        chainHeat = 1.0;
        Mcmc myMcmc = Mcmc(myModel, moves, monitors, true, chainHeat);
        myMcmc.run(100000);
        myMcmc.printOperatorSummary();
    }
    else
    {
        ParallelMcmcmc myPmc3(myModel, moves, monitors, numChains, numProcesses, swapInterval, deltaTemp);
        myPmc3.run(1000000);
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