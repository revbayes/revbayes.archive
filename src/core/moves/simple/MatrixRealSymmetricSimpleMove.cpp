//
//  MatrixRealSimpleMove.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "MatrixRealSymmetricSimpleMove.h"
#include "DistributionWishart.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;


MatrixRealSymmetricMove::MatrixRealSymmetricMove(StochasticNode<MatrixReal > *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ), storedValue(v->getValue()) {
    
}


/** Clone object */
MatrixRealSymmetricMove* MatrixRealSymmetricMove::clone( void ) const {
    
    return new MatrixRealSymmetricMove( *this );
}



const std::string& MatrixRealSymmetricMove::getMoveName( void ) const
{
    static std::string name = "MatrixRealSymmetricMove";
    
    return name;
}


double MatrixRealSymmetricMove::performSimpleMove( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    MatrixReal& mymat = variable->getValue();
    storedValue = mymat;
    
    size_t dim = mymat.getDim();
    size_t indexa = size_t( rng->uniform01() * dim );
    size_t indexb = size_t( rng->uniform01() * dim );

    double u = rng->uniform01();
    double m = lambda * (u - 0.5);
    mymat[indexa][indexb] += m;
    mymat[indexb][indexa] = mymat[indexa][indexb];
        
    return 0;   
}

void MatrixRealSymmetricMove::acceptSimpleMove()
{

    variable->clearTouchedElementIndices();
    
}

void MatrixRealSymmetricMove::printParameterSummary(std::ostream &o) const
{
    o << "lambda = " << lambda;
}


void MatrixRealSymmetricMove::rejectSimpleMove( void )
{
 
    variable->getValue() = storedValue;
    variable->clearTouchedElementIndices();
}

void MatrixRealSymmetricMove::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MatrixReal >* >( newN );
}


void MatrixRealSymmetricMove::tune( void )
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

