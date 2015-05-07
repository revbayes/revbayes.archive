//
//  Func_branchScoreDistance.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_branchScoreDistance.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Topology.h"
#include "BranchScoreDistanceStatistic.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_branchScoreDistance::Func_branchScoreDistance( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_branchScoreDistance* Func_branchScoreDistance::clone( void ) const {
    
    return new Func_branchScoreDistance( *this );
}


RevBayesCore::TypedFunction< double >* Func_branchScoreDistance::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau2 = static_cast<const TimeTree&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::BranchScoreDistanceStatistic* f = new RevBayesCore::BranchScoreDistanceStatistic( tau, tau2 );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_branchScoreDistance::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree1", TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "tree2", TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_branchScoreDistance::getClassType(void) {
    
    static std::string revType = "Func_branchScoreDistance";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_branchScoreDistance::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_branchScoreDistance::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
