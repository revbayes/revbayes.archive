#include "MatrixReal.h"
#include "RateMap.h"
#include "RateMatrix.h"
#include "RateMatrix_JC.h"
#include "RbVector.h"
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
RateMap::RateMap(size_t ns, size_t nc)
{
    
    numStates            = ns;
    numCharacters        = nc;
    
    needsUpdate          = true;
    
}


/** Destructor */
RateMap::~RateMap(void)
{
    
    
}


size_t RateMap::getNumberOfStates( void ) const
{
    return numStates;
}

size_t RateMap::getNumberOfCharacters( void ) const
{
    return numCharacters;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMap& x)
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


void RateMap::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...
        
        needsUpdate = false;
    }
}

