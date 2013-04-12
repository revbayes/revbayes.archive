
#include "CompletePathRescaleMove.h"
#include "ConstantNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "PathSampleMonitor.h"
#include "PathResampleMove.h"
#include "PathValueScalingMove.h"
#include "GeometricBrownianMotion.h"
#include "BrownianMotion.h"
#include "StochasticNode.h"
#include "TestBrownianMotion.h"

using namespace RevBayesCore;

TestBrownianMotion::TestBrownianMotion(int gen) : mcmcGenerations( gen ){
    
}

TestBrownianMotion::~TestBrownianMotion() {
    // nothing to do
}


bool TestBrownianMotion::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<double> *a = new ConstantNode<double>( "a", new double(10.0) );
    ConstantNode<double> *b = new ConstantNode<double>( "b", new double(0.5) );
    ConstantNode<double> *c = new ConstantNode<double>( "c", new double(10.0) );
    // then x
    StochasticNode<UnivariateFunction> *x = new StochasticNode<UnivariateFunction>( "x", new BrownianMotion(a, b, c, 10) );
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new PathValueScalingMove( x, 1.0, 1.0, true ) );
    moves.push_back( new PathResampleMove(x, 3, 0.2, 1.0, false) );
    moves.push_back( new PathResampleMove(x, 1, 0.5, 1.0, false) );
//    moves.push_back( new CompletePathRescaleMove( x, 1.0, 1.0, true ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    monitors.push_back( new PathSampleMonitor( x, 100, 0.0, c->getValue(), 10, "BrownianMotionTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    delete x;
    delete a;
    delete b;
    delete c;
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
