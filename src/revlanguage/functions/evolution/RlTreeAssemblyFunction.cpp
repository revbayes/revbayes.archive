//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "TreeAssemblyFunction.h"
#include "RlTreeAssemblyFunction.h"
#include "RateMatrix.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBranchLengthTree.h"
#include "RlTopology.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
TreeAssemblyFunction::TreeAssemblyFunction( void ) : Function( ) {
    
}


/** Clone object */
TreeAssemblyFunction* TreeAssemblyFunction::clone( void ) const {
    
    return new TreeAssemblyFunction( *this );
}


RbLanguageObject* TreeAssemblyFunction::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::Topology>* tau = static_cast<const Topology&>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* brlens = static_cast<const Vector<RealPos> &>( this->args[1].getVariable()->getValue() ).getValueNode();
    RevBayesCore::TreeAssemblyFunction* f = new RevBayesCore::TreeAssemblyFunction( tau, brlens );
    RevBayesCore::DeterministicNode<RevBayesCore::BranchLengthTree> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::BranchLengthTree>("", f);
    
    BranchLengthTree* value = new BranchLengthTree( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& TreeAssemblyFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "topology", true, Topology::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "brlens", true, Vector<RealPos>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& TreeAssemblyFunction::getClassName(void) { 
    
    static std::string rbClassName = "Tree-Assembly";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& TreeAssemblyFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& TreeAssemblyFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = BranchLengthTree::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& TreeAssemblyFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
