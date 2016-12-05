//
//  Func_TreeTrace.cpp
//  RevBayes_development_branch
//
//  Created by will freyman on 12/15/15.
//  Copyright (c) 2015 will freyman. All rights reserved.
//



#include "Func_treeTrace.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlTree.h"
#include "RlTraceTree.h"
#include "TraceTree.h"
#include "TypedDagNode.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;


Func_treeTrace* Func_treeTrace::clone( void ) const
{
    
    return new Func_treeTrace( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_treeTrace::execute( void ) {
    
    const ModelVector<RevLanguage::TimeTree>&         ttrees = static_cast<const ModelVector<RevLanguage::TimeTree>&>(         args[0].getVariable()->getRevObject() );
    const ModelVector<RevLanguage::BranchLengthTree>& btrees = static_cast<const ModelVector<RevLanguage::BranchLengthTree>&>( args[1].getVariable()->getRevObject() );
    
    if ( ttrees != RevNullObject::getInstance() )
    {
        RevBayesCore::TraceTree t = RevBayesCore::TraceTree( true );
    
        for (size_t i = 0; i < ttrees.size(); ++i)
        {
            t.addObject( new RevBayesCore::Tree( const_cast<RevBayesCore::Tree&>(ttrees[i]) ) );
        }
    
        return new RevVariable( new TraceTree( t ) );
    }
    else if ( btrees != RevNullObject::getInstance() )
    {
        RevBayesCore::TraceTree t = RevBayesCore::TraceTree( false );
    
        for (size_t i = 0; i < btrees.size(); ++i)
        {
            t.addObject( new RevBayesCore::Tree( const_cast<RevBayesCore::Tree&>(btrees[i]) ) );
        }
        
        return new RevVariable( new TraceTree( t ) );
    }
    throw RbException("treeTrace requires either a vector of TimeTrees or BranchLengthTrees.");
}


/** Get argument rules */
const ArgumentRules& Func_treeTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        argumentRules.push_back( new ArgumentRule( "time_trees",          ModelVector<TimeTree>::getClassTypeSpec(),         "Vector of TimeTrees.",         ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "branch_length_trees", ModelVector<BranchLengthTree>::getClassTypeSpec(), "Vector of BranchLengthTrees.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_treeTrace::getClassType(void)
{
    
    static std::string revType = "Func_treeTrace";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_treeTrace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_treeTrace::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "treeTrace";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_treeTrace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_treeTrace::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TraceTree::getClassTypeSpec();
    return returnTypeSpec;
}