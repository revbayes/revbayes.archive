/**
 * @file
 * This file contains the implementation of the class
 * Func_powermix, which is used to compute a
 * power mixture of two simplex parameters.
 *
 * @brief Implementation of Func_powermix
 *
 * (c) Copyright under GPL version 3
 */

#include "DeterministicNode.h"
#include "PowermixFunction.h"
#include "Func_powermix.h"
#include "Real.h"
#include "RealPos.h"
#include "Simplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_powermix::Func_powermix( void ) : Function( ) {
    
}


/** Clone object */
Func_powermix* Func_powermix::clone( void ) const {
    
    return new Func_powermix( *this );
}


RbLanguageObject* Func_powermix::execute() {
    
    std::vector<const RevBayesCore::TypedDagNode< std::vector<double> >* >  params;
    for ( size_t i = 0; i < args.size(); i++ ) {
        const RevBayesCore::TypedDagNode< std::vector<double> >* val = static_cast<const Simplex &> ( args[i].getVariable()->getValue() ).getValueNode();
        params.push_back( val );
    }
    
    RevBayesCore::PowermixFunction *func = new RevBayesCore::PowermixFunction( params );

    RevBayesCore::TypedDagNode<std::vector<double> > *detNode = new RevBayesCore::DeterministicNode<std::vector<double> >("", func);
    Simplex *theSimplex = new Simplex( detNode );
    
    return theSimplex;
}


/* Get argument rules */
const ArgumentRules& Func_powermix::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "a", true, Simplex::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "b", true, Simplex::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "mixprop", true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_powermix::getClassName(void) {
    
    static std::string rbClassName = "powermix";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_powermix::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Get return type */
const TypeSpec& Func_powermix::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Simplex::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_powermix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
