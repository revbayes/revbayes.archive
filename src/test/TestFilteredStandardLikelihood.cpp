#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneous.h"
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
#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "GammaDistribution.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "GtrRateMatrixFunction.h"
#include "TimeTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVector.h"
#include "DiscretizeGammaFunction.h"

#include <vector>

//#define USE_GTR_RATE_MAT
//#define USE_TIME_TREE
//#define USE_NUCLEOTIDE
#define USE_3_STATES
//#define USE_RATE_HET

using namespace RevBayesCore;

TestFilteredStandardLikelihood::TestFilteredStandardLikelihood(const std::string &afn, const std::string &tFn) : alignmentFilename( afn ), treeFilename( tFn ) {
}

TestFilteredStandardLikelihood::~TestFilteredStandardLikelihood() {
}


bool TestFilteredStandardLikelihood::run( void ) {
    std::cerr << "  starting TestFilteredStandardLikelihood...\n" ;
    /* First, we read in the data */
    // the matrix
    NclReader reader = NclReader();
    std::vector<AbstractCharacterData*> data = reader.readMatrices(alignmentFilename);
    AbstractDiscreteCharacterData * discrD = dynamic_cast<AbstractDiscreteCharacterData *>(data[0]);
#   if defined(USE_TIME_TREE)
        typedef TimeTree TestTreeType;
        std::vector<TestTreeType*> trees = NclReader().readTimeTrees( treeFilename );
        ConstantNode<TestTreeType> *tau = new ConstantNode<TestTreeType>( "tau", new TestTreeType( *(trees)[0] ) );
#   else
        typedef BranchLengthTree TestTreeType;
        std::vector<TestTreeType*> *trees = NclReader().readBranchLengthTrees( treeFilename );
        ConstantNode<TestTreeType> *tau = new ConstantNode<TestTreeType>( "tau", new TestTreeType( *(*trees)[0] ) );
#   endif
    
    std::cout << "tau:\t" << tau->getValue() << std::endl;
#   if defined(USE_3_STATES)
        const size_t numStates = 3;
#   else
        const size_t numStates = 4;
#   endif
    size_t numChar = discrD->getNumberOfCharacters();

#   if defined(USE_RATE_HET)
        ConstantNode<double>* shape = new ConstantNode<double>("alpha", new double(0.5) );
        ConstantNode<double>* rate = new ConstantNode<double>("", new double(0.5) );
        ConstantNode<int>* numCats = new ConstantNode<int>("ncat", new int(4) );

        DiscretizeGammaFunction *dFunc = new DiscretizeGammaFunction( shape, rate, numCats, false );
        DeterministicNode<RbVector<double> > *site_rates_norm_2 = new DeterministicNode<RbVector<double> >( "site_rates_norm", dFunc );
        std::cout << "rates:\t" << site_rates_norm_2->getValue() << std::endl;
#   endif

#if defined(USE_3_STATES) && defined(USE_NUCLEOTIDE)
#error "cannot use 3 state and nucleotide type"
#endif
#if defined(USE_3_STATES) && defined(USE_GTR_RATE_MAT)
#error "cannot use 3 state and USE_GTR_RATE_MAT"
#endif
#   if defined(USE_GTR_RATE_MAT)
        ConstantNode<RbVector<double> > *pi = new ConstantNode<RbVector<double> >( "pi", new RbVector<double>(4, 1.0/4.0) );
        ConstantNode<RbVector<double> > *er = new ConstantNode<RbVector<double> >( "er", new RbVector<double>(6, 1.0/6.0) );
        DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
        std::cout << "Q:\t" << q->getValue() << std::endl;
#       if defined (USE_NUCLEOTIDE)
            PhyloCTMCSiteHomogeneousNucleotide<DnaState, TestTreeType> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<DnaState, TestTreeType>(tau, false, numChar);
#       else
            PhyloCTMCSiteHomogeneous<DnaState, TestTreeType> *charModel = new PhyloCTMCSiteHomogeneous<DnaState, TestTreeType>(tau, 4, false, numChar);
#       endif
#   else
        DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new JcRateMatrixFunction(numStates));
#       if defined (USE_NUCLEOTIDE)
            FilteredPhyloCTMCSiteHomogeneousNucleotide<StandardState, TestTreeType> *charModel = new FilteredPhyloCTMCSiteHomogeneousNucleotide<StandardState, TestTreeType>(tau, false, numChar, true);
#       else
            FilteredPhyloCTMCSiteHomogeneous<StandardState, TestTreeType> *charModel = new FilteredPhyloCTMCSiteHomogeneous<StandardState, TestTreeType>(tau, numStates, false, numChar, true);
#       endif
#   endif
#   if defined(USE_RATE_HET)
        charModel->setSiteRates( site_rates_norm_2 );
#   endif
    charModel->setRateMatrix( q );
    StochasticNode< AbstractDiscreteCharacterData > *charactermodel = new StochasticNode< AbstractDiscreteCharacterData >("S", charModel);
    charactermodel->clamp( discrD );
    double lnp = charactermodel->getLnProbability();
    std::cerr << "    lnProb = " << lnp << std::endl;
#   if defined(USE_3_STATES)
#       if defined(USE_RATE_HET)
            const double paupLnL = lnp; // can't check this against paup....
#       else
            /* no filtering based on variability...
            const double paupLnL = -813.23060; */
            const double paupLnL = -758.90694;
#       endif
#   else
#       if defined(USE_RATE_HET)
            const double paupLnL = -900.9122;
#       else
            const double paupLnL = -892.5822;
#       endif
#   endif
    const double tol = 0.01;
    if (fabs(lnp - paupLnL) > tol) {
        std::cerr << "    deviates too much from the likelihood from PAUP* of " << paupLnL << std::endl;
        return false;
    }
    if (lnp >= 0.0) {
        std::cerr << "    lnProb is too high!" << std::endl;
        return false;
    }

    std::cout << "RevBayes LnL:\t\t" << charactermodel->getLnProbability() << std::endl;
    std::cout << "Finished GTR+Gamma model test." << std::endl;
    return true;
}
