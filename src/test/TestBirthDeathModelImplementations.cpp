
#include "TimeDependentBirthDeathProcess.h"
#include "Clade.h"
#include "CompletePathRescaleMove.h"
#include "ConstantBirthDeathProcess.h"
#include "ConstantFunction.h"
#include "ConstantNode.h"
#include "GeometricBrownianMotion.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "OneOverXDistribution.h"
#include "PathValueScalingMove.h"
#include "PathSampleMonitor.h"
#include "RbFileManager.h"
#include "ScaleMove.h"
#include "Skyline.h"
#include "SlidingMove.h"
#include "StepFunction.h"
#include "TestBirthDeathModelImplementations.h"
#include "TimeTree.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestBirthDeathModelImplementations::TestBirthDeathModelImplementations(const std::string &tfn, int gen) : treeFilename( tfn ), mcmcGenerations( gen ){
    
}

TestBirthDeathModelImplementations::~TestBirthDeathModelImplementations() {
    // nothing to do
}


bool TestBirthDeathModelImplementations::run( void ) {
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
//    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* testing the likelihood implementation */
    // first the priors
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(0.0) );
    ConstantNode<double> *b = new ConstantNode<double>("b", new double(1000.0) );
    ConstantNode<double> *c = new ConstantNode<double>("c", new double(0.01) );
        
        
    // then the parameters
    StochasticNode<double> *div = new StochasticNode<double>( "diversification", new UniformDistribution( a, b) );
//    ExponentialDistributedRandomVariable *lambda = new ExponentialDistributedRandomVariable( "diversification", a );
    StochasticNode<double> *turnover = new StochasticNode<double>("turnover", new ExponentialDistribution(c) );
//    UniformDistributedRandomVariable *mu = new UniformDistributedRandomVariable("turnover", c, d);
    ConstantNode<double> *sampling = new ConstantNode<double>("rho", new double(1.0));
        
    div->setValue( new double(1.0) );
    turnover->setValue( new double(1.0) );
    TimeTree* t = trees[0];
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tauCBD = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess( div, turnover, met, mep, sampling, "uniform", "survival", int(t->getNumberOfTips()), t->getNames(), std::vector<Clade>()) );
    //    std::cout << "tau:\t" << tauCBD->getValue() << std::endl;
    
    // attach the data
    tauCBD->clamp( t->clone() );
    
    std::cout << tauCBD->getLnProbability() << std::endl;
        
    
    
    // then the parameters
    double MAX = trees[0]->getRoot().getAge();
    std::vector<double> r;
    r.push_back(2.0);
    ConstantNode<UnivariateFunction> *lambda = new ConstantNode<UnivariateFunction>("lambda(t)", new ConstantStepFunction( new ConstantNode<double>("", new double(0.0) ), r, 0, MAX ) );
    r[0] = 1.0;
    ConstantNode<UnivariateFunction> *mu = new ConstantNode<UnivariateFunction>("mu(t)", new ConstantStepFunction( new ConstantNode<double>("", new double(0.0) ), r, 0, MAX ) );
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0) );


    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new TimeDependentBirthDeathProcess(lambda, mu, rho, int(t->getNumberOfTips()), t->getNames(), std::vector<Clade>() ) );
//    std::cout << "tau:\t" << tau->getValue() << std::endl;
    // attach the data
    tau->clamp( t->clone() );
    
//    std::cout << "tau-clamped:\t" << tau->getValue() << std::endl;
    std::cout << tau->getLnProbability() << std::endl;
//    
//    
//    /* add the moves */
//    std::vector<const Move*> moves;
//    moves.push_back( new PathValueScalingMove( lambda, 1.0, 1.0, true ) );
//    moves.push_back( new PathValueScalingMove( mu, 1.0, 1.0, true ) );
//    moves.push_back( new CompletePathRescaleMove( lambda, 0.1, 1.0, true ) );
//    moves.push_back( new CompletePathRescaleMove( mu, 0.1, 1.0, true ) );
//    //    moves.push_back( new ScaleMove( sigma_lambda, 0.1, 1.0 ) );
//    //    moves.push_back( new ScaleMove( sigma_mu, 0.1, 1.0 ) );
//    
//    /* add the monitors */
//    std::vector<const Monitor*> monitors;
//    std::set<DagNode*> priors;
//    //    priors.insert( sigma_lambda );
//    //    priors.insert( sigma_mu );
////    monitors.push_back( new FileMonitor( priors, 1, "TimeVaryingBirthDeathTest.log", "\t" ) );
//    monitors.push_back( new PathSampleMonitor( lambda, 1, 0, MAX, BLOCKS, "LambdaRates.log", "\t" ) );
//    monitors.push_back( new PathSampleMonitor( mu, 1, 0, MAX, BLOCKS, "MuRates.log", "\t" ) );
//    
//    /* instantiate the model */
//    Model myModel = Model(lambda);
//    
//    /* instiate and run the MCMC */
//    Mcmc myMcmc = Mcmc( myModel, moves, monitors, 100 );
//    myMcmc.run(mcmcGenerations);
//    
//    
//    /* clean up */
//    delete mu;
//    delete lambda;
//    delete a;
//    delete b;
//    delete tau;
//    for (std::vector<const Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
//        const Move *theMove = *it;
//        delete theMove;
//    }
//    for (std::vector<const Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
//        const Monitor *theMonitor = *it;
//        delete theMonitor;
//    }
//    for (std::vector<TimeTree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
//        const TimeTree *theTree = *it;
//        delete theTree;
//    }
    
    return true;
}
