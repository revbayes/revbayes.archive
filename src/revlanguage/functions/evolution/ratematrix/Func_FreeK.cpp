//
//  Func_FreeK.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/19/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "Func_FreeK.h"
#include "FreeKRateMatrixFunction.h"
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
Func_FreeK::Func_FreeK( void ) : Function( ) {
    
}


/** Clone object */
Func_FreeK* Func_FreeK::clone( void ) const {
    
    return new Func_FreeK( *this );
}


RevPtr<Variable> Func_FreeK::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* tr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* sf = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::FreeKRateMatrixFunction* f = new RevBayesCore::FreeKRateMatrixFunction( tr,sf );
    DeterministicNode<RevBayesCore::RateMatrix> *detNode = new DeterministicNode<RevBayesCore::RateMatrix>("", f, this->clone());
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_FreeK::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "transitionRates", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "stationaryFrequencies", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_FreeK::getClassType(void) {
    
    static std::string revType = "Func_FreeK";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_FreeK::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_FreeK::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_FreeK::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
