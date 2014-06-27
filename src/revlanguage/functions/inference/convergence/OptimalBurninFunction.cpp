/**
 * @file
 * This file contains the implementation of Func_type, which is
 * the function used to get the type of a variable.
 *
 * @brief Implementation of Func_quit
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2012-09-07
 *
 * $Id: Func_source.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "EssMax.h"
#include "OptimalBurninFunction.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlString.h"
#include "RlTrace.h"
#include "SemMin.h"
#include "Trace.h"
#include "TypeSpec.h"
#include "VectorRlPointer.h"


using namespace RevLanguage;

/** Default constructor */
OptimalBurninFunction::OptimalBurninFunction( void ) : Function() {
    
}


/** Clone object */
OptimalBurninFunction* OptimalBurninFunction::clone( void ) const {
    
    return new OptimalBurninFunction( *this );
}


/** Execute function */
RevObject* OptimalBurninFunction::execute( void ) {
    
    
    const std::vector<Trace*>& traces = static_cast<const VectorRlPointer<Trace> &>( args[0].getVariable()->getValue() ).getValue();
    const std::string&   method = static_cast<const RlString&>( args[1].getVariable()->getValue() ).getValue();
    
    std::vector<int> burnin;
    if ( method == "ESS" ) {
        EssMax estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( int(b));
        }
    } else if ( method == "SEM" ) {
        SemMin estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( int(b) );
        }
        
    } else {
        throw RbException("Method with name \"" + method + "\" not applicable.");
    }
    
    return new Vector<Natural>( burnin );
}


/** Get argument rules */
const ArgumentRules& OptimalBurninFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "traces", true, VectorRlPointer<Trace>::getClassTypeSpec() ) );
        Vector<RlString> options;
        options.push_back( RlString("ESS") );
        options.push_back( RlString("SEM") );
        argumentRules.push_back( new OptionRule( "method", new RlString("ESS"), options ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& OptimalBurninFunction::getClassName(void) { 
    
    static std::string rbClassName = "optimal-burnin function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& OptimalBurninFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& OptimalBurninFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& OptimalBurninFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<Natural>::getClassTypeSpec();
    
    return returnTypeSpec;
}

