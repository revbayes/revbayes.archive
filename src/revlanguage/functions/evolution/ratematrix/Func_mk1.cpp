#include "Func_mk1.h"
//
//  Func_mk1.cpp
//  RevBayesCore
//
//  Will Freyman
//

#include "Mk1RateMatrixFunction.h"
#include "Func_mk1.h"
#include "RateMatrix_Mk1.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mk1::Func_mk1( void ) : Function( ) {
    
}


/** Clone object */
Func_mk1* Func_mk1::clone( void ) const {
    
    return new Func_mk1( *this );
}


RevPtr<Variable> Func_mk1::execute() {
    
    
    RevBayesCore::TypedDagNode< int >* k = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* la = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
	
    RevBayesCore::Mk1RateMatrixFunction* f = new RevBayesCore::Mk1RateMatrixFunction( k, la );
    
    DeterministicNode<RevBayesCore::RateMatrix> *detNode = new DeterministicNode<RevBayesCore::RateMatrix>("", f, this->clone());
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mk1::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
		
        argumentRules.push_back( new ArgumentRule( "k"				, Natural::getClassTypeSpec()          , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "lambda"          , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mk1::getClassType(void) {
    
    static std::string revType = "Func_mk1";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mk1::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_mk1::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mk1::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
