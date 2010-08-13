/**
 * @file
 * This file contains the implementation of DistributionFunction, which
 * is used for functions related to a statistical distribution.
 *
 * @brief Implementation of DistributionFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DistributionFunction.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistributionReal.h"
#include "RbBool.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Constructor */
DistributionFunction::DistributionFunction(Distribution* dist, FuncType funcType)
    : RbFunction() {

    /* Set the distribution */
    distribution = dist;

    /* Set the function type */
    functionType = funcType;

    /* Initialize return value */
    retObject = NULL;

    /* Get the distribution parameter rules and set type to value argument except for rng */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for (ArgumentRules::const_iterator i = memberRules.begin(); i!=memberRules.end(); i++) {
        argumentRules.push_back(new ArgumentRule(*(*i)));
        if ((*i)->getLabel() != "rng")
            argumentRules.back()->setWrapperRule(false);
    }

    /* Modify argument rules and set return type based on function type */
    if (functionType == DENSITY) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("x", distribution->getVariableType()));
        returnType = RbDouble_name;
    }
    else if (functionType == RVALUE) {
        returnType = distribution->getVariableType();
    }
    else if (functionType == PROB) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("q", distribution->getVariableType()));
        returnType = RbDouble_name;
    }
    else if (functionType == QUANTILE) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", RbDouble_name));
        returnType = distribution->getVariableType();
    }
}


/** Copy constructor */
DistributionFunction::DistributionFunction(const DistributionFunction& x) {

    argumentRules = x.argumentRules;
    returnType    = x.returnType;
    distribution  = dynamic_cast<Distribution*>(x.distribution->clone());
    functionType  = x.functionType;
    retObject     = NULL;   // Do not copy work space
    retDouble     = x.retDouble;

    /* Modify argument rules based on function type */
    if (functionType == DENSITY) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("x", distribution->getVariableType()));
    }
    else if (functionType == RVALUE) {
    }
    else if (functionType == PROB) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("q", distribution->getVariableType()));
    }
    else if (functionType == QUANTILE) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", RbDouble_name));
    }
}


/** Destructor */
DistributionFunction::~DistributionFunction(void) {

    delete retObject;
    delete distribution;
}


/** Assignment operator */
DistributionFunction& DistributionFunction::operator=(const DistributionFunction& x) {

    if (this != &x) {
        
        delete distribution;

        argumentRules = x.argumentRules;
        returnType    = x.returnType;
        distribution  = dynamic_cast<Distribution*>(x.distribution->clone());
        functionType  = x.functionType;
        retDouble     = x.retDouble;

        /* Modify argument rules based on function type */
        if (functionType == DENSITY) {
            argumentRules.insert(argumentRules.begin(), new ArgumentRule("x", distribution->getVariableType()));
        }
        else if (functionType == RVALUE) {
        }
        else if (functionType == PROB) {
            argumentRules.insert(argumentRules.begin(), new ArgumentRule("q", distribution->getVariableType()));
        }
        else if (functionType == QUANTILE) {
            argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", RbDouble_name));
        }
    }

    return (*this);
}


/** Clone object */
DistributionFunction* DistributionFunction::clone(void) const {

    return new DistributionFunction(*this);
}


/** Pointer-based equals comparison */
bool DistributionFunction::equals(const RbObject* x) const {

    return false;
}


/** Execute operation: switch based on type */
const RbObject* DistributionFunction::executeOperation(const std::vector<DAGNode*>& args) {

    if (functionType == DENSITY) {
        if (((RbBool*)(args.back()->getValue()))->getValue() == false)
            retDouble.setValue(distribution->pdf(args[0]->getValue()));
        else
            retDouble.setValue(distribution->lnPdf(args[0]->getValue()));
    }
    else if (functionType == RVALUE) {
        if (retObject != NULL)
            delete retObject;
         retObject = distribution->rv();
         return retObject;
    }
    else if (functionType == PROB) {
        retDouble.setValue(((DistributionReal*)(distribution))->cdf(((RbDouble*)(args[0]->getValue()))->getValue()));
    }
    else if (functionType == QUANTILE) {
        retDouble.setValue((((DistributionReal*)(distribution))->quantile(((RbDouble*)(args[0]->getValue()))->getValue())));
    }

    return &retDouble;
}


/** Get argument rules */
const ArgumentRules& DistributionFunction::getArgumentRules(void) const {

    return argumentRules;
}


/** Get class vector describing type of object */
const StringVector& DistributionFunction::getClass(void) const { 

    static StringVector rbClass = StringVector(DistributionFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const std::string& DistributionFunction::getReturnType(void) const {

    return returnType;
}


/** Process arguments */
bool DistributionFunction::processArguments(const std::vector<Argument>& args, IntVector* matchScore) {

    if (!RbFunction::processArguments(args, matchScore))
        return false;

    /* Set member variables of the distribution */
    ArgumentRules::iterator i=argumentRules.begin();
    std::vector<DAGNode*>::iterator j = processedArguments.begin();
    if (functionType != RVALUE) {
        i++;
        j++;
    }

    for (; i!=argumentRules.end(); i++, j++) {
        std::string name = (*i)->getLabel();
        distribution->setVariable(name, (*j));
    }

    return true;
}


