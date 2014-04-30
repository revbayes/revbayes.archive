/**
 * @file
 * This file contains the declaration of DPPGibbsConcentrationMove, which performs the DPP move based on Neal (2000) Algorithm 8
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

namespace RevBayesCore {
	
    template <class valueType>
    class DPPGibbsConcentrationMove : public Move {
		
    public:
        DPPGibbsConcentrationMove(StochasticNode<double> *n, const StochasticNode< std::vector<valueType> >* v, int na, double w);                                                                      //!< Internal constructor
		
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
		
        StochasticNode<double>*									concentration;
		const StochasticNode< std::vector<valueType> >*			dppVariables;
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
RevBayesCore::DPPGibbsConcentrationMove<valueType>::DPPGibbsConcentrationMove(StochasticNode<double> *n, const StochasticNode< std::vector<valueType> >* v, int na, double w) : 
  Move( n, w, false ), concentration( n ), dppVariables(v) {
    
	
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
	double& cpv = concentration->getValue();
	
	const DirichletProcessPriorDistribution<valueType>& dppDist = static_cast<const DirichletProcessPriorDistribution<valueType> &>( dppVariables->getDistribution() );
	
	// This move is a fully conditional Gibbs move, with a Gamma distribution
	// Escobar MD, West M. Bayesian density estimation and inference using mixtures. J Am Stat Assoc. 1995;90:577-588.
	GammaDistribution& dist = static_cast<GammaDistribution &>( concentration->getDistribution() );
	
	// TAH: I added these functions to the Gamma dist. Probably a better way to do this.
	double sh = dist.getShapeValue();
	double rt = dist.getRateValue();
	
//	const std::set<const DagNode*>& gammaPs = dist.getParameters();
//	std::vector<double> gPs;
//	
//    for (std::set<const DagNode* >::const_iterator it = gammaPs.begin(); it != gammaPs.end(); ++it) {
//		
//    }
	
	int k = dppDist.getNumberOfCategories();
	int nV = dppDist.getNumberOfElements();
	
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
		newAlpha = RbStatistics::Gamma::rv(sh + k -1.0, rt - log(eta), *GLOBAL_RNG);
		
	cpv = newAlpha;
//	std::cout << oldConc << " --> " << cpv << std::endl;
}

template <class valueType>
void RevBayesCore::DPPGibbsConcentrationMove<valueType>::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    concentration = static_cast< StochasticNode<double>* >( newN );
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

