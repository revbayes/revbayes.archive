#include "CharacterEventContinuous.h"
#include "CharacterEventType.h"
#include <iostream>
#include <sstream>

using namespace RevBayesCore;


CharacterEventContinuous::CharacterEventContinuous(void) : CharacterEvent(),
    value(0.0)
{
    
}

CharacterEventContinuous::CharacterEventContinuous(size_t i, double v, double a, size_t t) : CharacterEvent(i,a,t),
    value(v)
{
    
}

CharacterEventContinuous::CharacterEventContinuous(const CharacterEventContinuous& c) : CharacterEvent(c),
    value(c.value)
{
    
}

CharacterEventContinuous::~CharacterEventContinuous(void)
{
    
}

CharacterEventContinuous* CharacterEventContinuous::clone( void ) const
{
    return new CharacterEventContinuous( *this );
}


double CharacterEventContinuous::getState(void) const
{
    return value;
}

std::string CharacterEventContinuous::getStateStr(void) const
{
    std::stringstream ss;
    // want to do this based on DiscreteDataType...
    //    ss << (char)(70+state);
    ss << value;
    return ss.str();
}


void CharacterEventContinuous::setState(double s)
{
    value = s;
}

