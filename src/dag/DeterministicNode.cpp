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
#include "RbException.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"

/** Basic constructor */
DeterministicNode::DeterministicNode(RbFunction* func, const std::vector<Argument>& args)
    : DAGNode(), function(func) {

    //arguments = function->processArguments(args);
    //for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
    //    parents.insert(*i);
//
//    value = function->execute(arguments);
}

/** Copy constructor */
DeterministicNode::DeterministicNode(const DeterministicNode& d)
    : DAGNode(d), arguments(d.arguments), function(d.function) {
}

RbObject* DeterministicNode::clone() const {
	DeterministicNode* x = new DeterministicNode(*this);
	return (RbObject*) x;
}


bool DeterministicNode::equals(const RbObject* obj) const {
	return false;
}

double DeterministicNode::getLnProbabilityRatio() {
	double lnProb = 0.0;
	for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
		lnProb += (*i)->getLnProbabilityRatio();
	}
	return lnProb;
}


/** Get value intelligently */
RbObject* DeterministicNode::getValue() {

    //if (isTouched() && !isChanged()) {
    //    if (storedValue != NULL)
    //        delete storedValue;
    //    storedValue = value;
    //    value = function->execute(arguments);
    //    changed = true;
    //}

    return value;
}

RbObject& DeterministicNode::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const DeterministicNode& x = dynamic_cast<const DeterministicNode&> (obj);

        DeterministicNode& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const DeterministicNode& x = dynamic_cast<const DeterministicNode&> (*(obj.convertTo("const_node")));

            DeterministicNode& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to const_node");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DeterministicNode& DeterministicNode::operator=(const DeterministicNode& obj) {

    changed = obj.changed;
    children = obj.children;
    (*lastMove) = (*obj.lastMove);
    members = obj.members;
    methods = obj.methods;
    monitors = obj.monitors;
    (*moves) = (*obj.moves);
    parents = obj.parents;
    (*storedValue) = (*obj.storedValue);
    touched = obj.touched;
    (*value) = (*obj.value);

    return (*this);
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

void DeterministicNode::printValue(std::ostream &o) const {

    o << value->toString() << std::endl;
}

std::string DeterministicNode::toString(void) const {

    return value->toString();
}


