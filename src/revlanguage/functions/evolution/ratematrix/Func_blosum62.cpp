//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_blosum62.h"
#include "Natural.h"
#include "RateMatrix_Blosum62.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_blosum62::Func_blosum62( void ) : Procedure( ) {
    
}


/** Clone object */
Func_blosum62* Func_blosum62::clone( void ) const {
    
    return new Func_blosum62( *this );
}


RevPtr<Variable> Func_blosum62::execute() {
    
    
    RevBayesCore::RateMatrix_Blosum62 *rmj = new RevBayesCore::RateMatrix_Blosum62();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_blosum62::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_blosum62::getClassType(void) { 
    
    static std::string revType = "Func_blosum62";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_blosum62::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_blosum62::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_blosum62::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
