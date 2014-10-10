/* 
 * File:   Func_t92GCBranchTree.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 20:59
 */

#include "T92GCBranchTree.h"
#include "Func_t92GCBranchTree.h"
#include "ModelVector.h"
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
        
        argumentRules.push_back( new ArgumentRule( "tree"    , RevLanguage::TimeTree::getClassTypeSpec()                , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "branchGC", RevLanguage::ModelVector<Probability>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "tstv"    , RealPos::getClassTypeSpec()                              , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
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
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* gcprocess = static_cast<const ModelVector<Probability> &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* kappa = static_cast<const RealPos &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::T92GCBranchTree* result = new RevBayesCore::T92GCBranchTree( tau, gcprocess, kappa );

    DeterministicNode<RevBayesCore::RbVector<RevBayesCore::RateMatrix> >* dag = new DeterministicNode<RevBayesCore::RbVector<RevBayesCore::RateMatrix> >("", result, this->clone());
    
    ModelVector<RateMatrix>* wrappedresult = new ModelVector<RateMatrix>( dag );

    return new Variable( wrappedresult );
}


void Func_t92GCBranchTree::printValue(std::ostream& o) const {
    
    o << " t92GCbranchtree(";
   
    o << "tree=";
    if ( argsProcessed && args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "branchGC=";
    if ( argsProcessed && args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "tstv=";
    if ( argsProcessed && args[2].getVariable() != NULL ) {
        o << args[3].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



