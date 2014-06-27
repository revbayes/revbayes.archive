#include "BranchLengthTree.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
//#include "Func_readTreeTrace.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NearestNeighborInterchange.h"
#include "NewickConverter.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TestNNI.h"
#include "TreeTrace.h"
#include "Tree.h"
#include "TreeSummary.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestNNI::TestNNI(int gen) : mcmcGenerations( gen ){
    
}

TestNNI::~TestNNI() {
    // nothing to do
}


TreeTrace<TimeTree> TestNNI::readTreeTrace(const std::string &fname) {
//    RevLanguage::Func_readTreeTrace reader;
//    std::vector<RevLanguage::Argument> args;
//    RevLanguage::RbPtr<RevLanguage::Variable> var = new RevLanguage::Variable(new RevLanguage::RlString(fname) );
//    args.push_back( RevLanguage::Argument(var,"filename") );
//    RevLanguage::RbPtr<RevLanguage::Variable> var2 = new RevLanguage::Variable(new RevLanguage::RlString("clock") );
//    args.push_back( RevLanguage::Argument(var2,"treetype") );
//    reader.processArguments(args);
//    
//    RevLanguage::RevObject* trace = reader.execute();
//    
//    TreeTrace<TimeTree> rv = static_cast<RevLanguage::TreeTrace<RevLanguage::TimeTree>* >( trace )->getValue();
//    
//    delete trace;
//    
//    return rv;

    return TreeTrace<TimeTree>();   // \TODO Sebastian: This is a temporary fix -- Fredrik
}


bool TestNNI::run( void ) {
    
    /* set up the model graph */
    
    // first the priors
    ConstantNode<double> *div = new ConstantNode<double>("diversification", new double(1.0) );
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0) );
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    
    
    // then the parameters
    std::vector<std::string> names;
    names.push_back("A");
    names.push_back("B");
    names.push_back("C");
    names.push_back("D");
    
    
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( 2.0 ) );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", 4, names, std::vector<Clade>()) );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    
    /* add the moves */
    std::vector<Move*> moves;
    moves.push_back( new NearestNeighborInterchange( tau, 1.0 ) );
    
    /* add the monitors */
    std::vector<Monitor*> monitors;
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( tau );
    monitors.push_back( new FileMonitor( monitoredNodes, 1, "NNITest.tree", "\t" ) );
    
    /* instantiate the model */
    Model myModel = Model(div);
    
    /* instiate and run the MCMC */
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations);
    
    
    // read in the tree trace
    TreeTrace<TimeTree> trace = readTreeTrace("NNITest.tree");
    
    TreeSummary<TimeTree> summary = TreeSummary<TimeTree>(trace);
    summary.summarize();
    summary.printTreeSummary(std::cerr);
    
    //    /* clean up */
    //    for (size_t i = 0; i < 10; ++i) {
    //        delete x[i];
    //    }
    //    delete [] x;
    //    delete mu;
    //    delete sigma;
    //    delete a;
    //    delete b;
    //    delete c;
    //    for (std::vector<const Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
    //        const Move *theMove = *it;
    //        delete theMove;
    //    }
    //    for (std::vector<const Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
    //        const Monitor *theMonitor = *it;
    //        delete theMonitor;
    //    }
    
    return true;
}
