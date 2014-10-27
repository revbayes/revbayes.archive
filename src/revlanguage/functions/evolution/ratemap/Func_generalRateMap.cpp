//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "Func_generalRateMap.h"
#include "GeneralRateMapFunction.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlRateMatrix.h"
#include "RateMatrix.h"
#include "RateMap.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMap.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_generalRateMap::Func_generalRateMap( void ) : Function( ) {
    
}


/** Clone object */
Func_generalRateMap* Func_generalRateMap::clone( void ) const {
    
    return new Func_generalRateMap( *this );
}


RevPtr<Variable> Func_generalRateMap::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix>* rm = static_cast<const RateMatrix&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural&>( this->args[2].getVariable()->getRevObject() ).getValue();
    unsigned ns = rm->getValue().getNumberOfStates();

    RevBayesCore::GeneralRateMapFunction* f = new RevBayesCore::GeneralRateMapFunction(ns, nc);
        
    if ( this->args[3].getVariable()->getRevObject().isType( ModelVector<RealPos>::getClassTypeSpec() ) )
    {
        RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* clockRates = static_cast<const ModelVector<RealPos> &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
        
        //        // sanity check
        //        if ( (nNodes-1) != clockRates->getValue().size() )
        //        {
        //            throw RbException( "The number of clock rates does not match the number of branches" );
        //        }
        
        f->setClockRate( clockRates );
    }
    else
    {
        RevBayesCore::TypedDagNode<double>* clockRate = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
        f->setClockRate( clockRate );
    }
    
    f->setRateMatrix(rm);
    f->setRootFrequencies(rf);
    
    
    
    DeterministicNode<RevBayesCore::RateMap> *detNode = new DeterministicNode<RevBayesCore::RateMap>("", f, this->clone());
    
    RateMap* value = new RateMap( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_generalRateMap::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "qSite"           , RateMatrix::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rfSite"          , Simplex::getClassTypeSpec()                , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "numChars"        , Natural::getClassTypeSpec()                , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "branchRates"    , branchRateTypes, ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_generalRateMap::getClassType(void) {
    
    static std::string revType = "Func_generalRateMap";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_generalRateMap::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_generalRateMap::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMap::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_generalRateMap::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Set a member variable */
void Func_generalRateMap::setConstParameter(const std::string& name, const RevPtr<const Variable> &var)
{

    if ( name == "qSite" )
    {
        q = var;
    }
    else if ( name == "rfSite" )
    {
        rootFrequencies = var;
    }
    else if ( name == "branchRates" )
    {
        rate = var;
    }
    else if ( name == "numChars" )
    {
        numChars = var;
    }
    else
    {
        Function::setConstParameter(name, var);
    }
    
}
