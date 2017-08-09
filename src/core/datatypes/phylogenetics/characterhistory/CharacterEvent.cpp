#include "CharacterEvent.h"
#include "CharacterEventType.h"
#include <iostream>
#include <sstream>

using namespace RevBayesCore;


CharacterEvent::CharacterEvent(void)
{

}

CharacterEvent::CharacterEvent(size_t i, size_t s, double a, size_t t) :
    site_index(i),
    state(s),
    age(a),
    event_type(t)
{

}

CharacterEvent::CharacterEvent(const CharacterEvent& c) :
    site_index(c.site_index),
    state(c.state),
    age(c.age),
    event_type(c.event_type)
{

}

CharacterEvent::~CharacterEvent(void)
{

}

CharacterEvent* CharacterEvent::clone( void ) const
{
    return new CharacterEvent( *this );
}

bool CharacterEvent::operator<(const CharacterEvent& rhs) const
{
    return age < rhs.age;
}

bool CharacterEvent::operator>(const CharacterEvent& rhs) const
{
    return age > rhs.age;
}

double CharacterEvent::getAge(void) const
{
    return age;
}

size_t CharacterEvent::getEventType(void) const
{
    return event_type;
}

size_t CharacterEvent::getSiteIndex(void) const
{
    return site_index;
}

size_t CharacterEvent::getState(void) const
{
    return state;
}

std::string CharacterEvent::getStateStr(void) const
{
    std::stringstream ss;
    // want to do this based on DiscreteDataType...
//    ss << (char)(70+state);
    ss << state;
    return ss.str();
}

void CharacterEvent::setAge(double a)
{
    age = a;
}

void CharacterEvent::setEventType(size_t t)
{
    event_type = t;
}

void CharacterEvent::setSiteIndex(size_t i)
{
    site_index = i;
}

void CharacterEvent::setState(size_t s)
{
    state = s;
}



void CharacterEvent::print(void) const
{
    std::cout << site_index << " " << state << " " << age << "\n";
}
