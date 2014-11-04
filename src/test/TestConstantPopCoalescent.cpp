
#include "Clade.h"
#include "ConstantPopulationCoalescent.h"
#include "ConstantNode.h"
#include "FileMonitor.h"
#include "NclReader.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "TestConstantPopCoalescent.h"
#include "TimeTree.h"
#include "UniformDistribution.h"

using namespace RevBayesCore;

TestConstantPopCoalescent::TestConstantPopCoalescent(const std::string &tfn) : treeFilename( tfn ) {
    
}

TestConstantPopCoalescent::~TestConstantPopCoalescent() {
    // nothing to do
}


bool TestConstantPopCoalescent::run( void ) {
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    TimeTree *t = trees[0];
    //    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* testing the likelihood implementation */
    // first the priors
    ConstantNode<double> *Ne = new ConstantNode<double>("N", new double(10000.0) );

    StochasticNode<TimeTree> *tauCPC = new StochasticNode<TimeTree>( "tau", new ConstantPopulationCoalescent( Ne, int(t->getNumberOfTips()), t->getTipNames(), std::vector<Clade>()) );
    //    std::cout << "tau:\t" << tauCBD->getValue() << std::endl;
    
    
    std::cerr << tauCPC->getValue() << std::endl;
    std::cout << tauCPC->getValue().getRoot().getAge() << std::endl;
    
    
    return true;
}
