
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "ScaleProposal.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TestSlidingMove.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestSlidingMove::TestSlidingMove(int gen) : mcmcGenerations( gen ){
    
}

TestSlidingMove::~TestSlidingMove() {
    // nothing to do
}


bool TestSlidingMove::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<double> *a = new ConstantNode<double>( "a", new double(-1.0) );
    ConstantNode<double> *b = new ConstantNode<double>( "b", new double( 1.0) );
    // then x
    ContinuousStochasticNode *x = new ContinuousStochasticNode( "x", new UniformDistribution(a, b) );
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new SlidingMove( x, 1.0, 1.0, false ) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    monitors.push_back( new FileMonitor( x, 1, "SlidingMoveTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    delete x;
    delete a;
    delete b;
    
    return true;
}
