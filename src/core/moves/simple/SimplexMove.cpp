/**
 * @file
 * This file contains the implementation of SimplexMove,
 * which changes a simplex.
 *
 * @brief Implementation of SimplexMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: SimplexMove.cpp 1535 2012-05-11 12:54:35Z hoehna $
 */

#include "DistributionDirichlet.h"
#include "SimplexMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>
#include <ostream>

using namespace RevBayesCore;

SimplexMove::SimplexMove(StochasticNode< RbVector<double> > *v, double a, size_t nc, double o, bool t, double w, double k /*=0.0*/) : SimpleMove( v, w, t ),
    variable( v ),
    alpha( a ),
    nCategories( nc ),
    offset( o ),
    kappa( k )
{
    
}


/** Clone object */
SimplexMove* SimplexMove::clone( void ) const {
    
    return new SimplexMove( *this );
}



const std::string& SimplexMove::getMoveName( void ) const {
    static std::string name = "Simplex-Redraw";
    
    return name;
}



/** Perform the move */
double SimplexMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    storedValue = variable->getValue();
    
	const std::vector<double>& curVal = variable->getValue();
	std::vector<double> newVal = curVal;
    size_t              n      = curVal.size();
    
	/* We update the simplex values by proposing new values from a Dirichlet centered
     on the current values. The i-th parameter of the Dirichlet is the i-th value
     in the simplex multiplied by a parameter (alpha0, AKA tuning) that controls the
     variance of the Dirichlet. We implement two cases of this general move. In one
     case, all of the elements of the simplex are targeted for update (n == k). In the
     other, more complicated, case a subset of the elements of the simplex are updated
     (k < n). Here, we construct a smaller simplex with k+1 elements. The first k of the
     elements are the values from the full simplex that were targeted for update. The last
     element of the smaller simplex accumulates the probabilities of all of the simplex
     values in the full simplex that were not targeted for update. We then update the
     small simplex by centering a Dirichlet on the small simplex. The values for those elements
     in the full simplex that were not targeted for update are all changed proportionally.
     This means that we need to calculate the Jacobian for the Hastings ratio in this case. */
	double lnProposalRatio = 0.0;
	if ( nCategories > n ) {
		// we can't update more elements than there are elements in the simplex
		throw RbException( "Attempting to update too many simplex variables" );
    }
	else if ( nCategories < 1 ) {
		// we also can't update 0 or a negative number of elements
		throw RbException( "Attempting to update too few simplex variables" );
    }
	else if ( nCategories < n ) {
		// we update a subset of the elements in the full simplex
		// pick k values at random, producing a map from the index in the full vector (curVal) to
		// the index in the reduced vector (x, alpha, and z)
		std::vector<size_t> indicesToUpdate;
		std::vector<size_t> tmpV;
		for (size_t i=0; i<n; i++)
			tmpV.push_back(i);
		RbStatistics::Helper::randomlySelectFromVectorWithoutReplacement<size_t>(tmpV, indicesToUpdate, nCategories, *rng);
		std::map<size_t,size_t> mapper;
		for (size_t i=0; i<indicesToUpdate.size(); i++)
			mapper.insert( std::make_pair(indicesToUpdate[i], i) );
        
		// set up the vectors
		std::vector<double> x(indicesToUpdate.size()+1, 0.0);
		std::vector<double> kappaV(indicesToUpdate.size()+1, 0.0);
		std::vector<double> alphaForward(indicesToUpdate.size()+1, 0.0);
		std::vector<double> alphaReverse(indicesToUpdate.size()+1, 0.0);
		std::vector<double> z(indicesToUpdate.size()+1, 0.0);
		for (size_t i=0; i<n; i++) {
			std::map<size_t,size_t>::iterator it = mapper.find(i);
			if (it != mapper.end()){
				x[it->second] += curVal[it->first];
				kappaV[it->second] += kappa;
			}
			else {
				x[x.size()-1] += curVal[i];
				kappaV[kappaV.size()-1] += kappa;
			}
       }
		for (size_t i=0; i<x.size(); i++)
            alphaForward[i] = (x[i]+offset) * alpha + kappaV[i];
        
		// draw a new value for the reduced vector
		z = RbStatistics::Dirichlet::rv( alphaForward, *rng );
		
		// fill in the Dirichlet parameters for the reverse probability calculations
		for (size_t i=0; i<z.size(); i++)
			alphaReverse[i] = (z[i]+offset) * alpha + kappaV[i];
		
		// fill in the full vector
		double factor = z[z.size()-1] / x[x.size()-1];
		for (size_t i=0; i<n; i++) {
			std::map<size_t,size_t>::iterator it = mapper.find(i);
			if (it != mapper.end())
				newVal[i] = z[it->second];
			else 
				newVal[i] = curVal[i] * factor;
            
            // test for 0-values
            if ( newVal[i] < 1E-100) {
                return RbConstants::Double::neginf;
            }
        }
        
		// Hastings ratio
		lnProposalRatio  = RbStatistics::Dirichlet::lnPdf(alphaReverse, x) - RbStatistics::Dirichlet::lnPdf(alphaForward, z); // Hastings Ratio
		lnProposalRatio += (n - nCategories) * log(factor); // Jacobian
        
    }
	else {
		// we update all of the elements in the vector
		// first, we get the parameters of the Dirichlet for the forward move
		std::vector<double> alphaForward(curVal.size());
		for (size_t i=0; i<curVal.size(); i++)
        {
			alphaForward[i] = (curVal[i]+offset) * alpha + kappa;
            // we need to check for 0 values
            if (alphaForward[i] < 1E-100) {
                // very low proposal probability which will hopefully result into a rejected proposal
                return RbConstants::Double::neginf;
            }
        }
        
		// then, we propose new values
		newVal = RbStatistics::Dirichlet::rv( alphaForward, *rng );
		
		// and calculate the Dirichlet parameters for the (imagined) reverse move
		std::vector<double> alphaReverse(newVal.size());
        for (size_t i=0; i<curVal.size(); i++) {
			alphaReverse[i] = (newVal[i]+offset) * alpha + kappa;
            // we need to check for 0 values
            if (alphaReverse[i] < 1E-100) {
                // very low proposal probability which will hopefully result into a rejected proposal
                return RbConstants::Double::neginf;
            }
        }
		
		// finally, we calculate the log of the Hastings ratio
		lnProposalRatio = RbStatistics::Dirichlet::lnPdf(alphaReverse, curVal) - RbStatistics::Dirichlet::lnPdf(alphaForward, newVal);
    }
    
    variable->setValue( new RbVector<double>(newVal), false );
    
    return lnProposalRatio;
}


void SimplexMove::printParameterSummary(std::ostream &o) const {
    o << "alpha = " << alpha;
}


void SimplexMove::rejectSimpleMove( void ) {
    // swap current value and stored value
    variable->setValue( new RbVector<double>(storedValue), false );
}


void SimplexMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
}


void SimplexMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        alpha /= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        alpha *= (2.0 - rate/0.234 );
    }
}

