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


/** Basic constructor */
RbFunction::RbFunction(const ArgumentRule** rules, const std::string& retType)
    : argRules(), returnType(retType) {

    for (int i=0; rules[0] != NULL; i++) {
        argRules.push_back(*(rules[i]));
    }

    /* Check for duplicate labels */
    for (std::vector<ArgumentRule>::iterator i=argRules.begin(); i!=argRules.end(); i++) {
        for (std::vector<ArgumentRule>::iterator j=i+1; j!=argRules.end(); j++) {
            if (i->getLabel() == j->getLabel()) {
                std::ostringstream msg;
                msg << "Label '" << i->getLabel() <<  "' is duplicated among argument rules";
                throw RbException(msg.str());
            }
        }
    }
}


/** Copy constructor */
RbFunction::RbFunction(const RbFunction &fn)
    : argRules(fn.argRules), returnType(fn.returnType) {
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
std::vector<DAGNode*>  RbFunction::processArguments(const std::vector<Argument>& args) {

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
    for (std::vector<Argument>::const_iterator i=args.begin(); i!=args.end(); i++, index++) {
        int theArg = -1;
        if (i->getLabel() == "") {
            theArg = index;
        }
        else {
            for (theArg=0; theArg<(int)argRules.size(); theArg++) {
                if (i->getLabel() == argRules[theArg].getLabel())
                    break;
            }
            if (theArg == (int)argRules.size()) {
                std::string msg = "Did not expect an argument with label '" + i->getLabel() + "'";
                arguments.clear();
                throw RbException(msg);
            }
        }
        if (arguments[theArg] != NULL) {
            std::ostringstream msg;
            msg << "Multiple arguments fit argument rule " << index+1;
            arguments.clear();
            throw RbException(msg.str());
        }
        arguments[theArg] = i->getDAGNode();
    }

    /* Fill in default values */
    index = 0;
    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ((*i) == NULL) {
            if (argRules[index].getValue() == NULL) {
                std::string msg = "No default value for argument label '" + argRules[index].getLabel() + "'";
                arguments.clear();
                throw RbException(msg);
            }
            (*i) = new ConstantNode(argRules[index].getValue()->clone());
        }
    }

    /* Success */
    return arguments;
}


