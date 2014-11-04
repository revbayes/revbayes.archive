#include "Clade.h"
#include "ConstantNode.h"
#include "ConstantRateBirthDeathProcess.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "FixedNodeheightPruneRegraft.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "MixtureAllocationMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NarrowExchange.h"
#include "NclReader.h"
#include "NearestNeighborInterchange.h"
#include "NodeTimeSlideBeta.h"
#include "NodeTimeSlideUniform.h"
#include "RbFileManager.h"
#include "RbMixtureDistribution.h"
#include "RbVector.h"
#include "RootTimeSlide.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestMixtureBranchHeterogeneousGtrModel.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVectorFunction.h"

using namespace RevBayesCore;

TestMixtureBranchHeterogeneousGtrModel::TestMixtureBranchHeterogeneousGtrModel(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestMixtureBranchHeterogeneousGtrModel::~TestMixtureBranchHeterogeneousGtrModel() {
    // nothing to do
}



bool TestMixtureBranchHeterogeneousGtrModel::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    std::vector<AbstractCharacterData*> data = NclReader().readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    std::cout << data[0] << std::endl;
    
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // birth-death process priors
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
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
    std::vector< const TypedDagNode < RateMatrix >* > q_mixture;
    
    
    StochasticNode<std::vector<double> > * er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) ;
    
    size_t numCats = 10;
    for (size_t i = 0 ; i < numCats ; i++ ) {
        std::ostringstream pi_name;
        pi_name << "pi(" << i << ")";
        pis.push_back(new StochasticNode<std::vector<double> >( pi_name.str(), new DirichletDistribution(bf) ) );
        //  ers.push_back(new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) ) );
        std::ostringstream q_name;
        q_name << "q_mixture(" << i << ")";
        q_mixture.push_back(new DeterministicNode<RateMatrix>( q_name.str(), new GtrRateMatrixFunction(er, pis[i]) ));
        std::cout << "Q:\t" << q_mixture[i]->getValue() << std::endl;
    }
    
    
    ConstantNode< std::vector<double> > *probs = new ConstantNode< std::vector< double > >( "mixture_probs", new std::vector<double>(numCats,1.0/numCats) );
    DeterministicNode< RbVector<RateMatrix> > *q_mixture_values = new DeterministicNode< RbVector<RateMatrix> >( "mix_val", new RbVectorFunction<RateMatrix>(q_mixture) );
    std::vector<const TypedDagNode<RateMatrix> *> q_branch;
    std::vector<StochasticNode<RateMatrix> *> q_branch_non_const;
    for (size_t i = 0 ; i < numBranches ; i++ ) {
        std::ostringstream q_name;
        q_name << "q_branch(" << i << ")";
        StochasticNode<RateMatrix>* q_tmp = new StochasticNode<RateMatrix>(q_name.str(), new RbMixtureDistribution<RateMatrix>(q_mixture_values,probs) );
        q_branch.push_back( q_tmp );
        q_branch_non_const.push_back( q_tmp );
        
    }
    
    DeterministicNode< RbVector< RateMatrix > >* qs_node = new DeterministicNode< RbVector< RateMatrix > >( "q_vector", new RbVectorFunction<RateMatrix>(q_branch) );
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
    
   // tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // and the character model
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setRootFrequencies( rf );
    phyloCTMC->setRateMatrix( qs_node );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 1, true ) );
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 20.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 500.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 20.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( rf, 500.0, 4, 0, true, 2.0 ) );
    
    for (unsigned int i = 0 ; i < numCats ; i ++ ) {
        //     moves.push_back( new SimplexMove( ers[i], 10.0, 1, true, 2.0 ) );
        moves.push_back( new SimplexMove( pis[i], 10.0, 1, 0, true, 2.0 ) );
        //    moves.push_back( new SimplexMove( ers[i], 100.0, 6, true, 2.0 ) );
        moves.push_back( new SimplexMove( pis[i], 100.0, 4, 0, true, 2.0 ) );
    }
    // TODO: This is currently commented out while we are still working on a good implementation for vectors. (Sebastian)
//    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
//        moves.push_back( new MixtureAllocationMove<RateMatrix>( q_branch_non_const[i], 1.0 ) );
//    }
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    //    monitoredNodes.insert( er );
    //    monitoredNodes.insert( pi );
  
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestMixtureBranchHeterogeneousGtrModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    /*    for (unsigned int i = 0 ; i < numBranches ; i ++ ) {
     monitoredNodes1.insert( pis[i] );
     }*/
    monitoredNodes1.insert( rf );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    for (unsigned int i = 0 ; i < numCats ; i ++ ) {
    monitoredNodes1.insert( pis[i] );
    }
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestMixtureBranchHeterogeneousGtrModelSubstRates.log", "\t" ) );

    std::set<DagNode*> monitoredNodes2;
    
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestMixtureBranchHeterogeneousGtrModel.tree", "\t", false, false, false ) );
    
    /* instantiate the model */
    Model myModel = Model(rf);
    
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
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
