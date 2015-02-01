//
//  MatrixRealSimpleMove.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "MatrixRealSymmetricSlideMove.h"
#include "DistributionWishart.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;


MatrixRealSymmetricSlideMove::MatrixRealSymmetricSlideMove(StochasticNode<MatrixReal > *v, double l, bool t, double w) : SimpleMove( v, w, t ),
    variable(v),
    lambda( l ),
    storedValue( 0.0 ),
    stored_i( 0 ),
    stored_j( 0 )
{
    
}


/** Clone object */
MatrixRealSymmetricSlideMove* MatrixRealSymmetricSlideMove::clone( void ) const
{
    
    return new MatrixRealSymmetricSlideMove( *this );
}



const std::string& MatrixRealSymmetricSlideMove::getMoveName( void ) const
{
    static std::string name = "MatrixRealSymmetricSlideMove";
    
    return name;
}


double MatrixRealSymmetricSlideMove::performSimpleMove( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    MatrixReal& mymat = variable->getValue();
    
    size_t dim = mymat.getDim();
    size_t indexa = size_t( rng->uniform01() * dim );
    size_t indexb = size_t( rng->uniform01() * dim );
    
    stored_i = indexa;
    stored_j = indexb;
    storedValue = mymat[stored_i][stored_j];

    double u = rng->uniform01();
    double m = lambda * (u - 0.5);
    mymat[indexa][indexb] += m;
    if ( indexa != indexb )
    {
        mymat[indexb][indexa] = mymat[indexa][indexb];
    }
    
    return 0;
}

void MatrixRealSymmetricSlideMove::acceptSimpleMove()
{

    variable->clearTouchedElementIndices();
    
}

void MatrixRealSymmetricSlideMove::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}


void MatrixRealSymmetricSlideMove::rejectSimpleMove( void )
{
 
    variable->getValue()[stored_i][stored_j] = storedValue;
    variable->getValue()[stored_j][stored_i] = storedValue;
    variable->clearTouchedElementIndices();
}

void MatrixRealSymmetricSlideMove::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MatrixReal >* >( newN );
}


void MatrixRealSymmetricSlideMove::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 )
    {
        lambda = size_t( round( lambda*(1.0 + ((rate-0.44)/0.56) ) ) );
    }
    else
    {
        lambda /= size_t( round( lambda/(2.0 - rate/0.44 ) ) );
    }
    
}

