
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "MixtureAllocationMove.h"
#include "MixtureDistribution.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NormalDistribution.h"
#include "ScaleProposal.h"
#include "SlidingMove.h"
#include "TestMixtureModel.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

using namespace RevBayesCore;

TestMixtureModel::TestMixtureModel(int gen) : mcmcGenerations( gen ){
    
}

TestMixtureModel::~TestMixtureModel() {
    // nothing to do
}


bool TestMixtureModel::run( void ) {
    
    /* set up the model graph */
    
    // first the priors on mu 
    ConstantNode<double> *a = new ConstantNode<double>( "a", new double(-1.0) );
    ConstantNode<double> *b = new ConstantNode<double>( "b", new double(-0.8) );
    ConstantNode<double> *c = new ConstantNode<double>( "c", new double(-0.6) );
    ConstantNode<double> *d = new ConstantNode<double>( "d", new double(-0.4) );
    ConstantNode<double> *e = new ConstantNode<double>( "e", new double(-0.2) );
    ConstantNode<double> *f = new ConstantNode<double>( "f", new double( 0.0) );
    ConstantNode<double> *g = new ConstantNode<double>( "g", new double( 0.2) );
    ConstantNode<double> *h = new ConstantNode<double>( "h", new double( 0.4) );
    ConstantNode<double> *i = new ConstantNode<double>( "i", new double( 0.6) );
    ConstantNode<double> *j = new ConstantNode<double>( "j", new double( 0.8) );
    ConstantNode<double> *k = new ConstantNode<double>( "k", new double( 1.0) );
    // then mu
    ContinuousStochasticNode *mu1 = new ContinuousStochasticNode( "mu1", new UniformDistribution(a, b) );
    ContinuousStochasticNode *mu2 = new ContinuousStochasticNode( "mu2", new UniformDistribution(b, c) );
    ContinuousStochasticNode *mu3 = new ContinuousStochasticNode( "mu3", new UniformDistribution(c, d) );
    ContinuousStochasticNode *mu4 = new ContinuousStochasticNode( "mu4", new UniformDistribution(d, e) );
    ContinuousStochasticNode *mu5 = new ContinuousStochasticNode( "mu5", new UniformDistribution(e, f) );
    ContinuousStochasticNode *mu6 = new ContinuousStochasticNode( "mu6", new UniformDistribution(f, g) );
    ContinuousStochasticNode *mu7 = new ContinuousStochasticNode( "mu7", new UniformDistribution(g, h) );
    ContinuousStochasticNode *mu8 = new ContinuousStochasticNode( "mu8", new UniformDistribution(h, i) );
    ContinuousStochasticNode *mu9 = new ContinuousStochasticNode( "mu9", new UniformDistribution(i, j) );
    ContinuousStochasticNode *mu10 = new ContinuousStochasticNode( "mu10", new UniformDistribution(j, k) );
    std::vector<const TypedDagNode<double>* > means = std::vector<const TypedDagNode<double>* >();
    means.push_back(mu1);
    means.push_back(mu2);
    means.push_back(mu3);
    means.push_back(mu4);
    means.push_back(mu5);
    means.push_back(mu6);
    means.push_back(mu7);
    means.push_back(mu8);
    means.push_back(mu9);
    means.push_back(mu10);
    
    DeterministicNode<std::vector<double> > *mus = new DeterministicNode<std::vector<double> >( "mus", new VectorFunction<double>(means) );
    ConstantNode<std::vector<double> > *mixture_probs = new ConstantNode<std::vector<double> >( "mixture_probs", new std::vector<double>(10,1.0/10.0) );
    
    StochasticNode<double> *rates = new StochasticNode<double>("rate_cats", new MixtureDistribution<double>(mus, mixture_probs) );
    
    // the prior on sigma
    ConstantNode<double> *sigma_prior = new ConstantNode<double>( "sigma_prior",  new double(1.0) );
    // then sigma
    ContinuousStochasticNode *sigma = new ContinuousStochasticNode( "sigma", new ExponentialDistribution(sigma_prior) );
    // the normal distribution expects a single value -> we need to extract the current value from the mixture object
    //ExtractParameterFromMixtureDeterministicVariable<double> *currentMu = new ExtractParameterFromMixtureDeterministicVariable<double>("current_mu", rateCats, 0);

    // finally the observation
    double values[] = {1.0180587, 0.3551789, -3.0993260, 2.0436601, 0.5752029, -2.2873659, -0.4357313, -2.1162503, -2.0887467, 0.5446856};
    ContinuousStochasticNode** x = new ContinuousStochasticNode*[10];
    for (size_t ijk = 0; ijk < 10; ++ijk) {
        x[ijk] = new ContinuousStochasticNode( "x", new NormalDistribution(rates, sigma) );
        x[ijk]->clamp( new double(values[ijk]) );
    }
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new SlidingMove( mu1, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu2, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu3, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu4, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu5, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu6, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu7, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu8, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu9, 1.0, false, 1.0 ) );
    moves.push_back( new SlidingMove( mu10, 1.0, false, 1.0 ) );
    moves.push_back( new MixtureAllocationMove<double>( rates, 1.0 ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(sigma, 1.0), 1, true ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( rates );
    monitoredNodes.insert( sigma );
    monitors.push_back( new FileMonitor( monitoredNodes, 1, "mixtureModelTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(a);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    for (size_t ijk = 0; ijk < 10; ++ijk) {
        delete x[ijk];
    }
    delete [] x;
    delete sigma_prior;
    delete sigma;
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
    delete i;
    delete j;
    delete k;
    delete mu1;
    delete mu2;
    delete mu3;
    delete mu4;
    delete mu5;
    delete mu6;
    delete mu7;
    delete mu8;
    delete mu9;
    delete mu10;
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
