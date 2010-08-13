/**
 * @file
 * This file contains the implementation of Func__lookup, which is used
 * for functions that construct member objects.
 *
 * @brief Implementation of Func__lookup
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
#include "Ellipsis.h"
#include "IntVector.h"
#include "Func__lookup.h"
#include "DAGNode.h"
#include "Frame.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbString.h"
#include "StringVector.h"
#include "WrapperRule.h"

#include <sstream>


/** Construct from type and dim */
Func__lookup::Func__lookup(const std::string& type, int dim)
    : RbFunction(), retValue(NULL) {

    argRules.push_back(new WrapperRule("", type, dim));
    argRules.push_back(new Ellipsis(RbInt_name));
}


/** Copy constructor */
Func__lookup::Func__lookup(const Func__lookup& x)
    : RbFunction(x), argRules(x.argRules), retValue(NULL) {
}


/** Destructor */
Func__lookup::~Func__lookup(void) {

    delete retValue;
}


/** Assignment operator */
Func__lookup& Func__lookup::operator=(const Func__lookup& x) {

    if (this != &x) {
        
        RbFunction::operator=(x);

        argRules = x.argRules;

        /* Do not copy workspace, we keep our own */
    }

    return (*this);
}


/** Clone the object */
Func__lookup* Func__lookup::clone(void) const {

    return new Func__lookup(*this);
}


/** Pointer-based equals comparison */
bool Func__lookup::equals(const RbObject* x) const {

    const Func__lookup* p = dynamic_cast<const Func__lookup*>(x);
    if ( p == NULL)
        return false;

    /* It is an error if arguments are not processed for a lookup function */
    if (argumentsProcessed == false || p->argumentsProcessed == false)
        return false;

    if (processedArguments.size() != p->processedArguments.size())
        return false;

    std::vector<DAGNode*>::const_iterator i, j;
    for (i=processedArguments.begin(), j=p->processedArguments.begin(); i!=processedArguments.end(); i++, j++)
        if ((*i) != (*j))
            return false;

    return true;
}


/** Execute operation: calculate index and get value from variable */
const RbObject* Func__lookup::executeOperation(const std::vector<DAGNode*>& args) {

    delete retValue;

    DAGNode* theVariable = args[0];

    IntVector index;
    for (std::vector<DAGNode*>::const_iterator i=args.begin()+1; i!=args.end(); i++) {
        index.push_back(((RbInt*)((*i)->getValue()))->getValue());
    }

    if (index.size() == 0)
        retValue = theVariable->getValue()->clone();
    else
        retValue = theVariable->getValElement(index)->clone();

    return retValue;
}


/** Get argument rules */
const ArgumentRules& Func__lookup::getArgumentRules(void) const {

    return argRules;
}


/** Get class vector describing type of object */
const StringVector& Func__lookup::getClass(void) const { 

    static StringVector rbClass = StringVector(Func__lookup_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get dim of return value*/
int Func__lookup::getReturnDim(void) const {

    return argRules[0]->getDim() - (int(argRules.size()) - 1);
}


/** Get type of return value */
const std::string& Func__lookup::getReturnType(void) const {

    return argRules[0]->getValueType();
}

