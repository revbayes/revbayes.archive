//
//  RlGeographyRateModifier.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlGeographyRateModifier.h"
#include "ArgumentRule.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"

using namespace RevLanguage;

GeographyRateModifier::GeographyRateModifier(void) : ModelObject<RevBayesCore::GeographyRateModifier>() {
    
}


GeographyRateModifier::GeographyRateModifier( RevBayesCore::GeographyRateModifier *v) : ModelObject<RevBayesCore::GeographyRateModifier>( v ) {
    
}


GeographyRateModifier::GeographyRateModifier( RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier> *m) : ModelObject<RevBayesCore::GeographyRateModifier>( m ) {
    
}


GeographyRateModifier* GeographyRateModifier::clone() const {
    return new GeographyRateModifier( *this );
}


/* Get Rev type of object */
const std::string& GeographyRateModifier::getClassType(void) {
    
    static std::string rev_type = "GeographyRateModifier";
    
	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& GeographyRateModifier::getClassTypeSpec(void) {
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& GeographyRateModifier::getTypeSpec(void) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}
