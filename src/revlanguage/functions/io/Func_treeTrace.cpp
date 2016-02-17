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
    
    const ModelVector<RevLanguage::Tree>&  trees       = static_cast<const ModelVector<RevLanguage::Tree>&>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    bool clock = true;
    RevBayesCore::TraceTree t = RevBayesCore::TraceTree( clock );
    
    for (size_t i = 0; i < trees.size(); ++i)
    {
        t.addObject( new RevBayesCore::Tree( const_cast<RevBayesCore::Tree&>(trees[i]) ) );
    }
    
    return new RevVariable( new TraceTree( t ) );
}


/** Get argument rules */
const ArgumentRules& Func_treeTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        argumentRules.push_back( new ArgumentRule( "trees"     , ModelVector<TimeTree>::getClassTypeSpec(), "Vector of TimeTrees.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
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
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
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
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_treeTrace::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TraceTree::getClassTypeSpec();
    return returnTypeSpec;
}