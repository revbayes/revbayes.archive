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
Func_generalRateMap::Func_generalRateMap( void ) : TypedFunction<RateMap>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_generalRateMap* Func_generalRateMap::clone( void ) const {
    
    return new Func_generalRateMap( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::RateMap>* Func_generalRateMap::createFunction() const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Simplex>* rf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural&>( this->args[2].getVariable()->getRevObject() ).getValue();
    size_t ns = rm->getValue().getNumberOfStates();

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
    
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_generalRateMap::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "qSite"           , RateGenerator::getClassTypeSpec(), "The rate generator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rfSite"          , Simplex::getClassTypeSpec()      , "The frequencies.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "num_chars"        , Natural::getClassTypeSpec()      , "The number of characters.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<TypeSpec> branchRateTypes;
        branchRateTypes.push_back( RealPos::getClassTypeSpec() );
        branchRateTypes.push_back( ModelVector<RealPos>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "branchRates"    , branchRateTypes, "The rates per branch.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );

        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_generalRateMap::getClassType(void)
{
    
    static std::string rev_type = "Func_generalRateMap";
    
	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_generalRateMap::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_generalRateMap::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnGeneralRateMap";
    
    return f_name;
}


const TypeSpec& Func_generalRateMap::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Set a member variable */
void Func_generalRateMap::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "qSite" )
    {
        q = var;
    }
    else if ( name == "rfSite" )
    {
        root_frequencies = var;
    }
    else if ( name == "branchRates" )
    {
        rate = var;
    }
    else if ( name == "num_chars" )
    {
        num_chars = var;
    }
    else
    {
        Function::setConstParameter(name, var);
    }
    
}
