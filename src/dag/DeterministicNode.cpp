/**
 * @file
 * This file contains the implementation of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with an
 * expression (equation) that determines their value.
 *
 * @brief Implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "DeterministicNode.h"

/** Basic constructor */
DeterministicNode::DeterministicNode(RbFunction* func, const std::vector<Argument>& args)
    : DAGNode(), function(func) {

    arguments = function->processArguments(args);
    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
        parents.insert(*i);

    value = function->execute(arguments);
}

/** Copy constructor */
DeterministicNode::DeterministicNode(const DeterministicNode& d)
    : DAGNode(d), arguments(d.arguments), function(d.function) {
}


/** Get value intelligently */
RbObject* DeterministicNode::getValue() {

    if (isTouched() && !isChanged()) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        value = function->execute(arguments);
        changed = true;
    }

    return value;
}


/**
 * @brief Print function
 *
 * This function prints complete info about the  object.
 *
 * @param o           The ostream for printing
 *
 */
void DeterministicNode::print(std::ostream &o) const {

    RbObject::print(o);     // Prints class
    DAGNode::print(o);      // Prints general info

    o << "Function = " << function->briefInfo() << std::endl;
}


