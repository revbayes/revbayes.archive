/**
 * @file
 * This file contains the implementation of DeterministicNode, which is a derived
 * class from DAGNode. DeterministicNode are DAG nodes with an expression assigned to its value.
 *
 * @brief Implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "DeterministicNode.h"
#include "../datatypes/RbDataType.h"
#include "../modelLanguage/parser/SyntaxElement.h"


DeterministicNode::DeterministicNode(RbDataType* dt) : DAGNode(dt) {
    rootSyntaxTree = NULL;
}

DeterministicNode::DeterministicNode(RbDataType* dt, SyntaxElement* e) : DAGNode(dt) {
	rootSyntaxTree = e;
}

DeterministicNode::DeterministicNode(DeterministicNode& dn) : DAGNode(dn.getValue()->clone()) {
    //TODO implement
//    rootSyntaxTree = dn.;

    //TODO implement
    // copy references to parents
    // copy references to children

    std::string message = "Copy constructor of DeterministicNode not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

DeterministicNode::~DeterministicNode() {
    //TODO add missing objects to delete
}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* DeterministicNode::clone(void) {

    RbObject *x = new DeterministicNode( *this );
    return x;
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void DeterministicNode::print(std::ostream &c) const {
    //TODO this is not really meaningful implemented
    value->print();
    storedValue->print();

    std::string message = "Print function of DeterministicNode not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void DeterministicNode::dump(std::ostream& c){
    //TODO implement

}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void DeterministicNode::resurrect(const RbDumpState& x){
    //TODO implement

}
