//
//  AbstractCharacterData.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/15/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "AbstractCharacterData.h"



std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractCharacterData& x) {
    
    o << "Origination:          " << x.getFileName() << std::endl;
    o << "Number of taxa:       " << x.getNumberOfTaxa() << std::endl;
    o << "Number of characters: " << x.getNumberOfCharacters() << std::endl;
    o << "Datatype:             " << x.getDatatype() << std::endl;
    
    return o;
}
