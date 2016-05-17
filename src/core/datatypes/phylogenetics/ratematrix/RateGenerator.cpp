//
//  RateGenerator.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "RateGenerator.h"
#include "RbException.h"
#include "RbMathMatrix.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateGenerator::RateGenerator(size_t n) :
    num_states( n )
{
    ; // do nothing
}

RateGenerator::~RateGenerator(void)
{
    ; // do nothing
}


RateGenerator& RateGenerator::assign(const Assignable &m)
{
    const RateGenerator *rm = dynamic_cast<const RateGenerator*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

void RateGenerator::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
    
    
    calculateTransitionProbabilities(t, 0.0, 1.0, P);
}

size_t RateGenerator::getNumberOfStates( void ) const
{
    return num_states;
}



size_t RateGenerator::size( void ) const
{
    return num_states;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateGenerator& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j)
        {
            if (j != 0)
                o << ", ";
                o << x.getRate(i,j,1e-6,1.0);
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}
