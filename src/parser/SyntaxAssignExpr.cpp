/**
 * @file
 * This file contains the implementation of SyntaxAssignExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <list>
#include <sstream>

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "SyntaxAssignExpr.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxAssignExpr::opCode[] = { "ARROW_ASSIGN", "TILDE_ASSIGN", "EQUATION_ASSIGN" };


/** Construct from operator type, variable and expression */
SyntaxAssignExpr::SyntaxAssignExpr(SyntaxAssignExpr::operatorT op, SyntaxVariable* var, SyntaxElement* expr) 
    : SyntaxElement(), variable(var), expression(expr), opType(op) {
}


/** Deep copy constructor */
SyntaxAssignExpr::SyntaxAssignExpr(const SyntaxAssignExpr& x)
    : SyntaxElement(x) {

    variable   = new SyntaxVariable(*x.variable);
    expression = x.expression->clone();
    opType     = x.opType;
}


/** Destructor deletes operands */
SyntaxAssignExpr::~SyntaxAssignExpr() {
    
    delete variable;
    delete expression;
}


/** Return brief info about object */
std::string SyntaxAssignExpr::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxAssignExpr: operation = " << opCode[opType];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxAssignExpr::clone () const {

    return (SyntaxElement*)(new SyntaxAssignExpr(*this));
}


/** Equals comparison */
bool SyntaxAssignExpr::equals(const SyntaxElement* elem) const {

	const SyntaxAssignExpr* p = dynamic_cast<const SyntaxAssignExpr*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && variable->equals(p->variable);
    result = result && expression->equals(p->expression);
    result = result && (opType == p->opType);
    
    return result;
}


/** Convert element to DAG node: insert symbol and return reference to it */
DAGNode* SyntaxAssignExpr::getDAGNode(Frame* frame) const {

    // Insert symbol; discard the return value
    delete getValue(frame);

    // Return DAG node from variable
    return variable->getDAGNode();
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbObject* SyntaxAssignExpr::getValue(Frame* frame) const {

    // Get variable identifier
    RbString varName = *variable->getIdentifier();

    // Get member frame
    const Frame* memberFrame = variable->getFrame();
    if (variable->isMember())
        theFrame = variable->getFrame();
    else
        theFrame = frame;

    // Deal with arrow assignments
    if (opType == ARROW_ASSIGN) {

        // Calculate the value of the rhs expression; we assume it is not a variable DAG node
        RbObject* value = expression->getValue();

        // Does the variable exist?
        if (!variable->isMember() && !frame->existsVariable(varName)) {

            // It does not exist - add it
            IntVector index = variable->getIndex(frame);
            if (index.size() != 0)
                frame->addVariable(varName, index, value);
            else
                frame->addVariable(varName, value);
        }
        else {

            // It exists - get a handle to it
            RbObject** handle = variable->getHandle(frame);

            // Get declared element type
            std::string declaredType = frame->getDeclaredType(varName);

            // Assign a new value depending on type of handle
            if ((*handle)->isType(RbNames::StochasticNode::name))
                ((StochasticNode*)(*handle))->clamp(value);
            else if ((*handle)->isType(RbNames::DAGNode::name)) {
                if (!value->isType(declaredType)) {
                    delete value;
                    throw (RbException("Incompatible types"));
                }
                delete (*handle);
                (*handle) = new ConstantNode(value);
            }
            else if ((*handle)->getElementDim() != 0) {
                (*handle)->setElement(variable->getHandleIndex(frame), value);
            }
            else {
                delete value;
                throw (RbException("Cannot assign dag node to " + variable->getFullName(frame)));
            }
        }
    }

    // Deal with equation assignments
    else if (opType == EQUATION_ASSIGN) {

        // Get DAG node representation of expression
        DeterministicNode* node = dynamic_cast<DeterministicNode*>(expression->getDAGNode());
        if (node == NULL)
            throw (RbException("Righ-hand side is not a variable expression"));

        // Does the variable exist?
        if (!variable->isMember() && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            IntVector index = variable->getIndex(frame);
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }
        else {

            // The variable exists - get a handle to it
            RbObject** handle = variable->getHandle(frame);

            // Get declared element type
            std::string declaredType = frame->getDeclaredType(varName);

            // Assign a new value to the handle
            if ((*handle)->isType(RbNames::DAGNode::name)) {
                if (!node->getValue()->isType(declaredType)) {
                    delete node;
                    throw (RbException("Incompatible types"));
                }
                delete (*handle);
                (*handle) = node;
            }
            else {
                delete node;
                throw (RbException("Cannot assign dag node to " + variable->getFullName(frame)));
            }
        }
    }

    // Deal with tilde assignments
    else if (opType == TILDE_ASSIGN) {

        // Get distribution
        Distribution* dist = dynamic_cast<Distribution*>(expression->getValue());
        if (dist == NULL)
            throw (RbException("Function does not return a distribution"));
 
        // Create new stochastic node
        StochasticNode* node = new StochasticNode(dist);

        // Does variable exist?
        if (!variable->isMember() && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            IntVector index = variable->getIndex(frame);
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }

        else {

            // The variable exists - get a handle to it
            RbObject** handle = variable->getHandle(frame);

            // Get declared element type
            std::string declaredType = frame->getDeclaredType(varName);

            // Assign a new value to the handle
            if ((*handle)->isType("DAGNode")) {
                if (!node->getValue()->isType(declaredType)) {
                    delete node;
                    throw (RbException("Incompatible types"));
                }
                else {
                    delete (*handle);
                    (*handle) = node;
                }
            }
            else {
                delete node;
                throw (RbException("Cannot assign dag node to " + variable->getFullName(frame)));
            }
        }
    }

    // Return copy of the value of the rhs expression
    return expression->getValue();
}


/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {

    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}


