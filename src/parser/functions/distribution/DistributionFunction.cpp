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
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistributionContinuous.h"
#include "DistributionFunction.h"
#include "Distribution.h"
#include "ParserDistributionContinuous.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
DistributionFunction::DistributionFunction( const RbPtr<ParserDistribution> &dist ) : RbFunction() {

    /* Ininitalize the argument rules */
    argumentRules = new ArgumentRules();
    
    /* Set the distribution */
    distribution = dist;

    /* Get the distribution parameter rules and set type to value argument */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for ( std::vector<ArgumentRule*>::const_iterator i = memberRules.begin(); i != memberRules.end(); i++ ) {
        // check if this rule has a default value
        if ((*i)->hasDefault()) {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec(), (*i)->getDefaultVariable().getValue().clone() ) );
        } else {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec() ) );
        }
    }

    /* Modify argument rules based on function type */
    if (functionType == DENSITY) {

        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "x"  , distribution->getVariableType() ) );
        argumentRules->push_back(                      new ConstArgumentRule( "log", new RbBoolean(false)            ) );
    }
    else if (functionType == RVALUE) {

        // Nothing to do
    }
    else if (functionType == PROB) {

        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "q"  , distribution->getVariableType() ) );
    }
    else if (functionType == QUANTILE) {

        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , RealPos::getClassTypeSpec()     ) );
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

    /* Modify argument rules based on function type */
    if (functionType == DENSITY) {
        
        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "x"  , distribution->getVariableType() ) );
        argumentRules->push_back(                      new ConstArgumentRule( "log", new RbBoolean(false)            ) );
    }
    else if (functionType == RVALUE) {
        
        // Nothing to do
    }
    else if (functionType == PROB) {
        
        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "q"  , distribution->getVariableType() ) );
    }
    else if (functionType == QUANTILE) {
        
        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , RealPos::getClassTypeSpec()     ) );
    }
}


/** Assignment operator */
DistributionFunction& DistributionFunction::operator=( const DistributionFunction& x ) {

    if (this != &x) {
        
        // call the assignment operator of the base class
        this->RbFunction::operator=(x);
        
        if ( returnType != x.returnType )
            throw RbException( "Invalid assignment involving distributions on different types of random variables" );
        
        
        // copy the argument rules
        argumentRules = new ArgumentRules();
        for (std::vector<ArgumentRule* >::const_iterator it = x.argumentRules->begin(); it != x.argumentRules->end(); it++) {
            argumentRules->push_back( (*it)->clone() );
        }
        
        distribution  = x.distribution->clone();

        /* Modify argument rules based on function type */
        if (functionType == DENSITY) {
            
            argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "x"  , distribution->getVariableType() ) );
            argumentRules->push_back(                      new ConstArgumentRule( "log", new RbBoolean(false)            ) );
        }
        else if (functionType == RVALUE) {
            
            // Nothing to do
        }
        else if (functionType == PROB) {
            
            argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "q"  , distribution->getVariableType() ) );
        }
        else if (functionType == QUANTILE) {
            
            argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , RealPos::getClassTypeSpec()     ) );
        }
    
    }

    return (*this);
}


/** Clone object */
DistributionFunction* DistributionFunction::clone( void ) const {

    return new DistributionFunction(*this);
}


/** Execute operation: switch based on type */
RbPtr<RbLanguageObject> DistributionFunction::executeFunction(const std::vector<const RbObject *> &args) {
    
    if ( functionType == DENSITY ) {
        // converting the arguments into atomic data types
        std::vector<RbValue<void*> > newArgs;
        // skip the first and the last parameter
        for (std::vector<const RbObject*>::const_iterator i = args.begin()+1; i+1 != args.end(); ++i) {
            RbValue<void*> arg;
            arg.value = (*i)->getLeanValue(arg.lengths);
            newArgs.push_back( arg );
        }
        
        // add the random value
        RbValue<void*> arg;
        arg.value = args[0]->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
        
        // Setting the parameter of the distribution
        distribution->setParameters( newArgs );

        if ( static_cast<const RbBoolean*>( args[args.size()-1] )->getValue() == false ) 
            density.setValue( distribution->pdf  ( *static_cast<const RbLanguageObject*>( args[0] ) ) );
        else
            density.setValue( distribution->lnPdf( *static_cast<const RbLanguageObject*>( args[0] ) ) );
        return density;
    }
    else if (functionType == RVALUE) {
        
        // converting the arguments into atomic data types
        std::vector<RbValue<void*> > newArgs;
        for (std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); ++i) {
            RbValue<void*> arg;
            arg.value = (*i)->getLeanValue(arg.lengths);
            newArgs.push_back( arg );
        }
        
        // add te return value
        RbLanguageObject* retVal = distribution->getTemplateRandomVariable().clone();
        RbValue<void*> arg;
        arg.value = retVal->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
        
        // Setting the parameter of the distribution
        distribution->setParameters( newArgs );
        const RbLanguageObject& draw = *retVal;
        distribution->rv();
        
        return draw;
    }
    else if (functionType == PROB) {
        // converting the arguments into atomic data types
        std::vector<RbValue<void*> > newArgs;
        
        // skip the first and the last parameter
        for (std::vector<const RbObject*>::const_iterator i = args.begin()+1; i != args.end(); ++i) {
            RbValue<void*> arg;
            arg.value = (*i)->getLeanValue(arg.lengths);
            newArgs.push_back( arg );
        }
        
        // add te return value
        RbLanguageObject* retVal = distribution->getTemplateRandomVariable().clone();
        RbValue<void*> arg;
        arg.value = retVal->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
        
        // Setting the parameter of the distribution
        distribution->setParameters( newArgs );
        
        cd.setValue( static_cast<ParserDistributionContinuous*>( distribution )->cdf( *static_cast<const Real*>( args[0] ) ) );
        return cd;
    }
    else if (functionType == QUANTILE) {
        
        // converting the arguments into atomic data types
        std::vector<RbValue<void*> > newArgs;
        // skip the first and the last parameter
        for (std::vector<const RbObject*>::const_iterator i = args.begin()+1; i != args.end(); ++i) {
            RbValue<void*> arg;
            arg.value = (*i)->getLeanValue(arg.lengths);
            newArgs.push_back( arg );
        }
        
        // add te return value
        RbLanguageObject* retVal = distribution->getTemplateRandomVariable().clone();
        RbValue<void*> arg;
        arg.value = retVal->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
        
        // Setting the parameter of the distribution
        distribution->setParameters( newArgs );
        
        double    prob  = static_cast<const RealPos*>( args[0] )->getValue();
        quant.setValue( static_cast<ParserDistributionContinuous*>( distribution )->quantile( prob ) );
        
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
    
    // Set member variables of the distribution
    for (std::vector<Argument>::iterator i = this->args.begin(); i != this->args.end(); i++) {
        Argument& theArg = *i;
        std::string label = theArg.getLabel();
        if ( label != "x" && label != "p" && label != "q" && label != "log" ) {
            distribution->setMember( label, theArg.getVariablePtr() );
        }
    }
}

