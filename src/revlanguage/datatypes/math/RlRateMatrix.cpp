//
//  RlRateMatrix.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlRateMatrix.h"

#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlTaxonData.h"

using namespace RevLanguage;

RateMatrix::RateMatrix(void) : ModelObject<RevBayesCore::RateMatrix>() {
    
}


RateMatrix::RateMatrix( RevBayesCore::RateMatrix *v) : ModelObject<RevBayesCore::RateMatrix>( v ) {
    
}


RateMatrix::RateMatrix( RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix> *m) : ModelObject<RevBayesCore::RateMatrix>( m ) {
    
}


RateMatrix* RateMatrix::clone() const {
    return new RateMatrix( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RateMatrix::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "[]") {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
        
        if (this->dagNode->getValue().getNumberOfStates() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        const std::vector<double>& element = this->dagNode->getValue()[ size_t(index.getValue()) - 1];
        std::vector<double> elementVector;
        for (size_t i=0; i < this->dagNode->getValue().size(); ++i) {
            elementVector.push_back( element[i] );
        }
        return new Variable( new ModelVector<Real>( elementVector ) );
    }
    else if (name == "size") {
        int n = (int)this->dagNode->getValue().getNumberOfStates();
        return new Variable( new Natural(n) );
    }
    
    return ModelObject<RevBayesCore::RateMatrix>::executeMethod( name, args );
}


/* Get Rev type of object */
const std::string& RateMatrix::getClassType(void) { 
    
    static std::string revType = "RateMatrix";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& RateMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get the methods for this vector class */
/* Get method specifications */
const MethodTable& RateMatrix::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable(); 
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
        
        
        // add method for call "x[]" as a function
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size",  new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::RateMatrix>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

