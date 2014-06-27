//
//  RlExponentialBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include "RlExponentialBranchTree.h"

#include "RealPos.h"
#include "Vector.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

ExponentialBranchTree::ExponentialBranchTree( const ExponentialBranchTree &d ) : Function(d) {}

/*
ExponentialBranchTree::~ExponentialBranchTree() {
    
}
*/

ExponentialBranchTree* ExponentialBranchTree::clone( void ) const {

    return new ExponentialBranchTree(*this);
}

/*
RevBayesCore::ExponentialBranchTree* ExponentialBranchTree::createFunction(void) const {
    
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in ExponentialBranchTree::createFunction");
    
    return NULL;
}
*/

/* Get argument rules */
const ArgumentRules& ExponentialBranchTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
 
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "nodevals", true, Vector<Real>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}



/* Get class name of object */
const std::string& ExponentialBranchTree::getClassName(void) {
    
    static std::string rbClassName = "ExponentialBranchTree";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& ExponentialBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedFunction<Vector<RealPos> >::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
const TypeSpec& ExponentialBranchTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& ExponentialBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevObject* ExponentialBranchTree::execute() {
    
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( args[0].getVariable()->getValue() ).getValueNode();
    
    RevBayesCore::TypedDagNode<std::vector<double> >* val = static_cast<const Vector<Real> &>( args[1].getVariable()->getValue() ).getValueNode();

    RevBayesCore::ExponentialBranchTree* result = new RevBayesCore::ExponentialBranchTree( tau, val );

    RevBayesCore::TypedDagNode<std::vector<double> >* dag = new RevBayesCore::DeterministicNode<std::vector<double> >("",result);
    
    Vector<RealPos>* wrappedresult = new Vector<RealPos>(dag);
    
    return wrappedresult;
}


void ExponentialBranchTree::printValue(std::ostream& o) const {
    
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



