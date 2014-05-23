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

#ifndef DPPGibbsConcentrationMove_H
#define DPPGibbsConcentrationMove_H

#include <ostream>

#include "DirichletProcessPriorDistribution.h"
#include "SimpleMove.h"
#include "StochasticNode.h"
#include "DeterministicNode.h"
#include "TypedDagNode.h"
#include "Move.h"
#include "ConstantNode.h"


namespace RevBayesCore {
	
    template <class valueType>
    class DPPGibbsConcentrationMove : public Move {
		
    public:
        DPPGibbsConcentrationMove(StochasticNode<double> *n, DeterministicNode<int>* v, TypedDagNode< double >* gS, TypedDagNode< double >* gR, int ne, double w);                                                                      //!< Internal constructor
		
        // Basic utility functions
        DPPGibbsConcentrationMove<valueType>*					clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
		const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
		bool													isGibbs(void) const;
		
    protected:
        void													performGibbsMove(void);                                                            //!< Perform move
        void													acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double													performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void													rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
		
    private:
		
        StochasticNode<double>*									variable;
		DeterministicNode<int>*									numCats;
		TypedDagNode< double >*									gammaShape;
		TypedDagNode< double >*									gammaRate;
		int														numElem;
    };
}

#include "NormalizeVectorFunction.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "GammaDistribution.h"
#include "DistributionDirichlet.h"
#include "DistributionGamma.h"
#include "RbConstants.h"
#include "RbMathVector.h"


#include <cmath>

template <class valueType>
RevBayesCore::DPPGibbsConcentrationMove<valueType>::DPPGibbsConcentrationMove(StochasticNode<double> *n, DeterministicNode<int>* v, TypedDagNode< double >* gS, TypedDagNode< double >* gR, int ne, double w) : 
  Move( n, w, false ), variable( n ), numCats(v), gammaShape(gS), gammaRate(gR), numElem(ne) {

	nodes.insert(numCats);
	nodes.insert(gammaShape);
	nodes.insert(gammaRate);
		
}


/** Clone object */
template <class valueType>
RevBayesCore::DPPGibbsConcentrationMove<valueType>* RevBayesCore::DPPGibbsConcentrationMove<valueType>::clone( void ) const {
    
    return new DPPGibbsConcentrationMove<valueType>( *this );
}

template <class valueType>
const std::string& RevBayesCore::DPPGibbsConcentrationMove<valueType>::getMoveName( void ) const {
    static std::string name = "DPPGibbsConcentrationMove";
    
    return name;
}

template <class valueType>
bool RevBayesCore::DPPGibbsConcentrationMove<valueType>::isGibbs( void ) const {
    
    return true;
}


/** Perform the move */
template <class valueType>
void RevBayesCore::DPPGibbsConcentrationMove<valueType>::performGibbsMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	double& cpv = variable->getValue();
	
	numCats->update();
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

template <class valueType>
void RevBayesCore::DPPGibbsConcentrationMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
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

template <class valueType>
void RevBayesCore::DPPGibbsConcentrationMove<valueType>::acceptMove( void ) {
    
}

template <class valueType>
double RevBayesCore::DPPGibbsConcentrationMove<valueType>::performMove(double& probRatio) {
	return 0.0;
}

template <class valueType>
void RevBayesCore::DPPGibbsConcentrationMove<valueType>::rejectMove( void ) {
    
}



#endif

