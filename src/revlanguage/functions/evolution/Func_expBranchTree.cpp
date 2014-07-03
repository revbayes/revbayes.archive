//
//  Func_expBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include "Func_expBranchTree.h"

#include "RealPos.h"
#include "RlTimeTree.h"
#include "Vector.h"

using namespace RevLanguage;

Func_expBranchTree::Func_expBranchTree( const Func_expBranchTree &x ) : Function(x) {}


Func_expBranchTree* Func_expBranchTree::clone( void ) const {

    return new Func_expBranchTree(*this);
}

/*
RevBayesCore::ExponentialBranchTree* ExponentialBranchTree::createFunction(void) const {
    
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in ExponentialBranchTree::createFunction");
    
    return NULL;
}
*/

/* Get argument rules */
const ArgumentRules& Func_expBranchTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
 
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, RevLanguage::TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "nodevals", true, Vector<Real>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}



/* Get class name of object */
const std::string& Func_expBranchTree::getClassName(void) {
    
    static std::string rbClassName = "Func_expBranchTree";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_expBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
const TypeSpec& Func_expBranchTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& Func_expBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevObject* Func_expBranchTree::execute() {
    
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<std::vector<double> >* val = static_cast<const Vector<Real> &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::ExponentialBranchTree* result = new RevBayesCore::ExponentialBranchTree( tau, val );

    RevBayesCore::TypedDagNode<std::vector<double> >* dag = new RevBayesCore::DeterministicNode<std::vector<double> >("",result);
    
    Vector<RealPos>* wrappedresult = new Vector<RealPos>(dag);
    
    return wrappedresult;
}


void Func_expBranchTree::printValue(std::ostream& o) const {
    
    o << " expbranchtree(";
   
    o << "tau=";
    if ( args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    
    o << "nodevals=";
    if ( args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



