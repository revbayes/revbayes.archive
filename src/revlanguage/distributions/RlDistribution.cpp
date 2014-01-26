//
//  Distribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RlDistribution.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

Distribution::Distribution() : RbLanguageObject() {
    
}



Distribution::~Distribution() {
    
}



/* Get class name of object */
const std::string& Distribution::getClassName(void) { 
    
    static std::string rbClassName = "Distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Distribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


void Distribution::printValue(std::ostream &o) const 
{
    o << getClassName() << "(...)" << std::endl;
}
