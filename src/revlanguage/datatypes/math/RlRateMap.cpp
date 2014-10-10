//
//  RlRateMap.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlRateMap.h"

#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlTaxonData.h"

using namespace RevLanguage;

RateMap::RateMap(void) : ModelObject<RevBayesCore::RateMap>() {
    
}


RateMap::RateMap( RevBayesCore::RateMap *v) : ModelObject<RevBayesCore::RateMap>( v ) {
    
}


RateMap::RateMap( RevBayesCore::TypedDagNode<RevBayesCore::RateMap> *m) : ModelObject<RevBayesCore::RateMap>( m ) {
    
}


RateMap* RateMap::clone() const {
    return new RateMap( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RateMap::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
   
    if (name == "nChars")
    {
        int n = (int)this->dagNode->getValue().getNumberOfCharacters();
        return new Variable( new Natural(n) );
    }
    else if (name == "nStates")
    {
        int n = (int)this->dagNode->getValue().getNumberOfStates();
        return new Variable( new Natural(n) );
    }

    return ModelObject<RevBayesCore::RateMap>::executeMethod( name, args );
}


/* Get Rev type of object */
const std::string& RateMap::getClassType(void) {
    
    static std::string revType = "RateMap";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RateMap::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Initialize the member methods for this class */
void RateMap::initializeMethods( void ) const
{
    
    // Insert inherited methods
    ModelObject<RevBayesCore::RateMap>::initializeMethods();
    
    // add method for call "x.nChars()" as a function
    ArgumentRules* nCharsArgRules = new ArgumentRules();
    methods.addFunction("nChars",  new MemberProcedure( Natural::getClassTypeSpec(), nCharsArgRules) );
    
    // add method for call "x.nStates()" as a function
    ArgumentRules* nStatesArgRules = new ArgumentRules();
    methods.addFunction("nStates",  new MemberProcedure( Natural::getClassTypeSpec(), nStatesArgRules) );
    
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMap::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
