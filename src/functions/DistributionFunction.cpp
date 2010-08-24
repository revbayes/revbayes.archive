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
#include "ConstantNode.h"
#include "DistributionFunction.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistributionReal.h"
#include "Boolean.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
DistributionFunction::DistributionFunction(Distribution* dist, FuncType funcType)
    : RbFunction() {

    /* Set the distribution */
    distribution = dist;

    /* Set the function type */
    functionType = funcType;

    /* Get the distribution parameter rules and set type to value argument */
    const ArgumentRules& memberRules = dist->getMemberRules();
    for (ArgumentRules::const_iterator i = memberRules.begin(); i!=memberRules.end(); i++) {
        argumentRules.push_back(new ArgumentRule(*(*i)));
    }

    /* Modify argument rules and set return type based on function type */
    if (functionType == DENSITY) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("x", distribution->getVariableType()));
        returnType = Real_name;
    }
    else if (functionType == RVALUE) {
        returnType = distribution->getVariableType();
    }
    else if (functionType == PROB) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("q", distribution->getVariableType()));
        returnType = Real_name;
    }
    else if (functionType == QUANTILE) {
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", Real_name));
        returnType = distribution->getVariableType();
    }
}


/** Copy constructor */
DistributionFunction::DistributionFunction(const DistributionFunction& x) {

    argumentRules = x.argumentRules;
    returnType    = x.returnType;
    distribution  = dynamic_cast<Distribution*>(x.distribution->clone());
    functionType  = x.functionType;

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
        argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", Real_name));
    }
}


/** Destructor */
DistributionFunction::~DistributionFunction(void) {

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
            argumentRules.insert(argumentRules.begin(), new ArgumentRule("p", Real_name));
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
DAGNode* DistributionFunction::executeOperation(const std::vector<DAGNode*>& args) {

    if (functionType == DENSITY) {
        if (((Boolean*)(args.back()->getValue()))->getValue() == false)
            return new ConstantNode(new Real(distribution->pdf(args[0]->getValue())));
        else
            return new ConstantNode(new Real(distribution->lnPdf(args[0]->getValue())));
    }
    else if (functionType == RVALUE) {
         return new ConstantNode(distribution->rv());
    }
    else if (functionType == PROB) {
        return new ConstantNode(new Real(((DistributionReal*)(distribution))->cdf(((Real*)(args[0]->getValue()))->getValue())));
    }
    else if (functionType == QUANTILE) {
        return new ConstantNode(new Real((((DistributionReal*)(distribution))->quantile(((Real*)(args[0]->getValue()))->getValue()))));
    }

    throw ("Unrecognized distribution function");
}


/** Get argument rules */
const ArgumentRules& DistributionFunction::getArgumentRules(void) const {

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& DistributionFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(DistributionFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const std::string& DistributionFunction::getReturnType(void) const {

    return returnType;
}

/** Process arguments */
bool DistributionFunction::processArguments(const std::vector<Argument>& args, VectorInteger* matchScore) {

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

