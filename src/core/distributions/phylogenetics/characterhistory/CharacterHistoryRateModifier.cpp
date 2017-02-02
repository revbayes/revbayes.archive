//
//  CharacterHistoryRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "CharacterHistoryRateModifier.h"
#include <iostream>

using namespace RevBayesCore;

CharacterHistoryRateModifier::CharacterHistoryRateModifier(void)
{
    ;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<CharacterHistoryRateModifier*>& x)
{
    o << " ";
    
    return o;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const CharacterHistoryRateModifier& x)
{
    o << " ";
    
    return o;
}
