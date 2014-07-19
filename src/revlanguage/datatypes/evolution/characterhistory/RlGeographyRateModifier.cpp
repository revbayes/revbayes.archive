//
//  RlGeographyRateModifier.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlGeographyRateModifier.h"
#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "ModelVector.h"
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


/** Get the methods for this vector class */
/* Get method specifications */
const MethodTable& RlGeographyRateModifier::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
//        // add method for call "x[]" as a function
//        ArgumentRules* squareBracketArgRules = new ArgumentRules();
//        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
//        methods.addFunction("[]",  new MemberFunction( ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
//        
//        // add method for call "x[]" as a function
//        ArgumentRules* sizeArgRules = new ArgumentRules();
//        methods.addFunction("size",  new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::GeographyRateModifier>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlGeographyRateModifier::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
