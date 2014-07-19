
#include "ConstantNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "ScaleProposal.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TestScalingMove.h"

using namespace RevBayesCore;

TestScalingMove::TestScalingMove(int gen) : mcmcGenerations( gen ){
    
}

TestScalingMove::~TestScalingMove() {
    // nothing to do
}


bool TestScalingMove::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<double> *a = new ConstantNode<double>( "a", new double(0.10) );
    // then x
    StochasticNode<double> *x = new StochasticNode<double>( "x", new ExponentialDistribution(a) );
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(x, 1.0), 1, true ) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    monitors.push_back( new FileMonitor( x, 1, "ScalingMoveTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    delete x;
    delete a;
    
    return true;
}
