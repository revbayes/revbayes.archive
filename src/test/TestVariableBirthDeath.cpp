
#include "TimeDependentBirthDeathProcess.h"
#include "BrownianMotion.h"
#include "Clade.h"
#include "CompletePathRescaleMove.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ConstantFunction.h"
#include "ConstantNode.h"
#include "ConstantStepFunction.h"
#include "GeometricBrownianMotion.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "LinearStepFunction.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "OneOverXDistribution.h"
#include "PathValueScalingMove.h"
#include "PathResampleMove.h"
#include "PathSampleMonitor.h"
#include "RbFileManager.h"
#include "ScaleMove.h"
#include "Skyline.h"
#include "SlidingMove.h"
#include "StepFunction.h"
#include "TestVariableBirthDeath.h"
#include "TimeTree.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestVariableBirthDeath::TestVariableBirthDeath(const std::string &tfn, int gen) : treeFilename( tfn ), mcmcGenerations( gen ){
    
}

TestVariableBirthDeath::~TestVariableBirthDeath() {
    // nothing to do
}


bool TestVariableBirthDeath::run( void ) {
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    /* set up the model graph */
    
    // first the priors
    ConstantNode<double> *a = new ConstantNode<double>("a", new double(1.0) );
    ConstantNode<double> *b = new ConstantNode<double>("b", new double(0.1) );
    double MAX = trees[0]->getRoot().getAge();
    ConstantNode<double> *c = new ConstantNode<double>("c", new double(MAX) );
    StochasticNode<double> *sigma_lambda = new StochasticNode<double>("sigma_lambda", new ExponentialDistribution( a ) );
    StochasticNode<double> *sigma_mu = new StochasticNode<double>("sigma_mu", new ExponentialDistribution( a ) );
    StochasticNode<double> *start_lambda = new StochasticNode<double>("start_lambda", new ExponentialDistribution( b ) );
    StochasticNode<double> *start_mu = new StochasticNode<double>("start_mu", new ExponentialDistribution( b ) );
    
    
    // then the parameters
    int BLOCKS = 20;
    StochasticNode<UnivariateFunction> *lambda = new StochasticNode<UnivariateFunction>( "speciation", new GeometricBrownianMotion(start_lambda, sigma_lambda, c, BLOCKS) );
    StochasticNode<UnivariateFunction> *mu = new StochasticNode<UnivariateFunction>("extinction", new GeometricBrownianMotion(start_mu, sigma_mu, c, BLOCKS) );    
    
//    std::vector<double> l;
//    l.push_back(2.0);
//    ConstantNode<UnivariateFunction> *lambda = new ConstantNode<UnivariateFunction>("lambda(t)", new ConstantStepFunction( new ConstantNode<double>("", new double(0.0) ), l, 0, MAX ) );
//    std::vector<double> m;
//    m.push_back(1.0);
//    ConstantNode<UnivariateFunction> *mu = new ConstantNode<UnivariateFunction>("mu(t)", new ConstantStepFunction( new ConstantNode<double>("", new double(0.0) ), m, 0, MAX ) );
    
    
    std::vector<double> r;
    r.push_back(1.0);
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0) );
    
    TimeTree* t = trees[0];
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new TimeDependentBirthDeathProcess(lambda, mu, rho, int(t->getNumberOfTips()), t->getNames(), std::vector<Clade>()) );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // attach the data
    tau->clamp( t->clone() );
    
    std::cout << "tau-clamped:\t" << tau->getValue() << std::endl;
    
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new PathValueScalingMove( lambda, 1.0, true, 10.0 ) );
    moves.push_back( new PathValueScalingMove( mu, 1.0, true, 10.0 ) );
    moves.push_back( new PathValueScalingMove( lambda, 3.0, true, 5.0 ) );
    moves.push_back( new PathValueScalingMove( mu, 3.0, true, 5.0 ) );
    moves.push_back( new PathValueScalingMove( lambda, 5.0, true, 5.0 ) );
    moves.push_back( new PathValueScalingMove( mu, 5.0, true, 5.0 ) );
//    moves.push_back( new PathResampleMove( lambda, 3, 0.1, true, 5.0, true ) );
//    moves.push_back( new PathResampleMove( mu, 3, 0.1, true, 5.0, true ) );
    moves.push_back( new ScaleMove( sigma_lambda, 0.1, true, 1.0 ) );
    moves.push_back( new ScaleMove( sigma_mu, 0.1, true, 1.0 ) );
    moves.push_back( new ScaleMove( start_lambda, 0.1, true, 1.0 ) );
    moves.push_back( new ScaleMove( start_mu, 0.1, true, 1.0 ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> priors;
    priors.insert( sigma_lambda );
    priors.insert( sigma_mu );
    priors.insert( start_lambda );
    priors.insert( start_mu );
    monitors.push_back( new FileMonitor( priors, 1, "TimeDependentBirthDeathTest.log", "\t" ) );
    monitors.push_back( new PathSampleMonitor( lambda, 1, 0, MAX*(1.0 + 0.9/BLOCKS), BLOCKS, "LambdaRates.log", "\t" ) );
    monitors.push_back( new PathSampleMonitor( mu, 1, 0, MAX*(1.0 + 0.9/BLOCKS), BLOCKS, "MuRates.log", "\t" ) );
    
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
    delete tau;
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        Monitor *theMonitor = *it;
        delete theMonitor;
    }
    for (std::vector<TimeTree*>::iterator it = trees.begin(); it != trees.end(); ++it) {
        const TimeTree *theTree = *it;
        delete theTree;
    }
    
    return true;
}
