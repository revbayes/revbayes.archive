#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "JcRateMatrixFunction.h"
#include "NclReader.h"
#include "NormalizeVectorFunction.h"
#include "QuantileFunction.h" 
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "TestFilteredStandardLikelihood.h"
#include "BranchLengthTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

#include <vector>

using namespace RevBayesCore;

TestFilteredStandardLikelihood::TestFilteredStandardLikelihood(const std::string &afn, const std::string &tFn) : alignmentFilename( afn ), treeFilename( tFn ) {
}

TestFilteredStandardLikelihood::~TestFilteredStandardLikelihood() {
}


bool TestFilteredStandardLikelihood::run( void ) {
    /* First, we read in the data */
    // the matrix
    NclReader& reader = NclReader::getInstance();
    std::vector<AbstractCharacterData*> data = reader.readMatrices(alignmentFilename);
    AbstractDiscreteCharacterData * datum = dynamic_cast<AbstractDiscreteCharacterData *>(data[0]);
    std::vector<BranchLengthTree*> *trees = NclReader::getInstance().readBranchLengthTrees( treeFilename );
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new JcRateMatrixFunction(3) );
    ConstantNode<BranchLengthTree> *tau = new ConstantNode<BranchLengthTree>( "tau", new BranchLengthTree( *(*trees)[0] ) );
    // and the character model
    size_t numChar = data[0]->getNumberOfCharacters();
    FilteredPhyloCTMCSiteHomogeneous<StandardState, BranchLengthTree> *charModel = new FilteredPhyloCTMCSiteHomogeneous<StandardState, BranchLengthTree>(tau, 3, false, numChar );
    charModel->setRateMatrix( q );
    TypedDistribution<AbstractDiscreteCharacterData> *td = charModel;
    StochasticNode< AbstractDiscreteCharacterData > *charactermodel = new StochasticNode< AbstractDiscreteCharacterData >("S", td);
    charactermodel->clamp( datum );
    double lnp = charactermodel->getLnProbability();
    std::cout << "lnProb = " << lnp << std::endl;
    if (lnp >= 0.0) {
        std::cerr << "lnProb is too high!" << std::endl;
        return false;
    }
    return true;
}
