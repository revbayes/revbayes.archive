//
//  CharacterState.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CharacterState.h"



std::ostream& RevBayesCore::operator<<(std::ostream& o, const CharacterState& x) {
    o << x.getStringValue();
    
    return o;
}

