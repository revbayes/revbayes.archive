//
//  AbstractCharacterHistoryRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AbstractCharacterHistoryRateModifier.h"
#include <iostream>

using namespace RevBayesCore;

AbstractCharacterHistoryRateModifier::AbstractCharacterHistoryRateModifier(void)
{
    ;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<AbstractCharacterHistoryRateModifier*>& x)
{
    o << " ";
    
    return o;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractCharacterHistoryRateModifier& x)
{
    o << " ";
    
    return o;
}
