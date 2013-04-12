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



Distribution::Distribution( const Distribution &d ) : RbLanguageObject(d) {
    
}



Distribution::~Distribution() {
    
}



/** Debug info about object */
std::string Distribution::debugInfo(void) const {
    
    std::ostringstream o;
    o << getTypeSpec() << ": " << std::endl;
    
//    for ( size_t i = 0;  i < args.size(); i++ ) {
//        o << " args[" << i << "] = ";
//        args[i].getVariable()->getValue().printValue(o);
//        o << std::endl;
//    }
    
    return o.str();
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
