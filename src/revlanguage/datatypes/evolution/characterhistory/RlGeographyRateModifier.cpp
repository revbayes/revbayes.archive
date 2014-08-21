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
#include "RlTaxonData.h"

using namespace RevLanguage;

RlGeographyRateModifier::RlGeographyRateModifier(void) : ModelObject<RevBayesCore::GeographyRateModifier>() {
    
}


RlGeographyRateModifier::RlGeographyRateModifier( RevBayesCore::GeographyRateModifier *v) : ModelObject<RevBayesCore::GeographyRateModifier>( v ) {
    
}


RlGeographyRateModifier::RlGeographyRateModifier( RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier> *m) : ModelObject<RevBayesCore::GeographyRateModifier>( m ) {
    
}


RlGeographyRateModifier* RlGeographyRateModifier::clone() const {
    return new RlGeographyRateModifier( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RlGeographyRateModifier::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    
//    if (name == "getNumberOfCharacters")
//    {
//        int n = (int)this->value->getValue().getNumberOfCharacters();
//        return new Natural(n);
//    }
//    else if (name == "getNumberOfStates")
//    {
//        int n = (int)this->value->getValue().getNumberOfStates();
//        return new Natural(n);
//    }
    
    return ModelObject<RevBayesCore::GeographyRateModifier>::executeMethod( name, args );
}


/* Get Rev type of object */
const std::string& RlGeographyRateModifier::getClassType(void) {
    
    static std::string revType = "RlGeographyRateModifier";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlGeographyRateModifier::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& RlGeographyRateModifier::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlGeographyRateModifier::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
