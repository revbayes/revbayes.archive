//
//  RateMap.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "MatrixReal.h"
#include "RateMap.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMap::RateMap(size_t ns, size_t nc) {
    
    numStates            = ns;
    numCharacters        = nc;
    needsUpdate          = true;
    
}



/** Copy constructor */
RateMap::RateMap(const RateMap& m) {
    
    numStates            = m.numStates;
    numCharacters        = m.numCharacters;
    needsUpdate          = m.needsUpdate;
    
}


/** Destructor */
RateMap::~RateMap(void) {
    
}


RateMap& RateMap::operator=(const RateMap &r) {
    
    if (this != &r) {
        
        numStates           = r.numStates;
        numCharacters       = r.numCharacters;
        needsUpdate         = true;
        
    }
    
    return *this;
}

size_t RateMap::getNumberOfStates( void ) const {
    return numStates;
}

size_t RateMap::getNumberOfCharacters( void ) const {
    return numCharacters;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMap& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}
