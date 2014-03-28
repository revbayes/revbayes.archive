//
//  PrecisionMatrixSimpleMove.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "PrecisionMatrixSimpleMove.h"
#include "DistributionWishart.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;


PrecisionMatrixMove::PrecisionMatrixMove(StochasticNode<PrecisionMatrix > *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ), storedValue(v->getValue()) {
    
}


/** Clone object */
PrecisionMatrixMove* PrecisionMatrixMove::clone( void ) const {
    
    return new PrecisionMatrixMove( *this );
}



const std::string& PrecisionMatrixMove::getMoveName( void ) const {
    static std::string name = "PrecisionMatrixMove";
    
    return name;
}


double PrecisionMatrixMove::performSimpleMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    int df = (int) lambda;
    
    PrecisionMatrix& mymat = variable->getValue();
    
    PrecisionMatrix newmat = RbStatistics::Wishart::rv(mymat,df,*rng);

    storedValue = mymat;
    
    mymat = newmat;
    
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void PrecisionMatrixMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void PrecisionMatrixMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void PrecisionMatrixMove::rejectSimpleMove( void ) {
    
    variable->getValue() = storedValue;
    variable->clearTouchedElementIndices();
}

void PrecisionMatrixMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<PrecisionMatrix >* >( newN );
    
}


void PrecisionMatrixMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}

