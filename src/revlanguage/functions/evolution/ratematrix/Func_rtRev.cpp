//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_rtRev.h"
#include "Natural.h"
#include "RateMatrix_RtRev.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_rtRev::Func_rtRev( void ) : Procedure( ) {
    
}


/** Clone object */
Func_rtRev* Func_rtRev::clone( void ) const {
    
    return new Func_rtRev( *this );
}


RevPtr<Variable> Func_rtRev::execute() {
    
    
    RevBayesCore::RateMatrix_RtRev *rmj = new RevBayesCore::RateMatrix_RtRev();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_rtRev::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_rtRev::getClassType(void) { 
    
    static std::string revType = "Func_rtRev";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_rtRev::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_rtRev::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_rtRev::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
