
#include "ConstantNode.h"
#include "DirichletDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "SimplexMove.h"
#include "StochasticNode.h"
#include "TestSimplexMove.h"

using namespace RevBayesCore;

TestSimplexMove::TestSimplexMove(int gen) : mcmcGenerations( gen ){
    
}

TestSimplexMove::~TestSimplexMove() {
    // nothing to do
}


bool TestSimplexMove::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<std::vector<double> > *a = new ConstantNode<std::vector<double> >( "a", new std::vector<double>(4,50) );
    // then x
    StochasticNode<std::vector<double> > *x = new StochasticNode<std::vector<double> >( "x", new DirichletDistribution(a) );
    
    std::vector<double> *x_val = new std::vector<double>();
    x_val->push_back(0.01);
    x_val->push_back(0.02);
    x_val->push_back(0.02);
    x_val->push_back(0.95);
    x->setValue( x_val );
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new SimplexMove( x, 100.0, 4, 0, false, 1.0 ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    monitors.push_back( new FileMonitor( x, 1, "SimplexMoveTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    delete x;
    delete a;
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
