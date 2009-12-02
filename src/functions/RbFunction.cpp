/**
 * @file
 * This file contains the implementation of some functions in
 * RbFunction, the interface fnd abstract base class for RevBayes
 * functions.
 *
 * @brief Partial implementation of RbFunction
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

#include <list>

#include "ConstantNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "StringVector.h"

const StringVector RbFunction::rbClass = StringVector("function") + RbObject::rbClass;

/** Basic constructor */
RbFunction::RbFunction(void) {
    argumentsProcessed = false;
}


/** Copy constructor */
RbFunction::RbFunction(const RbFunction &fn)
    : argRules(fn.argRules), returnType(fn.returnType) {
    
}

RbObject* RbFunction::execute(const std::vector<Argument*>& args) {

	std::vector<DAGNode*> dags = processArguments(args);
	RbObject* result = executeOperation(dags);
	return result;
}

RbObject* RbFunction::execute() {

    if (!argumentsProcessed) {
        throw RbException("Arguments were not processed before executing function.");
    }
    RbObject* result = executeOperation(processedArguments);
    return result;
}

/**
 * @brief Process arguments
 *
 * This function processes arguments based on argument rules.
 *
 * We use the following logic:
 *
 *  1. Count the number of argument rules.
 *  2. Check that the number of provided arguments is
 *     appropriate.
 *  3. Match labeled arguments to argument rules and
 *     argument positons as follows:
 *     - If no label is provided, the argument goes into
 *       the same position it is placed in the list of
 *       provided arguments. If that position is already
 *       filled, it is an error.
 *     - If a label is provided, the argument goes into
 *       the position with that label in its argument
 *       rule. If the label is not found among the
 *       argument rules, or the position is already
 *       filled, it is an error.
 *  4. Fill any remaining position with default values.
 *     If an empty position at this stage does not have
 *     a default value, it is an error.
 *
 */
std::vector<DAGNode*>  RbFunction::processArguments(const std::vector<Argument*>& args) {

    /* Check that the number of provided arguments is adequate */
    if (argRules.size() < args.size()) {
        if (argRules.size()==0) {
            throw RbException("Not expecting any arguments");
        }
        else {
            throw RbException("Too many arguments");
        }
    }

    /* Initialize vector of processed arguments */
    std::vector<DAGNode*>   arguments(argRules.size(),NULL);

    /* Match arguments */
    int index=0;
    for (std::vector<Argument*>::const_iterator i=args.begin(); i!=args.end(); i++, index++) {
        int theArg = -1;
        if ((*i)->getLabel() == "") {
            theArg = index;
        }
        else {
            for (theArg=0; theArg<(int)argRules.size(); theArg++) {
                if ((*i)->getLabel() == argRules[theArg].getLabel())
                    break;
            }
            if (theArg == (int)argRules.size()) {
                std::string msg = "Did not expect an argument with label '" + (*i)->getLabel() + "'";
                arguments.clear();
                throw RbException(msg);
            }
        }
        if (arguments[theArg] != NULL) {
        	char temp[100];
        	sprintf(temp, "Multiple arguments fit argument rule %d", index+1);
        	std::string msg = temp;
            arguments.clear();
            throw RbException(msg);
        }
        arguments[theArg] = (*i)->getDAGNode();
    }

    /* Fill in default values */
    index = 0;
    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ((*i) == NULL) {
            if (argRules[index].getDefaultValue() == NULL) {
                std::string msg = "No default value for argument label '" + argRules[index].getLabel() + "'";
                arguments.clear();
                throw RbException(msg);
            }
            (*i) = new ConstantNode(argRules[index].getDefaultValue()->clone());
        }
    }

    argumentsProcessed = true;
    processedArguments = arguments;

    /* Success */
    return arguments;
}


