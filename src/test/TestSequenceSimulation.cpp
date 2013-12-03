#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "FastaWriter.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "NclReader.h"
#include "RateMatrix_JC.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "TestSequenceSimulation.h"
#include "TimeTree.h"

using namespace RevBayesCore;

TestSequenceSimulation::TestSequenceSimulation(const std::string &tfn) : treeFilename( tfn ) {
    
}

TestSequenceSimulation::~TestSequenceSimulation() {
    // nothing to do
}



bool TestSequenceSimulation::run( void ) {
    
    /* First, we read in the data */
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    
    /* set up the model graph */
    
//    std::vector<std::string> names = trees[0]->getTaxonNames();
    ConstantNode<TimeTree> *tree = new ConstantNode<TimeTree>( "tau", new TimeTree( *trees[0] ) );
            
    ConstantNode<RateMatrix> *q = new ConstantNode<RateMatrix>( "Q", new RateMatrix_JC(4) );
        
    ConstantNode<double> *clockRate = new ConstantNode<double>("clockRate", new double(1.0) );
    
    // and the character model
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tree, 4, true, 100);
    phyloCTMC->setClockRate( clockRate );
    phyloCTMC->setRateMatrix( q );
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("S", phyloCTMC );
    
    // write the simulated sequence
    FastaWriter writer;
    writer.writeData( "primatesSimulated.fas", charactermodel->getValue() );
    
    return true;
}
