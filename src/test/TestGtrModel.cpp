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
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "SubtreeScale.h"
#include "TestGtrModel.h"
#include "TimeTree.h"
#include "TreeHeightStatistic.h"
#include "TreeScale.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestGtrModel::TestGtrModel(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestGtrModel::~TestGtrModel() {
    // nothing to do
}



bool TestGtrModel::run( void ) {
    
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
    
    
//    pi->setValue( new std::vector<double>(4,1.0/4.0) );
//    er->setValue( new std::vector<double>(6,1.0/6.0) );
    
    std::cout << "pi:\t" << pi->getValue() << std::endl;
    std::cout << "er:\t" << er->getValue() << std::endl;
    
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge()*2.0 ) );
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
    
//    tau->setValue( trees[0] );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
        
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );

    // and the character model
//    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new CharacterEvolutionAlongTree<DnaState, TimeTree>(tau, q, data[0]->getNumberOfCharacters()) );
    //    StochasticNode<CharacterData<DnaState> > *charactermodel = new StochasticNode<CharacterData <DnaState> >("S", new SimpleCharEvoModel<DnaState, TimeTree>(tau, q, data[0]->getNumberOfCharacters()) );
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data[0]->getNumberOfCharacters());
    phyloCTMC->setClockRate( clockRate );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    charactermodel->clamp( data[0] );
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    moves.push_back( new NearestNeighborInterchange( tau, 5.0 ) );
    moves.push_back( new NarrowExchange( tau, 10.0 ) );
    moves.push_back( new FixedNodeheightPruneRegraft( tau, 2.0 ) );
    moves.push_back( new SubtreeScale( tau, 5.0 ) );
//    moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
    moves.push_back( new RootTimeSlide( tau, 1.0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 10.0, 1, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( er, 100.0, 6, 0, true, 2.0 ) );
    moves.push_back( new SimplexMove( pi, 100.0, 4, 0, true, 2.0 ) );
    
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
//    monitoredNodes.insert( er );
//    monitoredNodes.insert( pi );
    monitoredNodes.insert( div );
    monitors.push_back( new FileMonitor( monitoredNodes, 10, "TestGtrModel.log", "\t" ) );
    std::set<DagNode*> monitoredNodes1;
    monitoredNodes1.insert( er );
    monitoredNodes1.insert( pi );
    monitoredNodes1.insert( q );
    monitoredNodes1.insert( treeHeight );
    monitors.push_back( new FileMonitor( monitoredNodes1, 10, "TestGtrModelSubstRates.log", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes1, 10, "\t" ) );
    std::set<DagNode*> monitoredNodes2;
    monitoredNodes2.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes2, 10, "TestGtrModel.tree", "\t", false, false, false ) );
    
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
    
    std::cout << "Finished GTR model test." << std::endl;
    
    return true;
}
