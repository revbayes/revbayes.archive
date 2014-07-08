/**
 * @file
 * This file contains the implementation of Func_beca, which is
 * the function used to do Burnin Estimation and Convergence Assessment (BECA)
 *
 * @brief Implementation of Func_beca
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "EssMax.h"
#include "Func_beca.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTrace.h"
#include "SemMin.h"
#include "Trace.h"
#include "TypeSpec.h"
#include "VectorRlPointer.h"


using namespace RevLanguage;

/** Default constructor */
Func_beca::Func_beca( void ) : Function() {
    
}


/** Clone object */
Func_beca* Func_beca::clone( void ) const {
    
    return new Func_beca( *this );
}


/** Execute function */
RevPtr<Variable> Func_beca::execute( void ) {
    
    bool converged = false;
    
    const std::string&   fn     = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string&   method = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::vector<Trace*>& traces = static_cast<const VectorRlPointer<Trace> &>( args[2].getVariable()->getRevObject() ).getValue();
    
    std::ostream* outStream = NULL;
    if ( fn != "" ) 
    {
        std::ofstream *o = new std::ofstream();
        
        // open the stream to the file
        o->open(fn.c_str(), std::fstream::out);
        
        outStream = o;
    }
    else 
    {
        outStream = &std::cout;
    }
    
    std::vector<size_t> burnin;
    if ( method == "ESS" ) 
    {
        RevBayesCore::EssMax estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) 
        {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( b );
        }
    } 
    else if ( method == "SEM" ) 
    {
        RevBayesCore::SemMin estimator;
        for ( std::vector<Trace*>::const_iterator it = traces.begin(); it != traces.end(); ++it ) 
        {
            const std::vector<double> &values = (*it)->getValue().getValues();
            size_t b = estimator.estimateBurnin( values );
            burnin.push_back( b );
        }
        
    } 
    else 
    {
        if ( fn == "" ) 
        {
            static_cast<std::ofstream*>(outStream)->close();
            delete outStream;
        }
        
        throw RbException("Method with name \"" + method + "\" not applicable.");
    }
    
    // if we had a file stream hope, let's close it
    if ( fn == "" ) 
    {
        static_cast<std::ofstream*>(outStream)->close();
        delete outStream;
    }
    
    return new Variable( new RlBoolean( converged ) );
}


/** Get argument rules */
const ArgumentRules& Func_beca::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "filename", true, RlString::getClassTypeSpec(), new RlString("") ) );
        Vector<RlString> options;
        options.push_back( RlString("ESS") );
        options.push_back( RlString("SEM") );
        argumentRules.push_back( new OptionRule( "method", new RlString("ESS"), options ) );
        argumentRules.push_back( new ArgumentRule( "traces", true, VectorRlPointer<Trace>::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis( VectorRlPointer<Trace>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_beca::getClassName(void) { 
    
    static std::string rbClassName = "Func_beca";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_beca::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_beca::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_beca::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<RlBoolean>::getClassTypeSpec();
    
    return returnTypeSpec;
}

