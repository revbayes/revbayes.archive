/**
 * @file
 * This file contains the implementation of DistributionFunction, which
 * is used for functions related to a statistical distribution.
 *
 * @brief Implementation of DistributionFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "RbBoolean.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistributionContinuous.h"
#include "DistributionFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
DistributionFunction::DistributionFunction( Distribution* dist, FuncType funcType ) : RbFunction(), returnType( funcType == DENSITY || funcType == PROB ? TypeSpec( funcType == DENSITY ? Real::getClassTypeSpec() : RealPos::getClassTypeSpec() ) : dist->getVariableType() ) {

    /* Ininitalize the argument rules */
    argumentRules = new ArgumentRules();
    
    /* Set the distribution */
    distribution = dist;

    /* Set the function type */
    functionType = funcType;

    /* Get the distribution parameter rules and set type to value argument */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for ( std::vector<ArgumentRule*>::const_iterator i = memberRules.begin(); i != memberRules.end(); i++ ) {
        // check if this rule has a default value
        if ((*i)->hasDefault()) {
            argumentRules->push_back( new ValueRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec(), (*i)->getDefaultVariable().getValue().clone() ) );
        } else {
            argumentRules->push_back( new ValueRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec() ) );
        }
    }

    /* Modify argument rules based on function type */
    if (functionType == DENSITY) {

        argumentRules->insert( argumentRules->begin(), new ValueRule( "x"  , distribution->getVariableType() ) );
        argumentRules->push_back(                      new ValueRule( "log", new RbBoolean(false)            ) );
    }
    else if (functionType == RVALUE) {

        // Nothing to do
    }
    else if (functionType == PROB) {

        argumentRules->insert( argumentRules->begin(), new ValueRule( "q"  , distribution->getVariableType() ) );
    }
    else if (functionType == QUANTILE) {

        argumentRules->insert( argumentRules->begin(), new ValueRule( "p"  , RealPos::getClassTypeSpec()     ) );
    }
}


/** Copy constructor */
DistributionFunction::DistributionFunction( const DistributionFunction& x ) : RbFunction(x), returnType( x.returnType ) {

    // copy the argument rules
    argumentRules = new ArgumentRules();
    for (std::vector<ArgumentRule* >::const_iterator it = x.argumentRules->begin(); it != x.argumentRules->end(); it++) {
        argumentRules->push_back( (*it)->clone() );
    }
    
    distribution  = x.distribution->clone();
    functionType  = x.functionType;

    /* Modify argument rules based on function type */
    if (functionType == DENSITY) {
        
        argumentRules->insert( argumentRules->begin(), new ValueRule( "x"  , distribution->getVariableType() ) );
        argumentRules->push_back(                      new ValueRule( "log", new RbBoolean(false)            ) );
    }
    else if (functionType == RVALUE) {
        
        // Nothing to do
    }
    else if (functionType == PROB) {
        
        argumentRules->insert( argumentRules->begin(), new ValueRule( "q"  , distribution->getVariableType() ) );
    }
    else if (functionType == QUANTILE) {
        
        argumentRules->insert( argumentRules->begin(), new ValueRule( "p"  , RealPos::getClassTypeSpec()     ) );
    }
    
    // copy the arguments
    this->x = x.x;
    log     = x.log;
    p       = x.p;
    q       = x.q;
}


/** Destructor */
DistributionFunction::~DistributionFunction(void) {
    delete distribution;
    if (argumentRules != NULL) {
        delete argumentRules;
    }
    
}


/** Assignment operator */
DistributionFunction& DistributionFunction::operator=( const DistributionFunction& x ) {

    if (this != &x) {
        
        // call the assignment operator of the base class
        this->RbFunction::operator=(x);
        
        if ( returnType != x.returnType )
            throw RbException( "Invalid assignment involving distributions on different types of random variables" );
        
        // free memory
        if (argumentRules != NULL) {
            delete argumentRules;
        }
        
        // copy the argument rules
        argumentRules = new ArgumentRules();
        for (std::vector<ArgumentRule* >::const_iterator it = x.argumentRules->begin(); it != x.argumentRules->end(); it++) {
            argumentRules->push_back( (*it)->clone() );
        }
        
        // free memory
        if (distribution != NULL) {
            delete distribution;
        }
        distribution  = x.distribution->clone();
        functionType  = x.functionType;

        /* Modify argument rules based on function type */
        if (functionType == DENSITY) {
            
            argumentRules->insert( argumentRules->begin(), new ValueRule( "x"  , distribution->getVariableType() ) );
            argumentRules->push_back(                      new ValueRule( "log", new RbBoolean(false)            ) );
        }
        else if (functionType == RVALUE) {
            
            // Nothing to do
        }
        else if (functionType == PROB) {
            
            argumentRules->insert( argumentRules->begin(), new ValueRule( "q"  , distribution->getVariableType() ) );
        }
        else if (functionType == QUANTILE) {
            
            argumentRules->insert( argumentRules->begin(), new ValueRule( "p"  , RealPos::getClassTypeSpec()     ) );
        }
        
        // Copy the arguments
        this->x = x.x;
        log     = x.log;
        p       = x.p;
        q       = x.q;
    }

    return (*this);
}


/** Clone object */
DistributionFunction* DistributionFunction::clone( void ) const {

    return new DistributionFunction(*this);
}


/** Execute operation: switch based on type */
const RbLanguageObject& DistributionFunction::executeFunction( void ) {

    if ( functionType == DENSITY ) {

        if ( static_cast<const RbBoolean&>( log->getValue() ).getValue() == false )
            density.setValue( distribution->pdf  ( x->getValue() ) );
        else
            density.setValue( distribution->lnPdf( x->getValue() ) );
        return density;
    }
    else if (functionType == RVALUE) {

        const RbLanguageObject& draw = distribution->rv();
        
        return draw;
    }
    else if (functionType == PROB) {
        cd.setValue( static_cast<DistributionContinuous*>( distribution )->cdf( q->getValue() ) );
        return cd;
    }
    else if (functionType == QUANTILE) {

        double    prob  = static_cast<const RealPos&>( p->getValue() ).getValue();
        const RbLanguageObject& quant = static_cast<DistributionContinuous*>( distribution )->quantile( prob );
        
        return quant;
    }

    throw RbException( "Unrecognized distribution function" );
}


/** Get argument rules */
const ArgumentRules& DistributionFunction::getArgumentRules(void) const {

    return *argumentRules;
}


/** Get class name of object */
const std::string& DistributionFunction::getClassName(void) { 
    
    static std::string rbClassName = "Distribution function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DistributionFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunction::getReturnType(void) const {

    return returnType;
}


/** Process arguments */
void DistributionFunction::processArguments( const std::vector<Argument>& args ) {

    // delegate first to the base class
    RbFunction::processArguments( args );

//    /* Set member variables of the distribution */
//    size_t i = 0;
//    if ( functionType != RVALUE )
//        i++;    // Add one because first argument is not a distribution parameter
//    size_t k = argumentRules->size();
//    if ( functionType == DENSITY )
//        k--;    // Subtract one because last argumet is log
//
//    for ( ; i < k; i++ ) {
//
//        std::string name = (*argumentRules)[i].getArgumentLabel();
//
//        const RbVariablePtr& arg = this->args[i].getVariablePtr();
//        
//        // TODO: Work on proper type checking
////        arg->setValueTypeSpec( (*argumentRules)[i].getArgumentTypeSpec());
//        
//        distribution->setMember( name, arg );
//    }
    
    // Set member variables of the distribution
    for (std::vector<Argument>::iterator i = this->args.begin(); i != this->args.end(); i++) {
        Argument& theArg = *i;
        std::string label = theArg.getLabel();
        if ( label != "x" && label != "p" && label != "q" && label != "log" ) {
            distribution->setMember( label, theArg.getVariablePtr() );
        }
    }
}


/** We catch here the setting of the argument variables to store our parameters. */
void DistributionFunction::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "log") {
        log = var;
    }
    else if ( name == "p") {
        p = var;
    }
    else if ( name == "q") {
        q = var;
    }
    
    // We expect a couple of parameters which we need to add to the distribution. Therefore we do not call the base class.
//    else {
//        RbFunction::setArgumentVariable(name, var);
//    }
}

