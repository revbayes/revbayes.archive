#include "BranchHeterogeneousCharEvoModel.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GtrRateMatrixFunction.h"
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
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestBranchHeterogeneousGtrModel.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVectorFunction.h"

using namespace RevBayesCore;

TestBranchHeterogeneousGtrModel::TestBranchHeterogeneousGtrModel(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestBranchHeterogeneousGtrModel::~TestBranchHeterogeneousGtrModel() {
    // nothing to do
}



bool TestBranchHeterogeneousGtrModel::run( void ) {
    
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
    
    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("div_lower", new double(0.0)), new ConstantNode<double>("div_upper", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );

    //Root frequencies
    StochasticNode<std::vector<double> > *rf = new StochasticNode<std::vector<double> >( "rf", new DirichletDistribution(bf) );

    
    std::cout << "bf:\t" << bf->getValue() << std::endl;
    std::cout << "e:\t" << e->getValue() << std::endl;
    
    //Declaring a vector of GTR matrices
     size_t numBranches = 2*data[0]->getNumberOfTaxa() - 2;
    std::vector<StochasticNode < std::vector<double> >* > pis;
  //  std::vector<StochasticNode < std::vector<double> >* > ers;
    std::vector< const TypedDagNode< RateMatrix >* > qs;

    
    StochasticNode<std::vector<double> > * er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) ;

    for (unsigned int i = 0 ; i < numBranches ; i++ ) {
        std::ostringstream pi_name;
        pi_name << "pi(" << i << ")";
        pis.push_back(new StochasticNode<std::vector<double> >( pi_name.str(), new DirichletDistribution(bf) ) );
        //  ers.push_back(new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) );
        std::ostringstream q_name;
        q_name << "q(" << i << ")";
        qs.push_back(new DeterministicNode<RateMatrix>( q_name.str(), new GtrRateMatrixFunction(er, pis[i]) ));
        std::cout << "Q:\t" << qs[i]->getValue() << std::endl;
    }
    
    DeterministicNode< RbVector< RateMatrix > >* qs_node = new DeterministicNode< RbVector< RateMatrix > >( "q_vector", new RbVectorFunction<RateMatrix>(qs) );
    
            
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
    
    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // and the character model
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", new BranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, qs_node, rf, true, data[0]->getNumberOfCharacters()) );
    charactermodel->clamp( data[0] );
    
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(div, 1.0, true, 2.0) );
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 100.0, 4, 0, true, 2.0 ) );

    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
   //     moves.push_back( new SimplexMove( ers[i], 10.0, 1, true, 2.0 ) );
        moves.push_back( new SimplexMove( pis[i], 10.0, 1, 0, true, 2.0 ) );
    //    moves.push_back( new SimplexMove( ers[i], 100.0, 6, true, 2.0 ) );
        moves.push_back( new SimplexMove( pis[i], 100.0, 4, 0, true, 2.0 ) );
    }
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestBranchHeterogeneousGtrModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
/*    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
        monitoredNodes1.insert( pis[i] );
    }*/
    monitoredNodes1.insert( rf );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestBranchHeterogeneousGtrModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestBranchHeterogeneousGtrModel.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(qs[0]);
    
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
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
