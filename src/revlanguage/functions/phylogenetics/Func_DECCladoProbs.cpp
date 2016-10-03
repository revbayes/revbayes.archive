//
//  Func_DECCladoProbs.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "Func_DECCladoProbs.h"
#include "ConstantNode.h"
#include "DECCladogeneticStateFunction.h"
#include "MatrixReal.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_DECCladoProbs::Func_DECCladoProbs( void ) : TypedFunction<MatrixReal>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_DECCladoProbs* Func_DECCladoProbs::clone( void ) const {
    
    return new Func_DECCladoProbs( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_DECCladoProbs::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ep = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getValue();
    unsigned ns = static_cast<const Natural &>( this->args[2].getVariable()->getRevObject() ).getValue();
    std::string pt = static_cast<const RlString &> ( this->args[3].getVariable()->getRevObject() ).getValue();
    bool ept = (pt == "pattern");
    bool wa = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();
    bool os = static_cast<const RlBoolean&>(this->args[5].getVariable()->getRevObject() ).getValue();
    
//    if ( er->getValue().size() != (bf->getValue().size() * (bf->getValue().size()-1) / 2.0) )
//    {
//        throw RbException("The dimension betwee the base frequencies and the substitution rates does not match.");
//    }
    RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >* er = new RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >("er", new RevBayesCore::RbVector<double>(2,.5) );
    
    
    RevBayesCore::DECCladogeneticStateFunction* f = new RevBayesCore::DECCladogeneticStateFunction( ep, er, nc, ns, ept, wa, os );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_DECCladoProbs::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "eventProbs", Simplex::getClassTypeSpec(), "The probabilities of the different event types.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
//        argumentRules.push_back( new ArgumentRule( "eventRates", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "numCharacters", Natural::getClassTypeSpec(), "The number of characters.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "num_states", Natural::getClassTypeSpec(), "The number of states,", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> options;
        options.push_back( "pattern" );
        options.push_back( "class" );
        argumentRules.push_back( new OptionRule( "probType", new RlString("pattern"), options, "Assign event weights over classes of patterns or over specific patterns" ) );
        
        argumentRules.push_back( new ArgumentRule( "widespreadAllopatry", RlBoolean::getClassTypeSpec(), "Allopatry may result in both daughter ranges being larger than size 1.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        
        argumentRules.push_back( new ArgumentRule( "orderStatesBySize", RlBoolean::getClassTypeSpec(), "Order states by size?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ));
        
        
        rules_set = true;
    }
    
    return argumentRules;
}



const std::string& Func_DECCladoProbs::getClassType(void)
{
    
    static std::string revType = "Func_DECCladoProbs";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_DECCladoProbs::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_DECCladoProbs::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnDECCladoProbs";
    
    return f_name;
}

std::vector<std::string> Func_DECCladoProbs::getFunctionNameAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "fnCladoProbs" );
    
    return a_names;
}


const TypeSpec& Func_DECCladoProbs::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
