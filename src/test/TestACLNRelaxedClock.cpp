#include "AutocorrelatedLognormalRateDistribution.h"
#include "BinaryDivision.h"
#include "BinaryMultiplication.h"
#include "BinarySubtraction.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "ExponentialFunction.h"
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
#include "RandomNumberGenerator.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SingleElementScaleMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestACLNRelaxedClock.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "ExtendedNewickTreeMonitor.h"
#include "TreeScale.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestACLNRelaxedClock::TestACLNRelaxedClock(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestACLNRelaxedClock::~TestACLNRelaxedClock() {
    // nothing to do
}

// todo:
/*   - need a RateNode
 - need NodeRate move (or just do a simple sliding move on the rate at each branch)
 */
bool TestACLNRelaxedClock::run( void ) {
    
    std::cout << "Seed:\t\t" << GLOBAL_RNG->getSeed() << std::endl; 
    
	std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
	// First, we read in the data 
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
	
    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
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
	
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
	
	tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // the priors of the ACLN
    ConstantNode<double> *sigma_prior = new ConstantNode<double>("sigma_prior", new double(1.0));
    ContinuousStochasticNode *sigma = new ContinuousStochasticNode("sigma", new ExponentialDistribution(sigma_prior) );
    ConstantNode<double> *root_rate_prior_mean = new ConstantNode<double>("root_rate_prior_mean", new double(0.0));
    ConstantNode<double> *root_rate_prior_sigma = new ConstantNode<double>("root_rate_prior_sigma", new double(1.0));
    ContinuousStochasticNode *root_rate = new ContinuousStochasticNode("root_rate", new NormalDistribution(root_rate_prior_mean, root_rate_prior_sigma) );
    DeterministicNode<double> *expRootRate = new DeterministicNode<double>("exp_root_rate", new ExponentialFunction(root_rate));
    
    // the ACLN branch rates
	StochasticNode< std::vector< double > > *branchRates = new StochasticNode< std::vector< double > >("branchRates", new AutocorrelatedLognormalRateDistribution(tau, sigma, expRootRate) );

    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setClockRate( branchRates );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
	charactermodel->clamp( data[0] );
    
	/* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    //    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    //    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    //    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 15.0 ) );
//    moves.push_back( new NodeTimeSlideBeta(tau, 10, true, 15.0 ));
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 4, 0, true, 2.0 ) );
//    moves.push_back( new ScaleMove(sigma, 1.0, true, 1.0) );
//    moves.push_back( new SlidingMove(root_rate, 0.25, false, 1.0) );
//    moves.push_back( new SingleElementScaleMove(branchRates, 1.0, true, 15.0) );
    
	
	
    
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestACLNModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );
    monitoredNodes1.insert( q );
    monitoredNodes1.insert( treeHeight );
    monitoredNodes1.insert( branchRates );
    //    monitoredNodes1.insert( muValLN );
	
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestACLNGtrModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestACLNGtrModel.tree", "\t", false, false, false ) );
    std::set<TypedDagNode<std::vector<double> > *> monitoredNodeVars;
    monitoredNodeVars.insert( branchRates );
    monitors.push_back( new ExtendedNewickTreeMonitor( tau, monitoredNodeVars, 10, "TestACLNGtrModelExtended.tree", "\t", false, false, false ) );

    
    /* instantiate the model */
    Model myModel = Model(q);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    myMcmc.printOperatorSummary();
    
    /* clean up */
    //    for (size_t i = 0; i < 10; ++i) {
    //        delete x[i];
    //    }
    //    delete [] x;
    delete div;
    //    delete sigma;
    //    delete a;
    //    delete b;
    //    delete c;
    
    return true;
}
