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
#include "DAGNodeContainer.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"
#include "StochasticNode.h"
#include "StringVector.h"
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


/** Get class vector describing type of object */
const StringVector& SyntaxAssignExpr::getClass(void) const { 

    static StringVector rbClass = StringVector(SyntaxAssignExpr_name) + SyntaxElement::getClass();
	return rbClass; 
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

    PRINTF("Evaluating assign expression\n");

    // Get variable identifier
    RbString varName = *variable->getIdentifier();

    // Get index
    IntVector index = variable->getIndex(frame);

    // Deal with arrow assignments
    if (opType == ArrowAssign) 
		{
        PRINTF("Arrow assignment\n");
        // Calculate the value of the rhs expression
        RbObject* exprValue = expression->getValue(frame);

        // Does the variable exist? If not, add it - but only to workspace, not to complex objects
        if (!variable->isMember()  && !frame->existsVariable(varName)) 
			{
            // It does not exist - add it
            if (index.size() != 0)
                frame->addVariable(varName, index, new ConstantNode(exprValue));
            else
                frame->addVariable(varName, new ConstantNode(exprValue));
			}
        else 
			{
            // It exists - use the frame or a complex object to assign to it 
            if (!variable->isMember()) 
				{
                if (index.size() != 0)
                    frame->setValElement(varName, index, exprValue);
                else
                    frame->setValue(varName, exprValue);
				}
            else 
				{
                PRINTF ("Assigning to existing member variable\n");
                const RbComplex* objectPtr = dynamic_cast<const RbComplex*>(variable->getVariable()->getValuePtr(frame));
                if (objectPtr == NULL)
                    throw RbException("Variable " + variable->getVariable()->getFullName(frame) + " does not have members");
                RbComplex* theObject = const_cast<RbComplex*>(objectPtr);
                if (index.size() == 0)
                    theObject->setValue(varName, exprValue);
                else 
					{
                    const DAGNodeContainer* containerPtr = dynamic_cast<const DAGNodeContainer*>(theObject->getVariable(varName));
                    if (containerPtr == NULL)
                        throw RbException("Variable " + variable->getVariable()->getFullName(frame) + "." + std::string(varName) + " does not have elements");
                    DAGNodeContainer* container = const_cast<DAGNodeContainer*>(containerPtr);
                    container->setElement(index, exprValue);
					}
				}
			}
		}

    // Deal with equation assignments
    else if (opType == EquationAssign) {

        PRINTF("Equation assignment\n");

        // Get DAG node representation of expression
        DAGNode* dag = expression->getDAGNode(frame);
        DeterministicNode* node = dynamic_cast<DeterministicNode*>(dag);
        if (node == NULL) {
            delete dag;
            throw (RbException("Righ-hand side is not a variable expression"));
        }

        // Does the variable exist?
        if (!variable->isMember() && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }
        else {

            // It exists - use the frame or a complex object to assign to it
            if (!variable->isMember()) {
                if (index.size() != 0)
                    frame->setVarElement(varName, index, node);
                else
                    frame->setVariable(varName, node);
            }
            else {

                PRINTF ("Assigning equation to existing member variable\n");
                const RbComplex* objectPtr = dynamic_cast<const RbComplex*>(variable->getVariable()->getValuePtr(frame));
                if (objectPtr == NULL)
                    throw RbException("Variable " + variable->getVariable()->getFullName(frame) + " does not have members");
                RbComplex* theObject = const_cast<RbComplex*>(objectPtr);
                if (index.size() == 0)
                    theObject->setVariable(varName, node);
                else {
                    const DAGNodeContainer* containerPtr = dynamic_cast<const DAGNodeContainer*>(theObject->getVariable(varName));
                    if (containerPtr == NULL)
                        throw RbException("Variable " + variable->getVariable()->getFullName(frame) + "." + std::string(varName) + " does not have elements");
                    DAGNodeContainer* container = const_cast<DAGNodeContainer*>(containerPtr);
                    container->setElement(index, node);
                }
            }
        }
    }

    // Deal with tilde assignments
    else if (opType == TildeAssign) {

        PRINTF("Tilde assignment\n");

        // Get distribution
        RbObject* exprValue = expression->getValue(frame);
        Distribution* dist = dynamic_cast<Distribution*>(exprValue);
        if (dist == NULL) {
            delete exprValue;
            throw (RbException("Function does not return a distribution"));
        }
 
        // Create new stochastic node
        StochasticNode* node = new StochasticNode(dist);

        // Does the variable exist?
        if (!variable->isMember() && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }
        else {

            // It exists - use the frame or a complex object to assign to it
            if (!variable->isMember()) {
                if (index.size() != 0)
                    frame->setVarElement(varName, index, node);
                else
                    frame->setVariable(varName, node);
            }
            else {

                PRINTF ("Assigning distribution to existing member variable\n");
                const RbComplex* objectPtr = dynamic_cast<const RbComplex*>(variable->getVariable()->getValuePtr(frame));
                if (objectPtr == NULL)
                    throw RbException("Variable " + variable->getVariable()->getFullName(frame) + " does not have members");
                RbComplex* theObject = const_cast<RbComplex*>(objectPtr);
                if (index.size() == 0)
                    theObject->setVariable(varName, node);
                else {
                    const DAGNodeContainer* containerPtr = dynamic_cast<const DAGNodeContainer*>(theObject->getVariable(varName));
                    if (containerPtr == NULL)
                        throw RbException("Variable " + variable->getVariable()->getFullName(frame) + "." + std::string(varName) + " does not have elements");
                    DAGNodeContainer* container = const_cast<DAGNodeContainer*>(containerPtr);
                    container->setElement(index, node);
                }
            }
        }
    }

    // Return copy of the value of the rhs expression
    return expression->getValue(frame);
}


/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {

    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}
