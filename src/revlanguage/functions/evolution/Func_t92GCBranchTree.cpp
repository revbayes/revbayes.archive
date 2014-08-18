/* 
 * File:   Func_t92GCBranchTree.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 20:59
 */

#include "T92GCBranchTree.h"
#include "Func_t92GCBranchTree.h"

#include "ModelVectorAbstractElement.h"
#include "Probability.h"
#include "RealPos.h"
#include "RbVector.h"
#include "RlTimeTree.h"
#include "RlRateMatrix.h"

using namespace RevLanguage;

Func_t92GCBranchTree::Func_t92GCBranchTree( const Func_t92GCBranchTree &x ) : Function(x) {}


Func_t92GCBranchTree* Func_t92GCBranchTree::clone( void ) const {

    return new Func_t92GCBranchTree(*this);
}

/* Get argument rules */
const ArgumentRules& Func_t92GCBranchTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
 
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, RevLanguage::TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "gctree", true, RevLanguage::ModelVector<Probability>::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "rootgc", true, Probability::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "kappa", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}



/* Get Rev type of object */
const std::string& Func_t92GCBranchTree::getClassType(void) {
    
    static std::string revType = "Func_t92GCBranchTree";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_t92GCBranchTree::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_t92GCBranchTree::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<RateMatrix>::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& Func_t92GCBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevPtr<Variable> Func_t92GCBranchTree::execute() {
    
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< std::vector<double> >* gcprocess = static_cast<const ModelVector<Probability> &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* rootgc = static_cast<const Probability &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* kappa = static_cast<const RealPos &>( args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::T92GCBranchTree* result = new RevBayesCore::T92GCBranchTree( tau, gcprocess, rootgc, kappa );

    DeterministicNode<RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* dag = new DeterministicNode<RevBayesCore::RbVector<RevBayesCore::RateMatrix> >("", result, this->clone());
    
    ModelVectorAbstractElement<RateMatrix>* wrappedresult = new ModelVectorAbstractElement<RateMatrix>( dag );

    return new Variable( wrappedresult );
    
//    return NULL;
}


void Func_t92GCBranchTree::printValue(std::ostream& o) const {
    
    o << " t92GCbranchtree(";
   
    o << "tree=";
    if ( args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "gctree=";
    if ( args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "rootgc=";
    if ( args[2].getVariable() != NULL ) {
        o << args[2].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "kappa=";
    if ( args[3].getVariable() != NULL ) {
        o << args[3].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



