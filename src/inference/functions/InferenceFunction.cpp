//
//  InferenceFunction.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceFunction.h"
#include <sstream>
#include <string>
#include <typeinfo>

std::string InferenceFunction::toString( void ) const {
    std::string name = "";
    std::stringstream o;
    o << typeid(*this).name();
    o << " (";
    
    for (std::vector<RbValue<void*> >::const_iterator i = members.begin(); i != members.end(); ++i) {
        if ( i != members.begin() ) {
            o << ", ";
        }
        const RbValue<void*>& val = *i;
        if ( val.value == NULL ) {
            o << "NULL";
        }
        else {
            o << long( val.value );
        }
    }
    o << ")";
    
    name = o.str();
    
    return name;
}


