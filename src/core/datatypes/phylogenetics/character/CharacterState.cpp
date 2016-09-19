//
//  CharacterState.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CharacterState.h"


using namespace RevBayesCore;


CharacterState::CharacterState() :
    isGap( false ),
    isMissing( false )
{
    
}


bool CharacterState::isGapState( void ) const
{
    return isGap;
}


bool CharacterState::isMissingState( void ) const
{
    return isMissing;
}


void CharacterState::setGapState( bool tf )
{
    isGap = tf;
}


void CharacterState::setMissingState( bool tf )
{
    isMissing = tf;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const CharacterState& x)
{
    o << x.getStringValue();
    
    return o;
}

