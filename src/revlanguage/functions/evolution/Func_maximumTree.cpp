//
//  Func_maximumTree.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_maximumTree.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"
#include "ModelVector.h"
#include "Topology.h"
#include "MaximumTreeFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_maximumTree::Func_maximumTree( void ) : Function( ) {
    
}


/** Clone object */
Func_maximumTree* Func_maximumTree::clone( void ) const {
    
    return new Func_maximumTree( *this );
}


RevPtr<Variable> Func_maximumTree::execute() {
    
   // RevBayesCore::TypedDagNode< RbVector<TimeTree> >* gTrees = static_cast<const ModelVector<TimeTree> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();

    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector< RevBayesCore::TimeTree > >* gTrees = static_cast<const ModelVector< TimeTree > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::MaximumTreeFunction* f = new RevBayesCore::MaximumTreeFunction( gTrees );
    
    DeterministicNode<RevBayesCore::TimeTree> *detNode = new DeterministicNode<RevBayesCore::TimeTree>("", f, this->clone());
    
    TimeTree* value = new TimeTree( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_maximumTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "geneTrees", ModelVector< TimeTree >::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_maximumTree::getClassType(void) {
    
    static std::string revType = "Func_maximumTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_maximumTree::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_maximumTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = TimeTree::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_maximumTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
