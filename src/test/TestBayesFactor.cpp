
#include "BinomialDistribution.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DistributionBeta.h"
#include "Model.h"
#include "Move.h"
#include "PathSampler.h"
#include "PowerPosteriorMcmc.h"
#include "SlidingMove.h"
#include "SteppingStoneSampler.h"
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
    RbVector<Move> moves;
    moves.push_back( new SlidingMove( p, 1.0, false, 1.0 ) );
        
    /* instantiate the model */
    Model myModel = Model(p);
    
    /* instiate and run the MCMC */
    std::string filename = "BayesFacter_test.ss";
    PowerPosteriorMcmc myMcmc = PowerPosteriorMcmc( myModel, moves, filename );
    std::vector<double> beta;
    int k = 50;
    for (int i = k; i >= 0; --i) {
        double b = RbStatistics::Beta::quantile(0.3,1.0,i / double(k));
        beta.push_back( b );
    }
    myMcmc.setPowers( beta );
    myMcmc.setSampleFreq( 10 );
    
    myMcmc.run(mcmcGenerations);
    
    PathSampler pSampler = PathSampler(filename, "power", "likelihood", "\t");
    SteppingStoneSampler sSampler = SteppingStoneSampler(filename, "power", "likelihood", "\t");
    
    
    double sss = sSampler.marginalLikelihood();
    double ps = pSampler.marginalLikelihood();
    
    std::cout << "True marginal likelihood:\t\t" << log(0.004975) << std::endl;
    std::cout << "Stepping-Stone-Sampling:\t\t" << sss << std::endl;
    std::cout << "Path-Sampling:\t\t\t\t\t" << ps << std::endl;
    
    
    
    /* clean up */
    delete x;
//    delete p;
//    delete l;
//    delete u;
    
    return true;
}
