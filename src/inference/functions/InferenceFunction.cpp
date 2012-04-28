//
//  InferenceFunction.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceFunction.h"
#include <typeinfo>
#include <string>

std::string InferenceFunction::toString( void ) const {
    std::string name = typeid(*this).name();
    name += " (";
    
    for (std::vector<RbValue<void*> >::const_iterator i = members.begin(); i != members.end(); ++i) {
        if ( i != members.begin() ) {
            name += ", ";
        }
        name += long( i->value );
    }
    name += ")";
    
    return name;
}


