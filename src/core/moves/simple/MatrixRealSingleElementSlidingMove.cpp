/* 
 * File:   MatrixRealSingleElementSlidingMove.cpp
 * Author: nl
 * 
 * Created on 13 juillet 2014, 17:55
 */


#include "MatrixRealSingleElementSlidingMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


MatrixRealSingleElementSlidingMove::MatrixRealSingleElementSlidingMove(StochasticNode<MatrixReal> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ) {
    
}


/** Clone object */
MatrixRealSingleElementSlidingMove* MatrixRealSingleElementSlidingMove::clone( void ) const
{
    
    return new MatrixRealSingleElementSlidingMove( *this );
}



const std::string& MatrixRealSingleElementSlidingMove::getMoveName( void ) const
{
    static std::string name = "MatrixRealSingleElementSlidingMove";
    
    return name;
}


double MatrixRealSingleElementSlidingMove::performSimpleMove( void )
{
        
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    MatrixReal& v = variable->getValue();
    // choose an index
    indexa = size_t( rng->uniform01() * v.getNumberOfRows() );
    indexb = size_t( rng->uniform01() * v.getNumberOfColumns() );
    
    // copy value
    storedValue = v[indexa][indexb];
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );
    v[indexa][indexb] += slidingFactor;
    
    variable->addTouchedElementIndex(indexa);
    variable->addTouchedElementIndex(indexb);

    // compute the Hastings ratio
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void MatrixRealSingleElementSlidingMove::acceptSimpleMove()
{
    variable->clearTouchedElementIndices();
}

void MatrixRealSingleElementSlidingMove::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}


void MatrixRealSingleElementSlidingMove::rejectSimpleMove( void )
{
    
    MatrixReal& v = variable->getValue();
    v[indexa][indexb] = storedValue;
    variable->clearTouchedElementIndices();
    
}

void MatrixRealSingleElementSlidingMove::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MatrixReal >* >( newN );
    
}


void MatrixRealSingleElementSlidingMove::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
}
