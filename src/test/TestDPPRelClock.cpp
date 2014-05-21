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
    
    
    // birth-death process priors
    ConstantNode<double> *div = new ConstantNode<double>("diversification", new double(0.05));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.5));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
	
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );
	
    std::cout << "bf:\t" << bf->getValue() << std::endl;
    std::cout << "e:\t" << e->getValue() << std::endl;
	
    // then the parameters
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );
    
    std::cout << "pi:\t" << pi->getValue() << std::endl;
    std::cout << "er:\t" << er->getValue() << std::endl;
	
	
	size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
	
//	ConstantNode<double> *exCP = new ConstantNode<double>("concentrp", new double(RbStatistics::Helper::dppConcParamFromNumTables(4.0, (double)numBranches)) );
//	ConstantNode<double> *dpA = new ConstantNode<double>("dp_a", new double(2.0) );
//	ConstantNode<double> *dpB = new ConstantNode<double>("dp_b", new double(dpA->getValue() / exCP->getValue()) );
//	StochasticNode<double> *cp = new StochasticNode<double>("cp", new GammaDistribution(dpA, dpB) );

	ConstantNode<double> *cp = new ConstantNode<double>("concentrp", new double(RbStatistics::Helper::dppConcParamFromNumTables(3.0, (double)numBranches)) );
	
	// G_0 is an exponential distribution
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(1.0) );
	TypedDistribution<double> *g = new ExponentialDistribution(a);
	
	// Branch rates
	StochasticNode<std::vector<double> > *branchRates = new StochasticNode<std::vector<double> >("branchRates", new DirichletProcessPriorDistribution<double>(g, cp, (int)numBranches) );
	
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*1.5 ) );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
	
	tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
	
	std::cout << " ** " << origin->getValue() << std::endl;

    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
	phyloCTMC->setClockRate( branchRates );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
	charactermodel->clamp( data[0] );
	
	std::cout << " branch rates: " << branchRates->getValue() << std::endl;

	/* add the moves */
    std::vector<Move*> moves;
//    moves.push_back( new ScaleMove(div, 1.0, true, 2.0) );
//    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
//    moves.push_back( new NarrowExchange( tau, 10.0 ) );
//    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
//    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
//    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 450.0, 6, 0, true, 2.0, 1.0 ) ); 
    moves.push_back( new SimplexMove( pi, 250.0, 4, 0, true, 2.0, 1.0 ) ); 
//    moves.push_back( new SimplexMove( er, 600.0, 6, 0, false, 2.0 ) );
//    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, false, 2.0, 1.0 ) );
    moves.push_back( new DPPScaleCatValsMove( branchRates, log(2.0), 2.0 ) );
    moves.push_back( new DPPAllocateAuxGibbsMove<double>( branchRates, 4, 3.0 ) );
//    moves.push_back( new DPPGibbsConcentrationMove<double>( cp, branchRates, 4, 2.0 ) );
		
	
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    DeterministicNode<int> *numCats = new DeterministicNode<int>("DPPNumCats", new DppNumTablesStatistic<double>(branchRates) );
	DeterministicNode<double> *meanBrRate = new DeterministicNode<double>("MeanBranchRate", new DppMeanContinuousValStatistic(branchRates) );
	
//	Clade nd14 = new Clade(

    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::vector<DagNode*> monitoredNodes;
	monitoredNodes.push_back( pi );
	monitoredNodes.push_back( treeHeight );
    monitoredNodes.push_back( numCats );
    monitoredNodes.push_back( meanBrRate );
    monitoredNodes.push_back( cp );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 10, "\t" ) );
 
    monitoredNodes.push_back( er );
 	monitoredNodes.push_back( branchRates );
	monitors.push_back( new FileMonitor( monitoredNodes, 10, "clock_test/test_RBdpp_clock_pm3.log", "\t" ) );

    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "clock_test/test_RBdpp_clock_pm3.tree", "\t", false, false, false ) );
    
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
