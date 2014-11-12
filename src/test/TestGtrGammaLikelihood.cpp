#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "GammaDistribution.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "GtrRateMatrixFunction.h"
#include "NclReader.h"
//#include "NucleotideBranchHeterogeneousCharEvoModel.h"
#include "NormalizeVectorFunction.h"
#include "QuantileFunction.h"
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "TestGtrGammaLikelihood.h"
#include "TimeTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVector.h"

#include <vector>

using namespace RevBayesCore;

TestGtrGammaLikelihood::TestGtrGammaLikelihood(const std::string &afn, const std::string &tFn) : alignmentFilename( afn ), treeFilename( tFn ) {
    
}

TestGtrGammaLikelihood::~TestGtrGammaLikelihood() {
    // nothing to do
}


bool TestGtrGammaLikelihood::run( void ) {
    
    /* First, we read in the data */
    // the matrix
    NclReader reader = NclReader();
    std::vector<AbstractCharacterData*> data = reader.readMatrices(alignmentFilename);
	
	AbstractDiscreteCharacterData *discrD = dynamic_cast<AbstractDiscreteCharacterData* >(data[0]);
    
	std::cout << "Read " << data.size() << " matrices." << std::endl;
    
    std::vector<TimeTree*> trees = NclReader().readTimeTrees( treeFilename );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    
    /* set up the model graph */
    
    //////////////////////
    // first the priors //
    //////////////////////
        
    // then the parameters
    ConstantNode<RbVector<double> > *pi = new ConstantNode<RbVector<double> >( "pi", new RbVector<double>(4, 1.0/4.0) );
    ConstantNode<RbVector<double> > *er = new ConstantNode<RbVector<double> >( "er", new RbVector<double>(6, 1.0/6.0) );
    
    //Rate heterogeneity
    ConstantNode<double> *alpha = new ConstantNode<double>("alpha", new double(0.5) );
    
    std::cout << "alpha:\t" << alpha->getValue() << std::endl;
    
    ConstantNode<double> *q1 = new ConstantNode<double>("q1", new double(0.125) );
    DeterministicNode<double> *q1_value = new DeterministicNode<double>("q1_value", new QuantileFunction(q1, new GammaDistribution(alpha, alpha) ) );
    ConstantNode<double> *q2 = new ConstantNode<double>("q2", new double(0.375) );
    DeterministicNode<double> *q2_value = new DeterministicNode<double>("q2_value", new QuantileFunction(q2, new GammaDistribution(alpha, alpha) ) );
    ConstantNode<double> *q3 = new ConstantNode<double>("q3", new double(0.625) );
    DeterministicNode<double> *q3_value = new DeterministicNode<double>("q3_value", new QuantileFunction(q3, new GammaDistribution(alpha, alpha) ) );
    ConstantNode<double> *q4 = new ConstantNode<double>("q4", new double(0.875) );
    DeterministicNode<double> *q4_value = new DeterministicNode<double>("q4_value", new QuantileFunction(q4, new GammaDistribution(alpha, alpha) ) );
    
//    ConstantNode<double> *q1_value = new ConstantNode<double>("q1_value", new double(1.0) );
//    ConstantNode<double> *q2_value = new ConstantNode<double>("q2_value", new double(1.0) );
//    ConstantNode<double> *q3_value = new ConstantNode<double>("q3_value", new double(1.0) );
//    ConstantNode<double> *q4_value = new ConstantNode<double>("q4_value", new double(1.0) );
    
    
    std::vector<const TypedDagNode<double>* > gamma_rates = std::vector<const TypedDagNode<double>* >();
    gamma_rates.push_back(q1_value);
    gamma_rates.push_back(q2_value);
    gamma_rates.push_back(q3_value);
    gamma_rates.push_back(q4_value);
    
    DeterministicNode<RbVector<double> > *site_rates = new DeterministicNode<RbVector<double> >( "site_rates", new VectorFunction<double>(gamma_rates) );
    ConstantNode<double> *sumNV = new ConstantNode<double>("sumnv", new double(1.0) );
//    ConstantNode<std::vector<double> > *site_rate_probs = new ConstantNode<std::vector<double> >( "site_rate_probs", new std::vector<double>(4,1.0/4.0) );
        
//    std::cout << "pi:\t" << pi->getValue() << std::endl;
//    std::cout << "er:\t" << er->getValue() << std::endl;
    std::cout << "rates:\t" << site_rates->getValue() << std::endl;
    
    DeterministicNode<RbVector<double> > *site_rates_norm = new DeterministicNode<RbVector<double> >( "site_rates_norm", new NormalizeVectorFunction(site_rates, sumNV) );
    std::cout << "rates:\t" << site_rates_norm->getValue() << std::endl;
    
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
    
    std::cout << "Q:\t" << q->getValue() << std::endl;
    
    ConstantNode<TimeTree> *tau = new ConstantNode<TimeTree>( "tau", new TimeTree( *trees[0] ) );
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    
    // and the character model
    size_t numChar = data[0]->getNumberOfCharacters();
//    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *charModel = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, numChar );
    PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree>(tau, true, numChar );
    charModel->setRateMatrix( q );
    charModel->setSiteRates( site_rates_norm );
//    charModel->setClockRate( clockRate );
    
    StochasticNode< AbstractDiscreteCharacterData > *charactermodel = new StochasticNode< AbstractDiscreteCharacterData >("S", charModel );
    charactermodel->clamp( discrD );
    
    std::cout << "BEAST LnL:\t\t\t\t" << -6281.4026 << std::endl;
    std::cout << "RevBayes LnL:\t\t" << charactermodel->getLnProbability() << std::endl;
    
    std::cout << "Finished GTR+Gamma model test." << std::endl;
    
    return true;
}
