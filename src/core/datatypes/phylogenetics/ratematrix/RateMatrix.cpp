#include "RateMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix::RateMatrix(size_t n) : RateGenerator(n)
{

}





/** Destructor */
RateMatrix::~RateMatrix(void)
{
    
}


RateMatrix& RateMatrix::assign(const Assignable &m)
{
    const RateMatrix *rm = dynamic_cast<const RateMatrix*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix& x)
{
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
            o << x.getRate( i, j);
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

