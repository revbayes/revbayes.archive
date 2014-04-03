//
//  RateFunctionMatrix.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/1/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateFunctionMatrix.h"
#include "RbException.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateFunctionMatrix::RateFunctionMatrix(size_t ns, size_t nc) : needsUpdate( true ) {
    
    numStates            = ns;
    numCharacters        = nc;
}

/** Copy constructor */
RateFunctionMatrix::RateFunctionMatrix(const RateFunctionMatrix& m) : needsUpdate( true ){
    
    numStates            = m.numStates;
    numCharacters        = m.numCharacters;
    
}


/** Destructor */
RateFunctionMatrix::~RateFunctionMatrix(void) {
    
}

RateFunctionMatrix& RateFunctionMatrix::operator=(const RateFunctionMatrix &r) {
    
    if (this != &r) {
        
        numStates           = r.numStates;
        numCharacters       = r.numCharacters;
        needsUpdate         = true;
        
    }
    
    return *this;
}

size_t RateFunctionMatrix::getNumberOfStates( void ) const {
    return numStates;
}

size_t RateFunctionMatrix::getNumberOfCharacters( void ) const {
    return numCharacters;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateFunctionMatrix& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
   
    /*
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
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else
            o << " ,\n";
        
    }
    */
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}
