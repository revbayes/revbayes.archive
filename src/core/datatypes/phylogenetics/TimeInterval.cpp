#include "TimeInterval.h"

using namespace RevBayesCore;

TimeInterval::TimeInterval() : start(0), end(0)
{
    
}

/**
 * Equals operator.

 */
bool TimeInterval::operator==(const RevBayesCore::TimeInterval &t) const
{
    
    if ( start != t.start || end != t.end )
    {
        return false;
    }
        
    return true;
}


/**
 * Not equals operator. We simply invert the result of the equals operation.
 */
bool TimeInterval::operator!=(const RevBayesCore::TimeInterval &t) const
{
    
    return !operator==(t);
}


/**
 * Get the beginning time of the interval
 */
double TimeInterval::getStart(void) const
{

    return start;
}


/**
 * Set the beginning time of the interval
 */
void TimeInterval::setStart(double s)
{

    start = s;
}


/**
 * Get the beginning time of the interval
 */
double TimeInterval::getEnd(void) const
{

    return end;
}


/**
 * Set the beginning time of the interval
 */
void TimeInterval::setEnd(double s)
{
    
    end = s;
}
