//
//  Func_atlasEpochs.cpp
//  RevBayesCore
//
//  Created by Michael Landis on 11/1/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GtrRateMatrixFunction.h"
#include "Func_atlasEpochs.h"
#include "ModelVector.h"
#include "Procedure.h"
#include "RateMatrix_GTR.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAtlas.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"
#include "Variable.h"
#include "VectorFunction.h"

using namespace RevLanguage;

/** default constructor */
Func_atlasEpochs::Func_atlasEpochs( void ) : Procedure( ) {
    
}


/** Clone object */
Func_atlasEpochs* Func_atlasEpochs::clone( void ) const {
    
    return new Func_atlasEpochs( *this );
}


RevPtr<Variable> Func_atlasEpochs::execute( void )
{
    
    const RevBayesCore::TimeAtlas* atlas = &( static_cast<const RlAtlas&>( this->args[0].getVariable()->getRevObject() ).getValue() );
    std::vector<double> ages = atlas->getEpochs();
    
    ModelVector<RealPos> *f = new ModelVector<RealPos>(ages);
    
    return new Variable(f);
}


/* Get argument rules */
const ArgumentRules& Func_atlasEpochs::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "atlas"        , RlAtlas::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_atlasEpochs::getClassType(void) {
    
    static std::string revType = "Func_atlasEpochs";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_atlasEpochs::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_atlasEpochs::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Get return type */
const TypeSpec& Func_atlasEpochs::getReturnType( void ) const {
    
    return ModelVector<ModelVector<RealPos> >::getClassTypeSpec();
}
