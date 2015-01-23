/**
 * @file
 * This file contains the implementation of BetaSimplexMove,
 * which changes a simplex.
 *
 * @brief Implementation of BetaSimplexMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: BetaSimplexMove.cpp 1535 2012-05-11 12:54:35Z hoehna $
 */

#include "DistributionBeta.h"
#include "BetaSimplexMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

BetaSimplexMove::BetaSimplexMove(StochasticNode<double > *v, double a, bool t, double w) : SimpleMove( v, w, t ), variable( v ), alpha( a ) {
    
}


/** Clone object */
BetaSimplexMove* BetaSimplexMove::clone( void ) const {
    
    return new BetaSimplexMove( *this );
}



const std::string& BetaSimplexMove::getMoveName( void ) const {
    static std::string name = "BetaSimplex-Redraw";
    
    return name;
}



/** Perform the move */
double BetaSimplexMove::performSimpleMove( void )
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    storedValue = variable->getValue();
    
	const double& curVal = variable->getValue();
    
	/* We update the simplex values by proposing new values from a Beta distribution centered
     on the current values. */
	double lnProposalRatio = 0.0;
    
    // first, we get the parameters of the Beta for the forward move
    double alphaForward = curVal * alpha;
    
    // then, we propose new values
    double newVal = RbStatistics::Beta::rv( alphaForward, alphaForward, *rng );
    
    // and calculate the Dirichlet parameters for the (imagined) reverse move
    double alphaReverse = newVal * alpha;
    if (alphaForward < 1E-10)
    {
        // very low proposal probability which will hopefully result into a rejected proposal
        return -1E10;
    }
    
    // finally, we calculate the log of the Hastings ratio
    lnProposalRatio = RbStatistics::Beta::lnPdf(alphaReverse, alphaReverse, curVal) - RbStatistics::Beta::lnPdf(alphaForward, alphaForward, newVal);
    
    variable->setValue( new double(newVal) );
    
    return lnProposalRatio;
}


void BetaSimplexMove::printParameterSummary(std::ostream &o) const {
    o << "alpha = " << alpha;
}


void BetaSimplexMove::rejectSimpleMove( void ) {
    // swap current value and stored value
    variable->setValue( new double(storedValue) );
}


void BetaSimplexMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<double>* >( newN );
}


void BetaSimplexMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        alpha /= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        alpha *= (2.0 - rate/0.234 );
    }
}

