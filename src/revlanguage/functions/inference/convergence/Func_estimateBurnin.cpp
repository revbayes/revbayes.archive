/**
 * @file
 * This file contains the implementation of Func_estimateBurnin, which is
 * the function used to estimate the optimal burnin length.
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
#include "Func_estimateBurnin.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlString.h"
#include "RlTrace.h"
#include "SemMin.h"
#include "Trace.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

/** Default constructor */
Func_estimateBurnin::Func_estimateBurnin( void ) : Function() {
    
}


/** Clone object */
Func_estimateBurnin* Func_estimateBurnin::clone( void ) const {
    
    return new Func_estimateBurnin( *this );
}


/** Execute function */
RevPtr<Variable> Func_estimateBurnin::execute( void ) {
    
    
    const std::vector<Trace*>& traces = static_cast<const WorkspaceVector<Trace> &>( args[0].getVariable()->getRevObject() ).getVectorRlPointer();
    const std::string&   method = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    
    std::vector<int> burnin;
    if ( method == "ESS" ) {
        RevBayesCore::EssMax estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( int(b));
        }
    } else if ( method == "SEM" ) {
        RevBayesCore::SemMin estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( int(b) );
        }
        
    } else {
        throw RbException("Method with name \"" + method + "\" not applicable.");
    }
    
    return new Variable( new ModelVector<Natural>( burnin ) );
}


/** Get argument rules */
const ArgumentRules& Func_estimateBurnin::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "traces", true, WorkspaceVector<Trace>::getClassTypeSpec() ) );
        std::vector<RlString> options;
        options.push_back( RlString("ESS") );
        options.push_back( RlString("SEM") );
        argumentRules.push_back( new OptionRule( "method", new RlString("ESS"), options ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_estimateBurnin::getClassType(void) { 
    
    static std::string rbClassName = "Func_estimateBurnin";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_estimateBurnin::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_estimateBurnin::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_estimateBurnin::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<Natural>::getClassTypeSpec();
    
    return returnTypeSpec;
}

