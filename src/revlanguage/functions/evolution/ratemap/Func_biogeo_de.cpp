//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "Func_FreeBinary.h"
#include "Func_biogeo_de.h"
#include "GeographyRateModifier.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlRateMatrix.h"
#include "RateMatrix_FreeBinary.h"
#include "RateMap_Biogeography.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlGeographyRateModifier.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_biogeo_de::Func_biogeo_de( void ) : Function( ) {
    
}


/** Clone object */
Func_biogeo_de* Func_biogeo_de::clone( void ) const {
    
    return new Func_biogeo_de( *this );
}


RevPtr<Variable> Func_biogeo_de::execute() {
    
//    RevBayesCore::TypedDagNode<std::vector<double> >* glr = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* rf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::GeographyRateModifier>* grm = static_cast<const RlGeographyRateModifier&>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural&>( this->args[3].getVariable()->getRevObject() ).getValue();
    bool fe = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();
//    RevBayesCore::TypedDagNode<std::vector<double> >* r = static_cast<const ModelVector<RealPos> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::BiogeographyRateMapFunction* f = new RevBayesCore::BiogeographyRateMapFunction(nc,fe); //(nc, true);
    f->setRateMatrix(rm);
    f->setGeographyRateModifier(grm);
    f->setRootFrequencies(rf);
    
    DeterministicNode<RevBayesCore::RateMap> *detNode = new DeterministicNode<RevBayesCore::RateMap>("", f, this->clone());
    
    RateMap* value = new RateMap( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_biogeo_de::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
//        argumentRules.push_back( new ArgumentRule( "gainLossRates"   , ModelVector<RealPos>::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "gainLossRates"   , RateMatrix::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rootFrequencies" , Simplex::getClassTypeSpec()                , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Simplex( std::vector<double>(2,0.5)) ) );
        argumentRules.push_back( new ArgumentRule( "geoRateMod"      , RlGeographyRateModifier::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ));
        argumentRules.push_back( new ArgumentRule( "numAreas"        , Natural::getClassTypeSpec()                , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "forbidExtinction", RlBoolean::getClassTypeSpec()              , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_biogeo_de::getClassType(void) {
    
    static std::string revType = "Func_biogeo_de";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_biogeo_de::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_biogeo_de::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMap::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_biogeo_de::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
