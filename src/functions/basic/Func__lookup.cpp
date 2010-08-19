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
#include "DAGNode.h"
#include "DAGNodeContainer.h"
#include "Ellipsis.h"
#include "Func__lookup.h"
#include "Frame.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorString.h"
#include "WrapperRule.h"

#include <cassert>
#include <sstream>


/** Construct from type and dim */
Func__lookup::Func__lookup(const std::string& type, int dim)
    : RbFunction() {

    argRules.push_back(new WrapperRule("", type, dim));
    argRules.push_back(new Ellipsis(Integer_name));
}


/** Copy constructor */
Func__lookup::Func__lookup(const Func__lookup& x)
    : RbFunction(x), argRules(x.argRules) {
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
RbObject* Func__lookup::executeOperation(const std::vector<DAGNode*>& args) {

    DAGNode* theVariable = args[0];

    // Get the index vector from the DAGNodeContainer corresponding to the ellipsis,
    // if there is one. Subtract one to get internal 0-based index.
    VectorInteger index;
    if (args.size() > 1) {
        const Container* indexContainer = dynamic_cast<const Container*>(args[1]->getValue());
        assert(indexContainer != NULL);
        for (size_t i=0; i<indexContainer->size(); i++)
            index.push_back(((Integer*)((*indexContainer)[i]))->getValue()-1);
    }
    
    if (index.size() == 0)
        return theVariable->getValue()->clone();
    else
        return theVariable->getValElement(index)->clone();
}


/** Get argument rules */
const ArgumentRules& Func__lookup::getArgumentRules(void) const {

    return argRules;
}


/** Get class vector describing type of object */
const VectorString& Func__lookup::getClass(void) const { 

    static VectorString rbClass = VectorString(Func__lookup_name) + RbFunction::getClass();
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

/** Get variable */
DAGNode* Func__lookup::getVariable(void) const {

    const std::vector<DAGNode*>& args = processedArguments;
    DAGNode* theVariable = args[0];

    if (args.size() == 1)
        return theVariable;

    DAGNodeContainer* theVariableContainer = dynamic_cast<DAGNodeContainer*>(args[0]);
    if (theVariableContainer == NULL)
        throw ("Reference to a value element that is not a DAG node");

    // Get the index vector from the DAGNodeContainer corresponding to the ellipsis,
    // if there is one. Subtract one to get internal 0-based index.
    VectorInteger index;
    const Container* indexContainer = dynamic_cast<const Container*>(args[1]->getValue());
    assert(indexContainer != NULL);
    for (size_t i=0; i<indexContainer->size(); i++)
        index.push_back(((Integer*)((*indexContainer)[i]))->getValue()-1);
    
    return theVariableContainer->getVarElement(index);
}

