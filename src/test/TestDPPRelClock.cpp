#include "BetaDistribution.h"
#include "BinaryDivision.h"
#include "BinaryMultiplication.h"
#include "BinarySubtraction.h"
#include "BirthRateConstBDStatistic.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeathRateConstBDStatistic.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "DirichletProcessPriorDistribution.h"
#include "DPPAllocateAuxGibbsMove.h"
#include "DPPGibbsConcentrationMove.h"
#include "DppMeanContinuousValStatistic.h"
#include "DppNumTablesStatistic.h"
#include "DPPScaleCatValsMove.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "LnFunction.h"
#include "LognormalDistribution.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "RbFileManager.h"
#include "RbStatisticsHelper.h"
#include "RootTimeSlide.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SubtreeScale.h"
#include "TestDPPRelClock.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestDPPRelClock::TestDPPRelClock(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestDPPRelClock::~TestDPPRelClock() {
    // nothing to do
}

bool TestDPPRelClock::run( void ) {
	
	std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
	
	// First, we read in the data 
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
	// #######################################
    // ###### birth-death process priors #####
	// #######################################

	//   Constant nodes
	ConstantNode<double> *dLambda = new ConstantNode<double>("div_rate", new double(1.0 / 5.0));		// Exponential rate for prior on div
	ConstantNode<double> *turnA   = new ConstantNode<double>("turn_alpha", new double(2.0));			// Beta distribution alpha
	ConstantNode<double> *turnB   = new ConstantNode<double>("turn_beta", new double(2.0));				// Beta distribution beta
    ConstantNode<double> *rho     = new ConstantNode<double>("rho", new double(1.0));					// assume 100% sampling for now
    ConstantNode<double> *origin  = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*1.5 ) );

	//   Stochastic nodes
    StochasticNode<double> *div   = new StochasticNode<double>("diversification", new ExponentialDistribution(dLambda));
    StochasticNode<double> *turn  = new StochasticNode<double>("turnover", new BetaDistribution(turnA, turnB));

	//   Deterministic nodes
	//    birthRate = div / (1 - turn)
	DeterministicNode<double> *birthRate = new DeterministicNode<double>("birth_rate", new BirthRateConstBDStatistic(div, turn));
	//    deathRate = (div * turn) / ( 1 - turn)
	DeterministicNode<double> *deathRate = new DeterministicNode<double>("death_rate", new DeathRateConstBDStatistic(div, turn));
	// For some datasets with large root ages, if div>1.0 (or so), the probability is NaN
	RandomNumberGenerator* rng = GLOBAL_RNG;
	div->setValue(rng->uniform01() / 1.5);

	// Birth-death tree
    std::vector<std::string> names = data[0]->getTaxonNames();
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, birthRate, deathRate, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
	
	
	// ####################################
	// #### DPP Model on Branch Rates #####
	// ####################################

	size_t numBranches = 2 * data[0]->getNumberOfTaxa() - 2;

	// The prior mean number of rate categories induces an expected concentration parameter of ~meanCP
	double priorMean = 3.0;
	double meanCP = RbStatistics::Helper::dppConcParamFromNumTables(priorMean, (double)numBranches);
	
	// Setting up the hyper prior on the concentratino parameter
	// This hyperprior is fully conditional on the DPP using a gamma distribution
	//    the parameters of the gamma distribution are set so that the expectation of the hyperprior = meanCP
	//    where meanCP = dpA / dpB
	ConstantNode<double> *dpA  = new ConstantNode<double>("dp_a", new double(2.0) );
	ConstantNode<double> *dpB  = new ConstantNode<double>("dp_b", new double(dpA->getValue() / meanCP) );
	StochasticNode<double> *cp = new StochasticNode<double>("DPP.cp", new GammaDistribution(dpA, dpB) );
	
	// G_0 is an exponential distribution
    ConstantNode<double> *a      = new ConstantNode<double>("a", new double(1.0) );
	TypedDistribution<double> *g = new ExponentialDistribution(a);
	
	// branchRates ~ DPP(g, cp, numBranches)
	StochasticNode<std::vector<double> > *branchRates = new StochasticNode<std::vector<double> >("branchRates", new DirichletProcessPriorDistribution<double>(g, cp, (int)numBranches) );

	// a deterministic node for calculating the number of rate categories (required for the Gibbs move on cp)
	DeterministicNode<int> *numCats = new DeterministicNode<int>("DPPNumCats", new DppNumTablesStatistic<double>(branchRates) );

	// ####################################


    // ###### GTR model priors ######
	//    Constant nodes
    ConstantNode<std::vector<double> > *bf   = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e    = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );
    //    Stochastic nodes
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );

    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
	
	tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
	std::cout << " ** origin   " << origin->getValue() << std::endl;
	std::cout << " ** root age " << trees[0]->getRoot().getAge() << std::endl;

    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
	phyloCTMC->setClockRate( branchRates );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
	charactermodel->clamp( data[0] );
	
	std::cout << " branch rates: " << branchRates->getValue() << std::endl;
	std::cout << " diversification: " << div->getValue() << std::endl;
	std::cout << " turnover: " << turn->getValue() << std::endl;
	std::cout << " birth rate: " << birthRate->getValue() << std::endl;
	std::cout << " death rate: " << deathRate->getValue() << std::endl;

	/* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(div, 1.0, true, 2.0) );
    moves.push_back( new ScaleMove(turn, 1.0, true, 2.0) );
//    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
//    moves.push_back( new NarrowExchange( tau, 10.0 ) );
//    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
//    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
	moves.push_back( new RootTimeSlide( tau, 50.0, true, 10.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new SimplexMove( er, 450.0, 6, 0, true, 2.0, 1.0 ) );
    moves.push_back( new SimplexMove( pi, 250.0, 4, 0, true, 2.0, 1.0 ) ); 
    moves.push_back( new SimplexMove( er, 200.0, 1, 0, false, 1.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 1, 0, false, 1.0 ) );
    moves.push_back( new DPPScaleCatValsMove( branchRates, log(2.0), 2.0 ) );
    moves.push_back( new DPPAllocateAuxGibbsMove<double>( branchRates, 4, 2.0 ) );
    moves.push_back( new DPPGibbsConcentrationMove<double>( cp, numCats, dpA, dpB, (int)numBranches, 2.0 ) );
		
	
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
	DeterministicNode<double> *meanBrRate = new DeterministicNode<double>("MeanBranchRate", new DppMeanContinuousValStatistic(branchRates) );
	
//	Clade nd14 = new Clade(

    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::vector<DagNode*> monitoredNodes;
	monitoredNodes.push_back( treeHeight );
    monitoredNodes.push_back( numCats );
    monitoredNodes.push_back( meanBrRate );
    monitoredNodes.push_back( cp );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 10, "\t" ) );
 
	monitoredNodes.push_back( div );
	monitoredNodes.push_back( turn );
	monitoredNodes.push_back( birthRate );
	monitoredNodes.push_back( deathRate );
	monitoredNodes.push_back( pi );
    monitoredNodes.push_back( er );
 	monitoredNodes.push_back( branchRates );
	monitors.push_back( new FileMonitor( monitoredNodes, 10, "clock_test/test_RBdpp_clock_x.log", "\t" ) );

    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "clock_test/test_RBdpp_clock_x.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(q);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    myMcmc.printOperatorSummary();
	
    
	/* clean up */
//	delete div;
//	delete turn;
//	delete rho;
//	delete cp;
//	delete a;
//	delete g;
//	delete branchRates;
//	delete q;
//	delete tau;
//	delete phyloCTMC;
//	delete charactermodel;
//	delete treeHeight;

    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
	}
	
	
	monitors.clear();
	moves.clear();
	
    return true;
}
