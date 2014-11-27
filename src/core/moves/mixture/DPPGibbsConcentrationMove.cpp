/**
 * @file
 * This file contains the declaration of DPPGibbsConcentrationMove, which performs a fully conditional Gibbs move on the concentration parameter of the DPP
 *   this move is conditional on the # of categories in the current state of the DPP
 *
 * @brief Declaration of DPPGibbsConcentrationMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: DPPGibbsConcentrationMove.h $
 */


#include <ostream>
#include <cmath>

#include "DPPGibbsConcentrationMove.h"
#include "DirichletProcessPriorDistribution.h"
#include "SimpleMove.h"
#include "StochasticNode.h"
#include "DeterministicNode.h"
#include "TypedDagNode.h"
#include "MoveOld.h"
#include "ConstantNode.h"
#include "NormalizeVectorFunction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "GammaDistribution.h"
#include "DistributionDirichlet.h"
#include "DistributionGamma.h"
#include "RbConstants.h"
#include "RbMathVector.h"



RevBayesCore::DPPGibbsConcentrationMove::DPPGibbsConcentrationMove(StochasticNode<double> *n, DeterministicNode<int>* v, TypedDagNode< double >* gS, TypedDagNode< double >* gR, int ne, double w) : 
  MoveOld( n, w, false ), variable( n ), numCats(v), gammaShape(gS), gammaRate(gR), numElem(ne) {

	nodes.insert(numCats);
	nodes.insert(gammaShape);
	nodes.insert(gammaRate);
		
}


/** Clone object */
RevBayesCore::DPPGibbsConcentrationMove* RevBayesCore::DPPGibbsConcentrationMove::clone( void ) const {
    
    return new DPPGibbsConcentrationMove( *this );
}

const std::string& RevBayesCore::DPPGibbsConcentrationMove::getMoveName( void ) const {
    static std::string name = "DPPGibbsConcentrationMove";
    
    return name;
}

bool RevBayesCore::DPPGibbsConcentrationMove::isGibbs( void ) const {
    
    return true;
}


/** Perform the move */
void RevBayesCore::DPPGibbsConcentrationMove::performGibbsMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	double& cpv = variable->getValue();
	
	int k = numCats->getValue();
	int nV = numElem;
	
	// This move is a fully conditional Gibbs move, with a Gamma distribution
	// Escobar MD, West M. Bayesian density estimation and inference using mixtures. J Am Stat Assoc. 1995;90:577-588.
	
	double sh = gammaShape->getValue();
	double rt = gammaRate->getValue();
		
	double oldConc = cpv;
	
	std::vector<double> z(2);
	
	z[0] = oldConc + 1.0;
	z[1] = (double)nV;

	std::vector<double> f = RbStatistics::Dirichlet::rv(z, *GLOBAL_RNG);
	double eta = f[0];

	double u = rng->uniform01();
	double x = ( sh + (double)k - 1.0 ) / ( (double)nV * (rt - log(eta)) );
	double newAlpha;
	if((u / (1.0 - u)) < x)
		newAlpha = RbStatistics::Gamma::rv(sh + k, rt - log(eta), *GLOBAL_RNG);
	else
		newAlpha = RbStatistics::Gamma::rv(sh + k - 1.0, rt - log(eta), *GLOBAL_RNG);
		
	cpv = newAlpha;
	variable->touch();
	variable->keep();
}

void RevBayesCore::DPPGibbsConcentrationMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    if (oldN == variable){
		variable = static_cast< StochasticNode<double>* >( newN );
	}
	else if (oldN == numCats){
        numCats = static_cast<DeterministicNode<int>* >(newN);
    }
	else if (oldN == gammaShape){
        gammaShape = static_cast<TypedDagNode<double>* >(newN);
    }
	else if (oldN == gammaRate){
        gammaRate = static_cast<TypedDagNode<double>* >(newN);
    }

}

void RevBayesCore::DPPGibbsConcentrationMove::acceptMove( void ) {
    
}

double RevBayesCore::DPPGibbsConcentrationMove::performMove(double& probRatio) {
	return 0.0;
}

void RevBayesCore::DPPGibbsConcentrationMove::rejectMove( void ) {
    
}

