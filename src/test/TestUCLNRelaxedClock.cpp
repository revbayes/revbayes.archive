#include "BinaryDivision.h"
#include "BinaryMultiplication.h"
#include "BinarySubtraction.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantBirthDeathProcess.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GammaDistribution.h"
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
#include "RootTimeSlide.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SimpleBranchRateTimeCharEvoModel.h"
#include "SubtreeScale.h"
#include "TestUCLNRelaxedClock.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

using namespace RevBayesCore;

TestUCLNRelaxedClock::TestUCLNRelaxedClock(const std::string &aFn, const std::string &tFn, int gen) : alignmentFilename( aFn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestUCLNRelaxedClock::~TestUCLNRelaxedClock() {
    // nothing to do
}

// todo:
/*   - need a RateNode
     - need NodeRate move (or just do a simple sliding move on the rate at each branch)
*/
bool TestUCLNRelaxedClock::run( void ) {
   
	std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;

	// First, we read in the data 
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    
    
    // first the priors
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(0.5) );
    ConstantNode<double> *b = new ConstantNode<double>("b", new double(0.25) );
	
    
    // then the parameters
//    ContinuousStochasticNode *expectLN = new ContinuousStochasticNode( "UCLN.expectation", new ExponentialDistribution(a) ); // the expectation of the LN dist so mu = log(expectLN) - (sigLN^2)/2
//    ContinuousStochasticNode *sigLN = new ContinuousStochasticNode("UCLN.variance", new ExponentialDistribution(b) );
//    
//	DeterministicNode<double> *logExpLN = new DeterministicNode<double>("logUCLN.exp", new LnFunction(expectLN) );
//	DeterministicNode<double> *squareSigLN = new DeterministicNode<double>("squareSigLN", new BinaryMultiplication<double, double, double>(sigLN, sigLN) );
//	DeterministicNode<double> *divSqSigLN = new DeterministicNode<double>("divSqSigLN", new BinaryDivision<double, double, double>(squareSigLN, new ConstantNode<double>( "2", new double (2.0))) );
//	DeterministicNode<double> *muValLN = new DeterministicNode<double>("MuValLN", new BinarySubtraction<double, double, double>(logExpLN, divSqSigLN) );

	
    TimeTree* t = trees[0];

    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(100.0)) ));
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
	
	
	size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
	
	std::vector<const TypedDagNode<double> *> branchRates;
	std::vector< ContinuousStochasticNode *> branchRates_nonConst;
	for( int i=0; i<numBranches; i++){
        std::ostringstream br_name;
        br_name << "br(" << i << ")";
//		ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new LognormalDistribution(muValLN, sigLN));
//		ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new GammaDistribution(a, b));
		ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new LognormalDistribution(a, b));
		branchRates.push_back( tmp_branch_rate );
		branchRates_nonConst.push_back( tmp_branch_rate );
	}
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchRates ) );

	
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
	
	tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;

    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new SimpleBranchRateTimeCharEvoModel<DnaState, TimeTree>(tau, q, br_vector, true, data[0]->getNumberOfCharacters()) );
	charactermodel->clamp( static_cast<CharacterData<DnaState> *>( data[0] ) );
 
	/* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(div, 1.0, true, 2.0) );
//    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
//    moves.push_back( new NarrowExchange( tau, 10.0 ) );
//    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 4, 0, true, 2.0 ) );
	for( int i=0; i<numBranches; i++){
		moves.push_back( new ScaleMove(branchRates_nonConst[i], 1.0, true, 2.0) );
	}
	
	

    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
	//    monitoredNodes.insert( er );
	//    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
	for( int i=0; i<numBranches; i++){
		monitoredNodes.insert( branchRates_nonConst[i] );
	}
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestUCLNModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );
    monitoredNodes1.insert( q );
    monitoredNodes1.insert( treeHeight );
//    monitoredNodes1.insert( muValLN );
	
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestUCLNGtrModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestUCLNGtrModel.tree", "\t", false, false, false ) );
    
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
