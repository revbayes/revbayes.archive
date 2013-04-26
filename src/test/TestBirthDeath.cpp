
#include "Clade.h"
#include "ConstantBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "RbFileManager.h"
#include "ScaleMove.h"
#include "SlidingMove.h"
#include "TestBirthDeath.h"
#include "TimeTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestBirthDeath::TestBirthDeath(const std::string &tfn, int gen) : treeFilename( tfn ), mcmcGenerations( gen ){
    
}

TestBirthDeath::~TestBirthDeath() {
    // nothing to do
}


bool TestBirthDeath::run( void ) {
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
    // first the priors
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(0.0) );
    ConstantNode<double> *b = new ConstantNode<double>("b", new double(1000.0) );
    ConstantNode<double> *c = new ConstantNode<double>("c", new double(0.01) );
        
    
    // then the parameters
    ContinuousStochasticNode *lambda = new ContinuousStochasticNode( "diversification", new UniformDistribution(a, b) );
//    ExponentialDistributedRandomVariable *lambda = new ExponentialDistributedRandomVariable( "diversification", a );
    ContinuousStochasticNode *mu = new ContinuousStochasticNode("turnover", new ExponentialDistribution(c) );
    //    UniformDistributedRandomVariable *mu = new UniformDistributedRandomVariable("turnover", c, d);
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    
    TimeTree* t = trees[0];
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(lambda, mu, met, mep, rho, "uniform", "survival",int(t->getNumberOfTips()), t->getNames(), std::vector<Clade>()) );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // attach the data
    tau->clamp( t );
    
    std::cout << "tau-clamped:\t" << tau->getValue() << std::endl;
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove( lambda, 1.0, 1.0, true ) );
    moves.push_back( new ScaleMove( mu, 1.0, 1.0, true ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( lambda );
    monitoredNodes.insert( mu );
    monitors.push_back( new FileMonitor( monitoredNodes, 100, "BirthDeathTest.log", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(lambda);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    /* clean up */
    delete mu;
    delete lambda;
    delete a;
    delete b;
    delete c;
    delete tau;
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
//    for (std::vector<TimeTree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
//        const TimeTree *theTree = *it;
//        delete theTree;
//    }
    
    return true;
}
