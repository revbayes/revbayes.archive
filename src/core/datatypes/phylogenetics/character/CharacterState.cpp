#include "CharacterState.h"


using namespace RevBayesCore;


CharacterState::CharacterState() :
    is_gap( false ),
    is_missing( false )
{
    
}


bool CharacterState::isGapState( void ) const
{
    return is_gap;
}


bool CharacterState::isMissingState( void ) const
{
    return is_missing;
}


void CharacterState::setGapState( bool tf )
{
    is_gap = tf;
}


void CharacterState::setMissingState( bool tf )
{
    is_missing = tf;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const CharacterState& x)
{
    o << x.getStringValue();
    
    return o;
}

