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
#include "Natural.h"
#include "RealPos.h"
#include "Probability.h"
#include "RlMultivariateRealNodeValTree.h"
#include "RlTimeTree.h"

using namespace RevLanguage;


Func_tanhBranchTree* Func_tanhBranchTree::clone( void ) const {

    return new Func_tanhBranchTree(*this);
}


/* Get argument rules */
const ArgumentRules& Func_tanhBranchTree::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
 
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree"      , RevLanguage::TimeTree::getClassTypeSpec()                   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "process"   , RevLanguage::MultivariateRealNodeValTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "offset"    , Real::getClassTypeSpec()                                    , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0)) );
        argumentRules.push_back( new ArgumentRule( "traitindex", Integer::getClassTypeSpec()                                 , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Natural(0) ) );
        
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



const TypeSpec& Func_tanhBranchTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_tanhBranchTree::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tau = static_cast<const TimeTree &>( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::MultivariateRealNodeContainer >* process = static_cast<const MultivariateRealNodeValTree &>( args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< double >* offset = static_cast<const Real &>( args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* traitindex = static_cast<const Integer &>( args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::HyperbolicTangentBranchTree* result = new RevBayesCore::HyperbolicTangentBranchTree( tau, process, offset, traitindex );

    
    return result;
}


void Func_tanhBranchTree::printValue(std::ostream& o) const {
    
    o << " tanhbranchtree(";
   
    o << "tree=";
    if ( argsProcessed && args[0].getVariable() != NULL ) {
        o << args[0].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "process=";
    if ( argsProcessed && args[1].getVariable() != NULL ) {
        o << args[1].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "offset=";
    if ( argsProcessed && args[2].getVariable() != NULL ) {
        o << args[2].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ", ";
    
    o << "traitindex=";
    if ( argsProcessed && args[3].getVariable() != NULL ) {
        o << args[3].getVariable()->getName();
    } else {
        o << "?";
    }
    o << ")";
}



