//
//  CladogeneticProbabilityMatrix.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/23/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "CladogeneticProbabilityMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
CladogeneticProbabilityMatrix::CladogeneticProbabilityMatrix(void) :
num_states( 0 )
{
    ; // do nothing
}

CladogeneticProbabilityMatrix::CladogeneticProbabilityMatrix(size_t n) :
num_states( n )
{
    ; // do nothing
}

CladogeneticProbabilityMatrix::~CladogeneticProbabilityMatrix(void)
{
    ; // do nothing
}

CladogeneticProbabilityMatrix& CladogeneticProbabilityMatrix::assign(const Assignable &m)
{
    
    const CladogeneticProbabilityMatrix *cp = dynamic_cast<const CladogeneticProbabilityMatrix*>(&m);
    if ( cp != NULL )
    {
        return operator=(*cp);
    }
    else
    {
        throw RbException("Could not assign cladogenetic probability matrix.");
    }
}


/*
void CladogeneticProbabilityMatrix::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
    
    
    calculateTransitionProbabilities(t, 0.0, 1.0, P);
}
*/


CladogeneticProbabilityMatrix* CladogeneticProbabilityMatrix::clone( void ) const
{
    
    return new CladogeneticProbabilityMatrix( *this );
}


std::map<std::vector<unsigned>, double> CladogeneticProbabilityMatrix::getEventMap()
{
    return eventMapProbs;
}

const std::map<std::vector<unsigned>, double>& CladogeneticProbabilityMatrix::getEventMap() const
{
    return eventMapProbs;
}

size_t CladogeneticProbabilityMatrix::getNumberOfStates( void ) const
{
    return num_states;
}

void CladogeneticProbabilityMatrix::initFromString( const std::string &s )
{
    throw RbException("MJL (23/10/2016): Missing implementation!");
}

void CladogeneticProbabilityMatrix::setEventMap(std::map<std::vector<unsigned>, double> m)
{
    eventMapProbs = m;
}


size_t CladogeneticProbabilityMatrix::size( void ) const
{
    return num_states;
}




void CladogeneticProbabilityMatrix::printForUser(std::ostream &o, const std::string &sep, int l, bool left) const
{
    /*
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
    */
}




void CladogeneticProbabilityMatrix::printForSimpleStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    /*
    
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
    */
}



void CladogeneticProbabilityMatrix::printForComplexStoring(std::ostream &o, const std::string &sep, int l, bool left) const
{
    /*
    o << "[ ";
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < size(); i++)
    {
        o << "[ ";
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
     */
    
}
