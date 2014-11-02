
//
//  Func_atlasEr.cpp
//  RevBayesCore
//
//  Created by Michael Landis on 11/1/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GtrRateMatrixFunction.h"
#include "Func_atlasEr.h"
#include "ModelVector.h"
#include "Procedure.h"
#include "RateMatrix_GTR.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"
#include "Variable.h"
#include "VectorFunction.h"

using namespace RevLanguage;

/** default constructor */
Func_atlasEr::Func_atlasEr( void ) : Procedure( ) {
    
}


/** Clone object */
Func_atlasEr* Func_atlasEr::clone( void ) const {
    
    return new Func_atlasEr( *this );
}


RevPtr<Variable> Func_atlasEr::execute( void )
{

    const RevBayesCore::TimeAtlas* atlas = &( static_cast<const RlAtlas&>( this->args[0].getVariable()->getRevObject() ).getValue() );
    bool symmetric = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
    std::vector<std::vector<RevBayesCore::GeographicArea*> > areas = atlas->getAreas();
    
    ModelVector<ModelVector<RealPos> > *f = new ModelVector<ModelVector<RealPos> >();
    for (size_t i = 0; i < areas.size(); i++)
    {
        ModelVector<RealPos> v;
        for (size_t j = 0; j < areas[i].size(); j++)
        {
            std::vector<double> a = areas[i][j]->getDispersalValues();
            for (size_t k = 0; k < areas[i].size(); k++)
            {
                if (j < k || !symmetric)
                {
                    v.push_back(a[k]);
                }
            }
        }
        f->push_back(v);
    }
    return new Variable(f);
}


/* Get argument rules */
const ArgumentRules& Func_atlasEr::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "atlas"          , RlAtlas::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "symmetric"      , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_atlasEr::getClassType(void) {
    
    static std::string revType = "Func_atlasEr";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_atlasEr::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_atlasEr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Get return type */
const TypeSpec& Func_atlasEr::getReturnType( void ) const {
    
    return ModelVector<ModelVector<RealPos> >::getClassTypeSpec();
}
