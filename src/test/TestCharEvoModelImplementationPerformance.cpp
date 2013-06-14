#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "Mcmc.h"
#include "Model.h"
#include "ModelMonitor.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "RbFileManager.h"
#include "ScreenMonitor.h"
#include "SimplexSingleElementScale.h"
#include "SimpleSiteHomogeneousCharEvoModel.h"
#include "TestCharEvoModelImplementationPerformance.h"
#include "TimeTree.h"

#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

using namespace RevBayesCore;

TestCharEvoModelImplementationPerformance::TestCharEvoModelImplementationPerformance(const std::string &afn, const std::string &tFn, int gen) : alignmentFilename( afn ), treeFilename( tFn ), mcmcGenerations( gen ){
    
}

TestCharEvoModelImplementationPerformance::~TestCharEvoModelImplementationPerformance() {
    // nothing to do
}


bool TestCharEvoModelImplementationPerformance::run( void ) {
    
    std::cerr << mcmcGenerations << std::endl;
    
    clock_t t;
    t = clock();
    
    // fix the rng seed
    std::vector<unsigned int> seed;
    seed.push_back(25);
    seed.push_back(42);
    GLOBAL_RNG->setSeed(seed);
    
    /* First, we read in the data */
    // the matrix
    NclReader& reader = NclReader::getInstance();
    std::vector<AbstractCharacterData*> data = reader.readMatrices(alignmentFilename);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
    
    // GTR model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(6,1.0) );
        
    // then the parameters
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );
                
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
        
    ConstantNode<TimeTree> *tau = new ConstantNode<TimeTree>( "tau", trees[0] );
        
    // and the character model
    size_t numChar = data[0]->getNumberOfCharacters();
//    ConstantNode<double> *rm = new ConstantNode<double>("rateMultiplier", new double(1.0) );
//    SimpleSiteHomogeneousCharEvoModel<DnaState, TimeTree> *charModel = new SimpleSiteHomogeneousCharEvoModel<DnaState, TimeTree>(tau, q, rm, true, numChar ); 
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *charModel = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, true, numChar ); 
    charModel->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", charModel );
    charactermodel->clamp( static_cast<CharacterData<DnaState> *>( data[0] ) );
    
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new SimplexSingleElementScale( er, 10.0, true, 6.0 ) );
    moves.push_back( new SimplexSingleElementScale( pi, 10.0, true, 4.0 ) );
        
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( pi );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 100, "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(q);

    monitors.push_back( new ModelMonitor( &myModel, 100, "TestCharEvoModelImplementationPerformance.log", "\t" ) );

    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    myMcmc.printOperatorSummary();
    
    /* clean up */
    
    t = clock() - t;
    printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    
    return true;
}
