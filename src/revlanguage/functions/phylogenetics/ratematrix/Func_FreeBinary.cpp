//
//  Func_FreeBinary.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/19/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "Func_FreeBinary.h"
#include "FreeBinaryRateMatrixFunction.h"
#include "FreeBinaryRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_FreeBinary::Func_FreeBinary( void ) : TypedFunction<RateGenerator>( ) {
    
}


/** Clone object */
Func_FreeBinary* Func_FreeBinary::clone( void ) const {
    
    return new Func_FreeBinary( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_FreeBinary::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* glr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::FreeBinaryRateMatrixFunction* f = new RevBayesCore::FreeBinaryRateMatrixFunction( glr );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_FreeBinary::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "transitionRates", ModelVector<RealPos>::getClassTypeSpec(), "The transition rates between the two states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_FreeBinary::getClassType(void) {
    
    static std::string revType = "Func_FreeBinary";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_FreeBinary::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_FreeBinary::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
