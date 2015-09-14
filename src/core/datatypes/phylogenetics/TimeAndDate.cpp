#include "TimeAndDate.h"

using namespace RevBayesCore;

TimeAndDate::TimeAndDate()
{
    
}

/**
 * Equals operator.

 */
bool TimeAndDate::operator==(const RevBayesCore::TimeAndDate &t) const
{
    
    if ( getDaysAD() != t.getDaysAD() )
    {
        return false;
    }
        
    return true;
}


/**
 * Not equals operator. We simply invert the result of the equals operation.
 */
bool TimeAndDate::operator!=(const RevBayesCore::TimeAndDate &t) const
{
    
    return !operator==(t);
}

/**
 * Get the number of days since counting.
 * DUMMY FUNCTION.
 */
double TimeAndDate::getDaysAD(void) const {
    return 0;
}


/**
 * Get the number of million years ago.
 * DUMMY FUNCTION.
 */
double TimeAndDate::getMya(void) const{
    return 0;
}

/**
 * Get the number of years since counting.
 * DUMMY FUNCTION.
 */
double TimeAndDate::getYearsAD(void) const{
    return 0;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::TimeAndDate& x) {
    o << "";    
    return o;
    
}
