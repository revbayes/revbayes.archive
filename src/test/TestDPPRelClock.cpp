#include "BetaDistribution.h"
#include "BinaryDivision.h"
#include "BinaryMultiplication.h"
#include "BinarySubtraction.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "DirichletProcessPriorDistribution.h"
#include "DPPAllocateAuxGibbsMove.h"
#include "DPPGibbsConcentrationMove.h"
#include "MeanVecContinuousValStatistic.h"
#include "NumUniqueInVector.h"
#include "DPPScaleCatValsMove.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "GtrRateMatrixFunction.h"
#include "LnFunction.h"
#include "LognormalDistribution.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "NormalDistribution.h"
#include "OriginTimeSlide.h"
#include "PoissonDistribution.h"
#include "QuantileFunction.h"
#include "RbFileManager.h"
#include "RbStatisticsHelper.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlideProposal.h"
#include "SubtreeScale.h"
#include "TestDPPRelClock.h"
#include "TimeTree.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorDoubleProductStatistic.h"
#include "VectorFunction.h"
//#include "RbVector.h"

using namespace RevBayesCore;

TestDPPRelClock::TestDPPRelClock(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestDPPRelClock::~TestDPPRelClock() {
    // nothing to do
}

bool TestDPPRelClock::run( void ) {
    
    // fix the rng seed
    GLOBAL_RNG->setSeed(42);
    
    alignmentFilename = "/Users/tracyh/Code/RevBayes_proj/tests/time_trees/tt_CLK_GTRG.nex";
    treeFilename = "/Users/tracyh/Code/RevBayes_proj/tests/time_trees/tt_CLK_true_relx.tre";
	
	std::vector<AbstractCharacterData*> data = NclReader().readMatrices(alignmentFilename);
	
	AbstractDiscreteCharacterData *discrD = dynamic_cast<AbstractDiscreteCharacterData* >(data[0]);
    
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
	
	// First, we read in the data 
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
	// #######################################
    // ###### birth-death process priors #####
	// #######################################

	//   Constant nodes
	ConstantNode<double> *dLambda = new ConstantNode<double>("div_rate", new double(1.0 / 5.0));		// Exponential rate for prior on div
//	ConstantNode<double> *turnA   = new ConstantNode<double>("turn_alpha", new double(2.0));			// Beta distribution alpha
//	ConstantNode<double> *turnB   = new ConstantNode<double>("turn_beta", new double(2.0));				// Beta distribution beta
	ConstantNode<double> *turnA   = new ConstantNode<double>("turn.uni_min", new double(0.0));			// Uniform distribution min
	ConstantNode<double> *turnB   = new ConstantNode<double>("turn.uni_max", new double(1.0));				// Uniform distribution max
    ConstantNode<double> *rho     = new ConstantNode<double>("rho", new double(1.0));					// assume 100% sampling for now
//	ConstantNode<double> *meanOT  = new ConstantNode<double>("meanOT", new double(trees[0]->getRoot().getAge()*1.5));
//	ConstantNode<double> *stdOT   = new ConstantNode<double>("stdOT", new double(10.0));
    ConstantNode<double> *origin  = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );

	//   Stochastic nodes
//    StochasticNode<double> *origin  = new StochasticNode<double>( "origin", new NormalDistribution(meanOT, stdOT) );
    StochasticNode<double> *div   = new StochasticNode<double>("diversification", new ExponentialDistribution(dLambda));
    StochasticNode<double> *turn  = new StochasticNode<double>("turnover", new UniformDistribution(turnA, turnB));
//    StochasticNode<double> *turn  = new StochasticNode<double>("turnover", new BetaDistribution(turnA, turnB));

	//   Deterministic nodes
	//    birthRate = div / (1 - turn)
	DeterministicNode<double> *birthRate = new DeterministicNode<double>("birth_rate", new BinaryDivision<double,double,double>(div, new DeterministicNode<double>("", new BinarySubtraction<double,double,double>(new ConstantNode<double>("", new double(1.0)), turn))));
	//    deathRate = (div * turn) / ( 1 - turn)
	DeterministicNode<double> *deathRate = new DeterministicNode<double>("death_rate", new BinaryDivision<double,double,double>( new DeterministicNode<double>("" , new BinaryMultiplication<double,double,double>(div, turn)), new DeterministicNode<double>("", new BinarySubtraction<double,double,double>(new ConstantNode<double>("", new double(1.0)), turn))));
	// For some datasets with large root ages, if div>1.0 (or so), the probability is NaN
//	RandomNumberGenerator* rng = GLOBAL_RNG;
//	div->setValue(rng->uniform01() / 1.5);

	// Birth-death tree
    std::vector<std::string> names = data[0]->getTaxonNames();
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, birthRate, deathRate, rho, "uniform", "nTaxa", taxa, std::vector<Clade>()) );
	//DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
	tau->setValue( trees[0] );

	
	// ####################################
	// #### DPP Model on Branch Rates #####
	// ####################################

	size_t numBranches = 2 * data[0]->getNumberOfTaxa() - 2;

	// The prior mean number of rate categories induces an expected concentration parameter of ~meanCP
	double priorMean = 5.0;
	double meanCP = RbStatistics::Helper::dppConcParamFromNumTables(priorMean, (double)numBranches);
	
	// Setting up the hyper prior on the concentratino parameter
	// This hyperprior is fully conditional on the DPP using a gamma distribution
	//    the parameters of the gamma distribution are set so that the expectation of the hyperprior = meanCP
	//    where meanCP = dpA / dpB
	ConstantNode<double> *dpA  = new ConstantNode<double>("dp_a", new double(2.0) );
	ConstantNode<double> *dpB  = new ConstantNode<double>("dp_b", new double(dpA->getValue() / meanCP) );
	StochasticNode<double> *cp = new StochasticNode<double>("DPP.cp", new GammaDistribution(dpA, dpB) ); 
//	ConstantNode<double> *cp = new ConstantNode<double>("DPP.cp", new double(meanCP) );
	
	// G_0 is an Gamma distribution
    ConstantNode<double> *a      = new ConstantNode<double>("a", new double(2.0) );
    ConstantNode<double> *b      = new ConstantNode<double>("b", new double(4.0) );
	TypedDistribution<double> *g = new GammaDistribution(a, b);
	
	// branchRates ~ DPP(g, cp, numBranches)
	StochasticNode<RbVector<double> > *branchRates = new StochasticNode<RbVector<double> >("branchRates", new DirichletProcessPriorDistribution<double>(g, cp, (int)numBranches) );

	// a deterministic node for calculating the number of rate categories (required for the Gibbs move on cp)
	//DeterministicNode<int> *numCats = new DeterministicNode<int>("DPPNumCats", new NumUniqueInVector<double>(branchRates) );
	
//	ConstantNode<double> *crA  = new ConstantNode<double>("CR.gammA", new double(0.1) );
//	ConstantNode<double> *crL  = new ConstantNode<double>("CR.gammL", new double(100.0) );
//	StochasticNode<double> *clockRate  = new StochasticNode<double>("clockRate", new GammaDistribution(crA, crL) );

//	ConstantNode<double> *crInv  = new ConstantNode<double>("invCr", new double(1.0) );
//	DeterministicNode<double> *scaleRate = new DeterministicNode<double>("scaleRate", new BinaryDivision<double, double, double>(crInv, treeHeight));
//
//	DeterministicNode<std::vector<double> > *branchSubRates = new DeterministicNode< std::vector<double> >("branchSRs", new VectorDoubleProductStatistic(branchRates, scaleRate));
    
//    ConstantNode<double> *tPoisL = new ConstantNode<double>("tp", new double(1.0));
//    TypedDistribution<int> *pois = new PoissonDistribution(tPoisL);
//    StochasticNode<std::vector<int> > *poiVals = new StochasticNode<std::vector<int> >("pois", new DirichletProcessPriorDistribution<int>(pois, cp, (int)numBranches) );


	// ####################################


    // ###### GTR model priors ######
	//    Constant nodes
    ConstantNode<RbVector<double> > *bf   = new ConstantNode<RbVector<double> >( "bf", new RbVector<double>(4,1.0) );
    ConstantNode<RbVector<double> > *e    = new ConstantNode<RbVector<double> >( "e", new RbVector<double>(6,1.0) );
    //    Stochastic nodes
	DirichletDistribution *bfDD = new DirichletDistribution(bf);
    StochasticNode<RbVector<double> > *pi = new StochasticNode<RbVector<double> >( "pi", bfDD );
    StochasticNode<RbVector<double> > *er = new StochasticNode<RbVector<double> >( "er", new DirichletDistribution(e) );

    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    std::cout << "Q:\t" << q->getValue() << std::endl;
	
//	// ####### Gamma Rate Het. ######
//	
//	ConstantNode<double> *shapePr = new ConstantNode<double>("gammaShPr", new double(0.5));
//	ContinuousStochasticNode *srAlpha = new ContinuousStochasticNode("siteRates.alpha", new ExponentialDistribution(shapePr));
//    ConstantNode<double> *q1 = new ConstantNode<double>("q1", new double(0.125) );
//    DeterministicNode<double> *q1Value = new DeterministicNode<double>("q1_value", new QuantileFunction(q1, new GammaDistribution(srAlpha, srAlpha) ) );
//    ConstantNode<double> *q2 = new ConstantNode<double>("q2", new double(0.375) );
//    DeterministicNode<double> *q2Value = new DeterministicNode<double>("q2_value", new QuantileFunction(q2, new GammaDistribution(srAlpha, srAlpha) ) );
//    ConstantNode<double> *q3 = new ConstantNode<double>("q3", new double(0.625) );
//    DeterministicNode<double> *q3Value = new DeterministicNode<double>("q3_value", new QuantileFunction(q3, new GammaDistribution(srAlpha, srAlpha) ) );
//    ConstantNode<double> *q4 = new ConstantNode<double>("q4", new double(0.875) );
//    DeterministicNode<double> *q4Value = new DeterministicNode<double>("q4_value", new QuantileFunction(q4, new GammaDistribution(srAlpha, srAlpha) ) );
//    std::vector<const TypedDagNode<double>* > gammaRates = std::vector<const TypedDagNode<double>* >();
//    gammaRates.push_back(q1Value);
//    gammaRates.push_back(q2Value);
//    gammaRates.push_back(q3Value);
//    gammaRates.push_back(q4Value);
//    DeterministicNode<RbVector<double> > *siteRates = new DeterministicNode<RbVector<double> >( "site_rates", new VectorFunction<double>(gammaRates) );
//    DeterministicNode<RbVector<double> > *siteRatesNormed = new DeterministicNode<RbVector<double> >( "site_rates_norm", new NormalizeVectorFunction(siteRates) );
//	
    
	
    std::cout << "tau:\t" << tau->getValue() << std::endl;
	std::cout << " ** origin   " << origin->getValue() << std::endl;
	std::cout << " ** root age " << trees[0]->getRoot().getAge() << std::endl;

    PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree> *phyloCTMC = new PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree>(tau, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setClockRate( branchRates );
//	phyloCTMC->setClockRate( branchSubRates );
    phyloCTMC->setRateMatrix( q );
//	phyloCTMC->setSiteRates( siteRatesNormed );
    StochasticNode< AbstractDiscreteCharacterData > *charactermodel = new StochasticNode< AbstractDiscreteCharacterData >("S", phyloCTMC );
	charactermodel->clamp( discrD );
	
	std::cout << " branch rates: " << branchRates->getValue() << std::endl;
	std::cout << " diversification: " << div->getValue() << std::endl;
	std::cout << " turnover: " << turn->getValue() << std::endl;
	std::cout << " birth rate: " << birthRate->getValue() << std::endl;
	std::cout << " death rate: " << deathRate->getValue() << std::endl;

	/* add the moves */
//    std::vector<Move* > moves;
//    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 1, true ) );
//    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(turn, 1.0), 2, true ) );
////    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
////    moves.push_back( new NarrowExchange( tau, 10.0 ) );
////    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
////    moves.push_back( new SubtreeScale( tau, 5.0 ) );
////    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
////	moves.push_back( new OriginTimeSlide( origin, tau, 20.0, true, 5.0 ) );
////	moves.push_back( new RootTimeSlide( tau, 10.0, true, 10.0 ) );
////    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(srAlpha, log(2.0)), 1, true ) );
//    moves.push_back( new NodeTimeSlideUniform( tau, 3.0 * ((double)numBranches) ) );
//    moves.push_back( new SimplexMove( er, 200.0, 6, 0, true, 2.0, 2.0 ) );
//    moves.push_back( new SimplexMove( pi, 150.0, 4, 0, true, 2.0, 2.0 ) ); 
//    moves.push_back( new SimplexMove( er, 200.0, 1, 0, false, 2.0 ) );
//    moves.push_back( new SimplexMove( pi, 100.0, 1, 0, false, 2.0 ) );
//    moves.push_back( new DPPScaleCatValsMove( branchRates, log(2.0), 2.0 ) );
//    moves.push_back( new DPPAllocateAuxGibbsMove<double>( branchRates, 4, 2.0 ) );
//    moves.push_back( new DPPGibbsConcentrationMove( cp, numCats, dpA, dpB, (int)numBranches, 2.0 ) );
////    moves.push_back( new SlidingMove( srAlpha, 0.25, false, 1.0 ) );
//	
//    // add some tree stats to monitor
//	DeterministicNode<double> *meanBrRate = new DeterministicNode<double>("MeanBranchRate", new MeanVecContinuousValStatistic(branchRates) );
//
//    /* add the monitors */
//    std::vector<Monitor* > monitors;
//    std::vector<DagNode*> monitoredNodes;
//	monitoredNodes.push_back( treeHeight );
//	monitoredNodes.push_back( origin );
//    monitoredNodes.push_back( numCats );
//    monitoredNodes.push_back( meanBrRate );
//    monitoredNodes.push_back( cp );
//    monitoredNodes.push_back( div );
////    monitoredNodes.push_back( srAlpha );
//	monitoredNodes.push_back( pi );
//	ScreenMonitor *sm = new ScreenMonitor( monitoredNodes, 10);
//    monitors.push_back( sm );
// 
//	monitoredNodes.push_back( div );
//	monitoredNodes.push_back( turn );
//	monitoredNodes.push_back( birthRate );
//	monitoredNodes.push_back( deathRate );
//    monitoredNodes.push_back( er );
////    monitoredNodes.push_back( srAlpha );
// 	monitoredNodes.push_back( branchRates );
////    monitoredNodes.push_back( siteRates );
////    monitoredNodes.push_back( siteRatesNormed );
//// 	monitoredNodes.push_back( scaleRate );
//// 	monitoredNodes.push_back( branchSubRates );
//
//	std::string logFN = "dpp_test/rb_tt_CLK_rn_1.log";
//	monitors.push_back( new FileMonitor( monitoredNodes, 10, logFN, "\t" ) );
//
////    std::set<DagNode*> monitoredNodes2;
////    monitoredNodes2.insert( tau );
////
////	std::string treFN = "/Users/tracyh/Code/RevBayes_proj/tests/time_trees/rb_tt_CLK_pr.trees";
////    monitors.push_back( new FileMonitor( monitoredNodes2, 10, treFN, "\t", false, false, false ) );
//    
//    /* instantiate the model */
//    Model myModel = Model(q);
//    
//	mcmcGenerations = 3000;
//
//    /* instiate and run the MCMC */
//    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
//    myMcmc.run(mcmcGenerations);
//    
//    myMcmc.printOperatorSummary();
//	
//   
//	/* clean up */
////	delete div;
////	delete turn;
////	delete rho;
////	delete cp;
////	delete branchRates;
////	delete q;
////	delete tau;
////	delete charactermodel;
////	delete treeHeight;
//	delete meanBrRate;
//	delete numCats;
//	delete g;
////	delete a;
////	delete birthRate;
////	delete phyloCTMC;
////	delete dLambda;
//
//	
//	monitors.clear();
//	moves.clear();
	
    return true;
}
