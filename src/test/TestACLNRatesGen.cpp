#include "AutocorrelatedLognormalRateDistribution.h"
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
#include "MeanVecContinuousValStatistic.h"
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
#include "QuantileFunction.h"
#include "RateOnBranchAve.h"
#include "RbFileManager.h"
#include "RbStatisticsHelper.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SingleElementScaleMove.h"
#include "SubtreeScale.h"
#include "TestACLNRatesGen.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "VectorSingleElementScaleMove.h"
#include "ScaleSingleACLNRatesMove.h"
#include "RateAgeACLNMixingMove.h"
#include "OriginTimeSlide.h"

using namespace RevBayesCore;

TestACLNRatesGen::TestACLNRatesGen(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestACLNRatesGen::~TestACLNRatesGen() {
    // nothing to do
}

bool TestACLNRatesGen::run( void ) {

//    alignmentFilename = "/Users/tracyh/Code/RevBayes_proj/tests/time_trees/tt_CLK_GTRG.nex";
//    treeFilename = "/Users/tracyh/Code/RevBayes_proj/tests/time_trees/tt_CLK_true_relx.tre";
	
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
	ConstantNode<double> *meanOT  = new ConstantNode<double>("meanOT", new double(trees[0]->getRoot().getAge()*1.5));
	ConstantNode<double> *stdOT   = new ConstantNode<double>("stdOT", new double(10.0));
	
	//   Stochastic nodes
    StochasticNode<double> *origin  = new StochasticNode<double>( "origin", new NormalDistribution(meanOT, stdOT) );
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
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, birthRate, deathRate, rho, "uniform", "nTaxa", int(names.size()), names, std::vector<Clade>()) );

    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
	
	
	// ##############################################
	// #### ACLN Model on Branch Rates #####
	// ##############################################
	
	size_t numBranches = 2 * data[0]->getNumberOfTaxa() - 2;
	size_t numNodes = numBranches + 1; // model rates at nodes
	
    ConstantNode<double> *a      = new ConstantNode<double>("a", new double(4.0) );
    ConstantNode<double> *b      = new ConstantNode<double>("b", new double(4.0) );
    ConstantNode<double> *anu    = new ConstantNode<double>("a_nu", new double(1.0) );
    ConstantNode<double> *bnu    = new ConstantNode<double>("b_nu", new double(8.0) );
	
	StochasticNode<double> *rootRate = new StochasticNode<double>("root.rate", new GammaDistribution(a, b));
	StochasticNode<double> *bmNu = new StochasticNode<double>("BM_var", new GammaDistribution(anu, bnu));
	
	size_t rootID = trees[0]->getRoot().getIndex();

	ConstantNode<double> *crInv  = new ConstantNode<double>("invCr", new double(1.0) );
	DeterministicNode<double> *scaleRate = new DeterministicNode<double>("scaleRate", new BinaryDivision<double, double, double>(crInv, treeHeight));

	StochasticNode< std::vector< double > > *nodeRates = new StochasticNode< std::vector< double > >( "NodeRates", new AutocorrelatedLognormalRateDistribution(tau, bmNu, rootRate, scaleRate) );
	
	std::cout << nodeRates->getValue().size() << std::endl;
	

	std::vector<const TypedDagNode<double> *> branchRates;
	for( size_t i=0; i<numBranches; i++){
		std::ostringstream brName;
        brName << "br(" << i << ")";
		DeterministicNode<double> *tmpBrRt = new DeterministicNode<double>(brName.str(), new RateOnBranchAve(nodeRates, tau, scaleRate, i));
		branchRates.push_back( tmpBrRt );
	}
    DeterministicNode< std::vector< double > >* brVector = new DeterministicNode< std::vector< double > >( "branchRates", new VectorFunction< double >( branchRates ) );
	
	// making a combined DagNode for a compound move
	std::vector<DagNode*> treeAndRates;
	treeAndRates.push_back( tau );
	treeAndRates.push_back(nodeRates);
	treeAndRates.push_back(rootRate);

	
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

	// ####### Gamma Rate Het. ######
	
	ConstantNode<double> *shapePr = new ConstantNode<double>("gammaShPr", new double(0.5));
	StochasticNode<double> *srAlpha = new StochasticNode<double>("siteRates.alpha", new ExponentialDistribution(shapePr));
    ConstantNode<double> *q1 = new ConstantNode<double>("q1", new double(0.125) );
    DeterministicNode<double> *q1Value = new DeterministicNode<double>("q1_value", new QuantileFunction(q1, new GammaDistribution(srAlpha, srAlpha) ) );
    ConstantNode<double> *q2 = new ConstantNode<double>("q2", new double(0.375) );
    DeterministicNode<double> *q2Value = new DeterministicNode<double>("q2_value", new QuantileFunction(q2, new GammaDistribution(srAlpha, srAlpha) ) );
    ConstantNode<double> *q3 = new ConstantNode<double>("q3", new double(0.625) );
    DeterministicNode<double> *q3Value = new DeterministicNode<double>("q3_value", new QuantileFunction(q3, new GammaDistribution(srAlpha, srAlpha) ) );
    ConstantNode<double> *q4 = new ConstantNode<double>("q4", new double(0.875) );
    DeterministicNode<double> *q4Value = new DeterministicNode<double>("q4_value", new QuantileFunction(q4, new GammaDistribution(srAlpha, srAlpha) ) );
    std::vector<const TypedDagNode<double>* > gammaRates = std::vector<const TypedDagNode<double>* >();
    gammaRates.push_back(q1Value);
    gammaRates.push_back(q2Value);
    gammaRates.push_back(q3Value);
    gammaRates.push_back(q4Value);
    DeterministicNode<std::vector<double> > *siteRates = new DeterministicNode<std::vector<double> >( "site_rates", new VectorFunction<double>(gammaRates) );
    DeterministicNode<std::vector<double> > *siteRatesNormed = new DeterministicNode<std::vector<double> >( "site_rates_norm", new NormalizeVectorFunction(siteRates) );
    
	
	tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
	std::cout << " ** origin   " << origin->getValue() << std::endl;
	std::cout << " ** root age " << trees[0]->getRoot().getAge() << std::endl;
	
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
	phyloCTMC->setClockRate( brVector ); 
    phyloCTMC->setRateMatrix( q );
	phyloCTMC->setSiteRates( siteRatesNormed );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
	charactermodel->clamp( data[0] );
	
	std::cout << " diversification: " << div->getValue() << std::endl;
	std::cout << " turnover: " << turn->getValue() << std::endl;
	std::cout << " birth rate: " << birthRate->getValue() << std::endl;
	std::cout << " death rate: " << deathRate->getValue() << std::endl;
	
	/* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 1.0, true ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(turn, 1.0), 1.0, true ) );
	//	moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
	//	moves.push_back( new NarrowExchange( tau, 10.0 ) );
	//	moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
	//	moves.push_back( new SubtreeScale( tau, 5.0 ) );
	//	moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
	moves.push_back( new RootTimeSlide( tau, 50.0, true, 10.0 ) );
	moves.push_back( new OriginTimeSlide( origin, tau, 50.0, true, 10.0 ) );
	moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
	moves.push_back( new SimplexMove( er, 450.0, 6, 0, true, 2.0, 0.5 ) );
	moves.push_back( new SimplexMove( pi, 250.0, 4, 0, true, 2.0, 0.5 ) ); 
	moves.push_back( new SimplexMove( er, 200.0, 1, 0, false, 0.5 ) );
	moves.push_back( new SimplexMove( pi, 100.0, 1, 0, false, 0.5 ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(srAlpha, log(2.0)), 1, true ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(bmNu, 0.75), 4, true ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(rootRate, 0.5), 2, false ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(rootRate, 1.0), 2, false ) );
	moves.push_back( new ScaleSingleACLNRatesMove( nodeRates, 1.0, false, 8.0 * (double)numNodes) );
	moves.push_back( new ScaleSingleACLNRatesMove( nodeRates, 2.0, false, 8.0 * (double)numNodes) );
	moves.push_back( new RateAgeACLNMixingMove( treeAndRates, 0.02, false, 2.0 ) ); 
	
    // add some tree stats to monitor
	DeterministicNode<double> *meanNdRate = new DeterministicNode<double>("MeanNodeRate", new MeanVecContinuousValStatistic(nodeRates) );
	
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::vector<DagNode*> monitoredNodes;
	monitoredNodes.push_back( meanNdRate );
	monitoredNodes.push_back( treeHeight );
	monitoredNodes.push_back( origin );
	monitoredNodes.push_back( nodeRates );
	monitoredNodes.push_back( rootRate );
	monitoredNodes.push_back( bmNu );
	monitoredNodes.push_back( scaleRate );
	monitors.push_back( new ScreenMonitor( monitoredNodes, 10, "\t" ) );
	
	monitoredNodes.push_back( div );
	monitoredNodes.push_back( turn );
	monitoredNodes.push_back( birthRate );
	monitoredNodes.push_back( deathRate );
	monitoredNodes.push_back( pi );
    monitoredNodes.push_back( er );
    monitoredNodes.push_back( srAlpha );
	monitoredNodes.push_back( brVector );
	
	std::string logFN = "clock_test/test_rb_ACLN_6June_rn_3.log";
	monitors.push_back( new FileMonitor( monitoredNodes, 10, logFN, "\t" ) );
	
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
	
//	std::string treFN = "clock_test/test_rb_ACLN_6June_pr.tre";
//	monitors.push_back( new FileMonitor( monitoredNodes2, 10, treFN, "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(q);
	
	mcmcGenerations = 200000;

    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    myMcmc.printOperatorSummary();
	
	
	/* clean up */
	//	delete div;
	//	delete turn;
	//	delete rho;
	//	delete cp;
	//	delete branchRates;
	//	delete q;
	//	delete tau;
	delete charactermodel;
	//	delete a;
	//	delete birthRate;
	//	delete phyloCTMC;
	//	delete dLambda;
	
	
	monitors.clear();
	moves.clear();
	
    return true;
}
