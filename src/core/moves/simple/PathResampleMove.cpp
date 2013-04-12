/**
 * @file
 * This file contains the implementation of a path-resample move,
 * which resamples the path of a random walk.
 *
 * @brief Implementation of PathResampleMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-14, version 1.0
 *
 * $Id: SimplexMove.cpp 1535 2012-05-11 12:54:35Z hoehna $
 */

#include "DistributionNormal.h"
#include "StepFunction.h"
#include "PathResampleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

PathResampleMove::PathResampleMove(StochasticNode<UnivariateFunction> *v, int nb, double l, bool tuning, double weight) : SimpleMove( v, weight, tuning ), variable( v ), nBlocks( nb ), lambda( l ) {
    
    storedValue.resize( nBlocks );
}


/** Clone object */
PathResampleMove* PathResampleMove::clone( void ) const {
    
    return new PathResampleMove( *this );
}



double PathResampleMove::computeProposalProbability(const std::vector<double> &values, int begin, int end) {
    
    double prob = 0.0;
    if ( begin == end ) {
        
        double a = values[begin-1];
        double b = values[end+1];
        double expected = a + 0.5 * (b-a);
        double mid_val = values[begin];
        prob = RbStatistics::Normal::lnPdf(expected, lambda*2, mid_val);
    }
    else {
        int midpoint = begin + (end - begin) / 2;
        double a = values[begin-1];
        double b = values[end+1];
        double expected = a + (midpoint-begin+1.0)/(end-begin+2.0) * (b-a);
        double mid_val = values[midpoint];
        
        prob = RbStatistics::Normal::lnPdf(expected, lambda*(end-begin+2), mid_val);
        if (midpoint > begin) {
            prob += computeProposalProbability(values, begin, midpoint-1);
        }
        prob += computeProposalProbability(values, midpoint+1, end);
    }
    
    return prob;
}



const std::string& PathResampleMove::getMoveName( void ) const {
    static std::string name = "Path-Resample";
    
    return name;
}


/** Perform the move */
double PathResampleMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    storedValue.clear();
    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    
    // draw the index first
    int n = int(vals.size()) - nBlocks;
    index = (int)( n*rng->uniform01() ) + 1;
    
    copy ( vals.begin()+index, vals.begin()+nBlocks+index, storedValue.begin() );
	
    double backwardProb = computeProposalProbability(vals, index, index+nBlocks-1);
//    variable->setValue( newVal );
    propose(vals, index, index+nBlocks-1);

    double forwardProb = computeProposalProbability(vals, index, index+nBlocks-1);
    
    return backwardProb - forwardProb;
}


void PathResampleMove::propose(std::vector<double> &values, int begin, int end) {
    
    if ( begin == end ) {
        
        double a = values[begin-1];
        double b = values[end+1];
        double expected = a + 0.5 * (b-a);
        values[begin] = RbStatistics::Normal::rv(expected, lambda*2, *GLOBAL_RNG);
    }
    else {
        int midpoint = begin + (end - begin) / 2;
        double a = values[begin-1];
        double b = values[end+1];
        double expected = a + (midpoint-begin+1.0)/(end-begin+2.0) * (b-a);
        
        values[midpoint] = RbStatistics::Normal::rv(expected, lambda*(end-begin+2), *GLOBAL_RNG);
        if ( begin < midpoint) {
            propose(values, begin, midpoint-1);
        }
        propose(values, midpoint+1, end);
    }
}


void PathResampleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void PathResampleMove::rejectSimpleMove( void ) {
    StepFunction& f = static_cast<StepFunction &>( variable->getValue() );
    std::vector<double>& vals = f.getPathValues();
    copy ( storedValue.begin(), storedValue.begin()+nBlocks, vals.begin()+index );
}


void PathResampleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<UnivariateFunction>* >( newN );
}


void PathResampleMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        lambda *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        lambda /= (2.0 - rate/0.234 );
    }
}

