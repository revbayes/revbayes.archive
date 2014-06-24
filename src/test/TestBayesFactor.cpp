
#include "BinomialDistribution.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DistributionBeta.h"
#include "Model.h"
#include "Move.h"
#include "PowerPosteriorMcmc.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TestBayesFactor.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestBayesFactor::TestBayesFactor(int gen) : mcmcGenerations( gen ){
    
}

TestBayesFactor::~TestBayesFactor() {
    // nothing to do
}


bool TestBayesFactor::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on p 
    ConstantNode<double> *l = new ConstantNode<double>( "lower", new double(0.0) );
    ConstantNode<double> *u = new ConstantNode<double>( "upper", new double(1.0) );
    // then p
    ContinuousStochasticNode *p = new ContinuousStochasticNode( "p", new UniformDistribution(l,u) );
    
    // now the observation
    ConstantNode<int> *n = new ConstantNode<int>( "n", new int(200) );
    StochasticNode<int> *x = new StochasticNode<int>("x", new BinomialDistribution(n,p) );
    x->clamp( new int(115) );
    
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new SlidingMove( p, 1.0, false, 1.0 ) );
        
    /* instantiate the model */
    Model myModel = Model(p);
    
    /* instiate and run the MCMC */
    PowerPosteriorMcmc myMcmc = PowerPosteriorMcmc( myModel, moves );
    std::vector<double> beta;
    int k = 50;
    for (int i = k; i >= 0; --i) {
        double b = RbStatistics::Beta::quantile(0.3,1.0,i / double(k));
        beta.push_back( b );
    }
    myMcmc.setBeta( beta );
    myMcmc.setSampleFreq( 10 );
    
    myMcmc.run(mcmcGenerations);
    double sss = myMcmc.steppingStoneSampling();
    double ps = myMcmc.pathSampling();
    
    std::cout << "True marginal likelihood:\t\t" << log(0.004975) << std::endl;
    std::cout << "Stepping-Stone-Sampling:\t\t" << sss << std::endl;
    std::cout << "Path-Sampling:\t\t\t\t\t" << ps << std::endl;
    
    
    
    /* clean up */
    delete x;
    delete p;
    delete l;
    delete u;
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    
    return true;
}
