//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DistanceDependentDispersalFunction.h"
#include "Func_biogeo_grm.h"
#include "GeographyRateModifier.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlGeographyRateModifier.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_biogeo_grm::Func_biogeo_grm( void ) : Function( ) {
    
}


/** Clone object */
Func_biogeo_grm* Func_biogeo_grm::clone( void ) const {
    
    return new Func_biogeo_grm( *this );
}


RevPtr<Variable> Func_biogeo_grm::execute() {
    
    const RevBayesCore::TimeAtlas* atlas = &( static_cast<const RlAtlas&>( this->args[0].getVariable()->getRevObject() ).getValue() );
    
    RevBayesCore::TypedDagNode<double>* dp = static_cast<const RealPos&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    bool uadj   = static_cast<const RlBoolean &>( this->args[2].getVariable()->getRevObject() ).getValue();
    bool uav    = static_cast<const RlBoolean &>( this->args[3].getVariable()->getRevObject() ).getValue();
    bool udd    = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();

    RevBayesCore::DistanceDependentDispersalFunction* f = new RevBayesCore::DistanceDependentDispersalFunction(dp, atlas, uadj, true, udd);
    
    DeterministicNode<RevBayesCore::GeographyRateModifier> *detNode = new DeterministicNode<RevBayesCore::GeographyRateModifier>("", f, this->clone());
    
    RlGeographyRateModifier* value = new RlGeographyRateModifier( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_biogeo_grm::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "atlas", true, RlAtlas::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "distancePower", false, RealPos::getClassTypeSpec(), new RealPos(1e-5) ) );
        argumentRules.push_back( new ArgumentRule( "useAdjacency", false, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        argumentRules.push_back( new ArgumentRule( "useAvailable", false, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        argumentRules.push_back( new ArgumentRule( "useDistances", false, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_biogeo_grm::getClassType(void) {
    
    static std::string revType = "Func_biogeo_grm";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_biogeo_grm::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_biogeo_grm::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMap::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_biogeo_grm::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
