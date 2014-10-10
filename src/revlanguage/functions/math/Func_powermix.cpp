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

#include "Func_powermix.h"
#include "PowermixFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_powermix::Func_powermix( void ) : Function( ) {
    
}


/** Clone object */
Func_powermix* Func_powermix::clone( void ) const {
    
    return new Func_powermix( *this );
}


RevPtr<Variable> Func_powermix::execute() {
    
    std::vector<const RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* >  params;
    for ( size_t i = 0; i < args.size(); i++ ) {
        const RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* val = static_cast<const Simplex &> ( args[i].getVariable()->getRevObject() ).getDagNode();
        params.push_back( val );
    }
    
    RevBayesCore::PowermixFunction *func = new RevBayesCore::PowermixFunction( params );

    DeterministicNode<RevBayesCore::RbVector<double> > *detNode = new DeterministicNode<RevBayesCore::RbVector<double> >("", func, this->clone());

    Simplex *theSimplex = new Simplex( detNode );
    
    return new Variable( theSimplex );
}


/* Get argument rules */
const ArgumentRules& Func_powermix::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "a"      , Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "b"      , Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "mixprop", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_powermix::getClassType(void) {
    
    static std::string revType = "Func_powermix";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_powermix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
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
