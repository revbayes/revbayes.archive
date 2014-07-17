
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NormalDistribution.h"
#include "ScaleProposal.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TestNormalModel.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestNormalModel::TestNormalModel(int gen) : mcmcGenerations( gen ){
    
}

TestNormalModel::~TestNormalModel() {
    // nothing to do
}


bool TestNormalModel::run( void ) {
        
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<double> *a = new ConstantNode<double>( "a", new double(-1.0) );
    ConstantNode<double> *b = new ConstantNode<double>( "b", new double( 1.0) );
    // then mu
    ContinuousStochasticNode *mu = new ContinuousStochasticNode( "mu", new UniformDistribution(a, b) );
    // the prior on sigma
    ConstantNode<double> *c = new ConstantNode<double>( "c", new double( 1.0) );
    // then sigma
    ContinuousStochasticNode *sigma = new ContinuousStochasticNode( "sigma", new ExponentialDistribution(c) );
    // finally the observation
    double values[] = {1.0180587, 0.3551789, -3.0993260, 2.0436601, 0.5752029, -2.2873659, -0.4357313, -2.1162503, -2.0887467, 0.5446856};
    ContinuousStochasticNode** x = new ContinuousStochasticNode*[10];
    for (size_t i = 0; i < 10; ++i) {
        x[i] = new ContinuousStochasticNode( "x", new NormalDistribution(mu, sigma) );
        x[i]->clamp( new double(values[i]) );
    }
    
    /* add the moves */
    RbVector<Move> moves;
    moves.push_back( new SlidingMove( mu, 1.0, 1.0, false ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(sigma, 1.0), 1, true ) );
    
    /* add the monitors */
    RbVector<Monitor> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( mu );
    monitoredNodes.insert( sigma );
    monitors.push_back( new FileMonitor( monitoredNodes, 1, "normalModelTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    for (size_t i = 0; i < 10; ++i) {
        delete x[i];
    }
    delete [] x;
    delete mu;
    delete sigma;
    delete a;
    delete b;
    delete c;
    
    return true;
}
