//
//  Func_treePairwiseDistances.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_treePairwiseDistances.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Topology.h"
#include "TreePairwiseDistances.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_treePairwiseDistances::Func_treePairwiseDistances( void ) : TypedFunction< RevBayesCore::MatrixReal >( )
{
    
}


/** Clone object */
Func_treePairwiseDistances* Func_treePairwiseDistances::clone( void ) const {
    
    return new Func_treePairwiseDistances( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_treePairwiseDistances::createFunction( void ) const
{
    
   // RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
	
	RevBayesCore::TreePairwiseDistances* f = NULL;
	
	tau ;
	
	if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( TimeTree::getClassTypeSpec() ) )
	{
		RevBayesCore::TypedDagNode < RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
		f = new RevBayesCore::TreePairwiseDistances( tau );
	}
	else if ( this->args[0].getVariable()->getRequiredTypeSpec().isDerivedOf( BranchLengthTree::getClassTypeSpec() ) )
	{
		tau = static_cast<const BranchLengthTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
		f = new RevBayesCore::TreePairwiseDistances( tau );
	}
	
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_treePairwiseDistances::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
		std::vector<TypeSpec> treeTypes;
		treeTypes.push_back( TimeTree::getClassTypeSpec() );
		treeTypes.push_back( BranchLengthTree::getClassTypeSpec() );
		
		
		argumentRules.push_back( new ArgumentRule( "tree", treeTypes , ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_treePairwiseDistances::getClassType(void) {
    
    static std::string revType = "Func_treePairwiseDistances";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_treePairwiseDistances::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_treePairwiseDistances::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
