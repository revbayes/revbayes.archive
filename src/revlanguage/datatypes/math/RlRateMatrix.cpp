//
//  RlRateMatrix.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlRateMatrix.h"

#include "ArgumentRule.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlTaxonData.h"

using namespace RevLanguage;

RateMatrix::RateMatrix(void) : ModelObject<RevBayesCore::RateMatrix>() {
    
}


RateMatrix::RateMatrix( const RevBayesCore::RateMatrix &v) : ModelObject<RevBayesCore::RateMatrix>( v.clone() ) {
    
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
        RevBayesCore::RbVector<double> elementVector;
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


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& RateMatrix::getMethods( void ) const
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
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


/** Make member methods for this class */
MethodTable RateMatrix::makeMethods( void ) const
{
    MethodTable methods = MethodTable();
    
    // add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]",  new MemberProcedure( ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    
    // add method for call "x[]" as a function
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
    
    // Insert inherited methods
    methods.insertInheritedMethods( ModelObject<RevBayesCore::RateMatrix>::makeMethods() );
    
    return methods;
}

