//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_wag.h"
#include "Natural.h"
#include "RateMatrix_Wag.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_wag::Func_wag( void ) : Procedure( ) {
    
}


/** Clone object */
Func_wag* Func_wag::clone( void ) const {
    
    return new Func_wag( *this );
}


RevPtr<Variable> Func_wag::execute() {
    
    RevBayesCore::RateMatrix_Wag *rmj = new RevBayesCore::RateMatrix_Wag();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_wag::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_wag::getClassType(void) { 
    
    static std::string revType = "Func_wag";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_wag::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_wag::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_wag::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
