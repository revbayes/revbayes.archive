//
//  Func_expBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include "Func_expBranchTree.h"

#include "Integer.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlMultivariateRealNodeValTree.h"
#include "RlTimeTree.h"

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
        argumentRules.push_back( new ArgumentRule( "process", true, RevLanguage::MultivariateRealNodeValTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "offset", true, Real::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "traitindex", true, Integer::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}



/* Get Rev type of object */
const std::string& Func_expBranchTree::getClassType(void) {
    
    static std::string revType = "Func_expBranchTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_expBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_expBranchTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& Func_expBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevPtr<Variable> Func_expBranchTree::execute() {
    
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::MultivariateRealNodeContainer >* process = static_cast<const MultivariateRealNodeValTree &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* offset = static_cast<const Real &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* traitindex = static_cast<const Integer &>( args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::ExponentialBranchTree* result = new RevBayesCore::ExponentialBranchTree( tau, process, offset, traitindex );

    DeterministicNode<std::vector<double> >* dag = new DeterministicNode<std::vector<double> >("", result, this->clone());
    
    ModelVector<RealPos>* wrappedresult = new ModelVector<RealPos>(dag);
    
    return new Variable( wrappedresult );
}


void Func_expBranchTree::printValue(std::ostream& o) const {
    
    o << " expbranchtree(";
   
    o << "tree=";
    if ( args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "process=";
    if ( args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "offset=";
    if ( args[2].getVariable() != NULL ) {
        o << args[2].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "traitindex=";
    if ( args[3].getVariable() != NULL ) {
        o << args[3].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



