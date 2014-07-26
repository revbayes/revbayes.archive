/* 
 * File:   Func_tanhBranchTree.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 19:56
 */

#include "Func_tanhBranchTree.h"
#include "HyberpolicTangentBranchTree.h"

#include "Integer.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlMultivariateRealNodeValTree.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

Func_tanhBranchTree::Func_tanhBranchTree( const Func_tanhBranchTree &x ) : Function(x) {}


Func_tanhBranchTree* Func_tanhBranchTree::clone( void ) const {

    return new Func_tanhBranchTree(*this);
}

/*
RevBayesCore::HyperbolicTangentBranchTree* HyperbolicTangentBranchTree::createFunction(void) const {
    
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in HyperbolicTangentBranchTree::createFunction");
    
    return NULL;
}
*/

/* Get argument rules */
const ArgumentRules& Func_tanhBranchTree::getArgumentRules( void ) const {
    
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
const std::string& Func_tanhBranchTree::getClassType(void) {
    
    static std::string revType = "Func_tanhBranchTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_tanhBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_tanhBranchTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& Func_tanhBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevPtr<Variable> Func_tanhBranchTree::execute() {
    
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::MultivariateRealNodeContainer >* process = static_cast<const MultivariateRealNodeValTree &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* offset = static_cast<const Real &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* traitindex = static_cast<const Integer &>( args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::HyperbolicTangentBranchTree* result = new RevBayesCore::HyperbolicTangentBranchTree( tau, process, offset, traitindex );

    DeterministicNode<std::vector<double> >* dag = new DeterministicNode<std::vector<double> >("", result, this->clone());
    
    ModelVector<RealPos>* wrappedresult = new ModelVector<RealPos>(dag);
    
    return new Variable( wrappedresult );
}


void Func_tanhBranchTree::printValue(std::ostream& o) const {
    
    o << " tanhbranchtree(";
   
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



