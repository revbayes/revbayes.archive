/**
 * @file
 * This file contains the implementation of DistributionFunction, which
 * is used for functions related to a statistical distribution.
 *
 * @brief Implementation of DistributionFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-10 16:04:22 +0200 (Thu, 10 May 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: DistributionFunction.cpp 1529 2012-05-10 14:04:22Z hoehna $
 */

#include "ArgumentRule.h"
#include "RbBoolean.h"
#include "ConstantNode.h"
#include "ConstArgumentRule.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistributionFunctionPdf.h"
#include "Distribution.h"
#include "ParserDistribution.h"
#include "Probability.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
DistributionFunctionPdf::DistributionFunctionPdf( const RbPtr<ParserDistribution> &dist ) : RbFunction(), returnType( Real::getClassTypeSpec() ) {
    
    /* Ininitalize the argument rules */
    argumentRules = new ArgumentRules();
    
    /* Set the distribution */
    distribution = dist;
    
    /* Get the distribution parameter rules and set type to value argument */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for ( std::vector<ArgumentRule*>::const_iterator i = memberRules.begin(); i != memberRules.end(); i++ ) {
        // check if this rule has a default value
        if ((*i)->hasDefault()) {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec(), RbPtr<RbLanguageObject>( (*i)->getDefaultVariable().getValue().getSingleValue()->clone() ) ) );
        } else {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec() ) );
        }
    }
    
    argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "x"  , distribution->getVariableType() ) );
    argumentRules->push_back(                      new ConstArgumentRule( "log", new RbBoolean(false)            ) );
    
    name = "pdf<" + dist->getClassName() + ">(";
    for ( std::vector<ArgumentRule*>::const_iterator i = argumentRules->begin(); i != argumentRules->end(); i++ ) {
        // add the argument
        if ( i != argumentRules->begin() ) {
            name += ", ";
        }
        name += (*i)->getArgumentTypeSpec().getType() + " ";
        name += (*i)->getArgumentLabel();
        
        if ( (*i)->hasDefault() ) {
            name += " = ";
            std::stringstream o;
            (*i)->getDefaultVariable().getValue().getSingleValue()->printValue( o );
            name += o.str();
        }
    }
    name += ")";
}


/** Copy constructor */
DistributionFunctionPdf::DistributionFunctionPdf( const DistributionFunctionPdf& x ) : RbFunction(x), returnType( x.returnType ) {
    
    // copy the argument rules
    argumentRules = new ArgumentRules();
    for (std::vector<ArgumentRule* >::const_iterator it = x.argumentRules->begin(); it != x.argumentRules->end(); it++) {
        argumentRules->push_back( (*it)->clone() );
    }
    
    distribution  = x.distribution->clone();
    
    name = x.name;
    
}


/** Assignment operator */
DistributionFunctionPdf& DistributionFunctionPdf::operator=( const DistributionFunctionPdf& x ) {
    
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
        
        name = x.name;
        
    }
    
    return (*this);
}


/** Clone object */
DistributionFunctionPdf* DistributionFunctionPdf::clone( void ) const {
    
    return new DistributionFunctionPdf(*this);
}


/** Execute operation */
RbPtr<RbLanguageObject> DistributionFunctionPdf::executeFunction(const std::vector<const RbObject *> &args) {
    
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
        return RbPtr<RbLanguageObject>( new RealPos( distribution->pdf  ( *static_cast<const RbLanguageObject*>( args[0] ) ) ) );
    else
        return RbPtr<RbLanguageObject>( new Real( distribution->lnPdf( *static_cast<const RbLanguageObject*>( args[0] ) ) ) );
    
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionPdf::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get class name of object */
const std::string& DistributionFunctionPdf::getClassName(void) { 
    
    static std::string rbClassName = "PDF";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionPdf::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionPdf::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = TypeSpec( name, new TypeSpec( RbFunction::getClassTypeSpec() ) );;
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunctionPdf::getReturnType(void) const {
    
    return returnType;
}


/** Process arguments */
void DistributionFunctionPdf::processArguments( const std::vector<RbPtr<Argument> >& args ) {
    
    // delegate first to the base class
    RbFunction::processArguments( args );
    
    // Set member variables of the distribution
    for (std::vector<RbPtr<Argument> >::iterator i = this->args.begin(); i != this->args.end(); i++) {
        const RbPtr<Argument>& theArg = *i;
        std::string label = theArg->getLabel();
        if ( label != "x" && label != "log" ) {
            distribution->setConstMember( label, theArg->getVariable() );
        }
    }
}

