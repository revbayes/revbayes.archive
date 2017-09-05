//
//  Function_phylogeneticDistanceRateModifier.cpp
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/24/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "Func_phylogeneticDistanceRateModifier.h"
#include "PhylogeneticDistanceRateModifierFunction.h"
#include "CharacterHistoryRateModifier.h"
#include "ModelVector.h"
#include "RbVector.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "RlTree.h"
#include "Natural.h"
#include "RlRateGeneratorSequence.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_phylogeneticDistanceRateModifier::Func_phylogeneticDistanceRateModifier( void ) : TypedFunction< CharacterHistoryRateModifier >( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_phylogeneticDistanceRateModifier* Func_phylogeneticDistanceRateModifier::clone( void ) const {
    
    return new Func_phylogeneticDistanceRateModifier( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::CharacterHistoryRateModifier >* Func_phylogeneticDistanceRateModifier::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree >* tree  = static_cast<const Tree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* scale = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::PhylogeneticDistanceRateModifierFunction* f = new RevBayesCore::PhylogeneticDistanceRateModifierFunction(tree, scale);
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_phylogeneticDistanceRateModifier::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "Multiplies character i by rate-multiplier r_i.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY) );
        argumentRules.push_back( new ArgumentRule( "scale", RealPos::getClassTypeSpec(), "Assigns event classes to rate matrix events.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0)) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_phylogeneticDistanceRateModifier::getClassType(void)
{
    
    static std::string revType = "Func_phylogeneticDistanceRateModifier";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_phylogeneticDistanceRateModifier::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_phylogeneticDistanceRateModifier::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnPhylogeneticDistanceRateModifier";
    
    return f_name;
}


const TypeSpec& Func_phylogeneticDistanceRateModifier::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}