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

#include "DistributionBeta.h"
#include "DistributionDirichlet.h"
#include "SimplexSingleElementScale.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

SimplexSingleElementScale::SimplexSingleElementScale(StochasticNode< RbVector<double> > *v, double a, bool t, double weight) : SimpleMove( v, weight, t ),
    variable( v ),
    alpha( a )
{
    
}


/** Clone object */
SimplexSingleElementScale* SimplexSingleElementScale::clone( void ) const {
    
    return new SimplexSingleElementScale( *this );
}



const std::string& SimplexSingleElementScale::getMoveName( void ) const {
    static std::string name = "SimplexSingleElementScale";
    
    return name;
}



/** Perform the move */
double SimplexSingleElementScale::performSimpleMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // get the current value
    std::vector<double>& value = variable->getValue();
    
    // store the value
    storedValue = value;
    
    // we need to know the number of categories
    size_t cats = value.size();
    
    // randomly draw a new index
    size_t chosenIndex = size_t( floor(rng->uniform01()*double(cats)) );
    double currentValue = value[chosenIndex];
    
    // draw new rates and compute the hastings ratio at the same time
    double a = alpha * currentValue + 1.0;
    double b = alpha * (1.0-currentValue) + 1.0;
    double new_value = RbStatistics::Beta::rv(a, b, *rng);
    
    // set the value
    value[chosenIndex] = new_value;
    
    // rescale
    double sum = 0;
	for(size_t i=0; i<cats; i++) {
		sum += value[i];
    }
	for(size_t i=0; i<cats; i++)
		value[i] /= sum;
    
    // compute the Hastings ratio
    double forward = RbStatistics::Beta::lnPdf(a, b, new_value);
    double new_a = alpha * value[chosenIndex] + 1.0;
    double new_b = alpha * (1.0-value[chosenIndex]) + 1.0;
    double backward = RbStatistics::Beta::lnPdf(new_a, new_b, currentValue);
    
    return backward - forward;
}


void SimplexSingleElementScale::printParameterSummary(std::ostream &o) const {
    o << "alpha = " << alpha;
}


void SimplexSingleElementScale::rejectSimpleMove( void ) {
    // swap current value and stored value

    variable->setValue( new RbVector<double>(storedValue), false );
}


void SimplexSingleElementScale::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
}


void SimplexSingleElementScale::tune( void )
{

    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 )
    {
        alpha /= (1.0 + ((rate-0.234)/0.766) );
    }
    else
    {
        alpha *= (2.0 - rate/0.234 );
    }
}

