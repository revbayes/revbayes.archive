#include "AbstractCharacterData.h"
#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "NearestNeighborInterchange_nonClock.h"
#include "RbFileManager.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "TestGtrUnrooted.h"
#include "TreeAssemblyFunction.h"
#include "Topology.h"
#include "UniformDistribution.h"
#include "UniformTopologyDistribution.h"
#include "VectorFunction.h"

using namespace RevBayesCore;

TestGtrUnrooted::TestGtrUnrooted(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestGtrUnrooted::~TestGtrUnrooted() {
    // nothing to do
}



bool TestGtrUnrooted::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
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
    
    // first we create the random variable for the topology
    std::vector<std::string> names = data[0]->getTaxonNames();
    StochasticNode<Topology> *tau = new StochasticNode<Topology>( "tau", new UniformTopologyDistribution(int(names.size()), names) );
    
    // second, we create the vector of branch lengths
    size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
	std::vector<const TypedDagNode<double> *> branchLength;
	std::vector< ContinuousStochasticNode *> branchLength_nonConst;
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(1.0) );
	for( size_t i=0; i<numBranches; i++){
        std::ostringstream br_name;
        br_name << "brlen(" << i << ")";
		ContinuousStochasticNode* tmp_branch_rate = new ContinuousStochasticNode( br_name.str(), new ExponentialDistribution(a));
		branchLength.push_back( tmp_branch_rate );
		branchLength_nonConst.push_back( tmp_branch_rate );
	}
	//Build a node out of the vector of nodes
    DeterministicNode< std::vector< double > >* br_vector = new DeterministicNode< std::vector< double > >( "br_vector", new VectorFunction< double >( branchLength ) );

    
    // and thirdly, we create the unrooted tree from the topology and the branch lengths
    DeterministicNode<BranchLengthTree> *psi = new DeterministicNode<BranchLengthTree>( "psi", new TreeAssemblyFunction(tau, br_vector) );
    
    //    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    
    // and the character model
    GeneralBranchHeterogeneousCharEvoModel<DnaState, BranchLengthTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, BranchLengthTree>(psi, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setClockRate( clockRate );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 4, 0, true, 2.0 ) );
    moves.push_back( new NearestNeighborInterchange_nonClock( tau, 5.0 ) );
    
    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(branchLength_nonConst[i], 10.0), .5, true ) );
    }
        
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestGtrUnrooted.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );
    monitoredNodes1.insert( q );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestGtrUnrootedSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestGtrUnrooted.tree", "\t", false, false, false ) );
    
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
    //    delete sigma;
    delete a;
    //    delete b;
    //    delete c;
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
