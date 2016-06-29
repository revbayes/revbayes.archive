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



void RateGenerator::printForUser(std::ostream &o, const std::string &sep, int l, bool left) const
{
    std::streamsize previous_precision = o.precision();
    std::ios_base::fmtflags previous_flags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        if (i == 0)
        {
            o << "[ ";
        }
        else
        {
            o << "  ";
        }
        
        for (size_t j = 0; j < size(); ++j)
        {
            if (j != 0)
            {
                o << ", ";
            }
            o << getRate( i, j, 1e-6,1.0);
        }
        o <<  " ]";
        
        if (i == size()-1)
        {
            o << " ]";
        }
        else
        {
            o << " ,\n";
        }
        
    }
    
    o.setf(previous_flags);
    o.precision(previous_precision);
    
}



void RateGenerator::printForSimpleStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        if (i > 0)
        {
            o << sep;
        }
        for (size_t j = 0; j < size(); ++j)
        {
            if (j > 0)
            {
                o << sep;
            }
            o << getRate( i, j, 1e-6, 1.0);
        }
        
    }
    
}



void RateGenerator::printForComplexStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    
    o << "[ ";
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        if (i == 0)
        {
            o << "[ ";
        }
        else
        {
            o << "  ";
        }
        
        for (size_t j = 0; j < size(); ++j)
        {
            if (j != 0)
            {
                o << ", ";
            }
            o << getRate( i, j, 1e-6, 1.0);
        }
        o <<  " ]";
        
        if (i == size()-1)
        {
            o << " ]";
        }
        else
        {
            o << " ,";
        }
        
    }
    
}
