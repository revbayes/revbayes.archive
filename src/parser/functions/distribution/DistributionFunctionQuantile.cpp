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
#include "DistributionContinuous.h"
#include "DistributionFunctionQuantile.h"
#include "Distribution.h"
#include "ParserDistributionContinuous.h"
#include "Probability.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( const RbPtr<ParserDistributionContinuous> &dist ) : RbFunction(), returnType( Real::getClassTypeSpec() ) {
    
    /* Ininitalize the argument rules */
    argumentRules = new ArgumentRules();
    
    /* Set the distribution */
    distribution = dist;
    
    /* Get the distribution parameter rules and set type to value argument */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for ( std::vector<ArgumentRule*>::const_iterator i = memberRules.begin(); i != memberRules.end(); i++ ) {
        // check if this rule has a default value
        if ((*i)->hasDefault()) {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec(), (*i)->getDefaultVariable().getValue()->clone() ) );
        } else {
            argumentRules->push_back( new ConstArgumentRule( (*i)->getArgumentLabel(), (*i)->getArgumentTypeSpec() ) );
        }
    }
    
    argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , Probability::getClassTypeSpec()     ) );
    
}


/** Copy constructor */
DistributionFunctionQuantile::DistributionFunctionQuantile( const DistributionFunctionQuantile& x ) : RbFunction(x), returnType( x.returnType ) {
    
    // copy the argument rules
    argumentRules = new ArgumentRules();
    for (std::vector<ArgumentRule* >::const_iterator it = x.argumentRules->begin(); it != x.argumentRules->end(); it++) {
        argumentRules->push_back( (*it)->clone() );
    }
    
    distribution  = x.distribution->clone();
    
    argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , Probability::getClassTypeSpec()     ) );
    
}


/** Assignment operator */
DistributionFunctionQuantile& DistributionFunctionQuantile::operator=( const DistributionFunctionQuantile& x ) {
    
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
        
        argumentRules->insert( argumentRules->begin(), new ConstArgumentRule( "p"  , Probability::getClassTypeSpec()     ) );
        
    }
    
    return (*this);
}


/** Clone object */
DistributionFunctionQuantile* DistributionFunctionQuantile::clone( void ) const {
    
    return new DistributionFunctionQuantile(*this);
}


/** Execute operation: switch based on type */
RbPtr<RbLanguageObject> DistributionFunctionQuantile::executeFunction(const std::vector<const RbObject *> &args) {
    
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

    return RbPtr<RbLanguageObject>( new Real( distribution->quantile( prob ) ) );
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionQuantile::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get class name of object */
const std::string& DistributionFunctionQuantile::getClassName(void) { 
    
    static std::string rbClassName = "Quantile";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionQuantile::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionQuantile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunctionQuantile::getReturnType(void) const {
    
    return returnType;
}


/** Process arguments */
void DistributionFunctionQuantile::processArguments( const std::vector<RbPtr<Argument> >& args ) {
    
    // delegate first to the base class
    RbFunction::processArguments( args );
    
    // Set member variables of the distribution
    for (std::vector<RbPtr<Argument> >::iterator i = this->args.begin(); i != this->args.end(); i++) {
        const RbPtr<Argument>& theArg = *i;
        std::string label = theArg->getLabel();
        if ( label != "p" ) {
            distribution->setMember( label, theArg->getVariable() );
        }
    }
}

