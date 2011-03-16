/**
 * @file
 * This file contains the implementation of SyntaxAssignExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <list>
#include <sstream>

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodePlate.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"
#include "StochasticNode.h"
#include "VectorString.h"
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
const VectorString& SyntaxAssignExpr::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxAssignExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node: insert symbol and return reference to it */
DAGNode* SyntaxAssignExpr::getDAGNodeExpr(Frame* frame) const {

    // Insert symbol; discard the return value
    delete getValue(frame);

    // Return DAG node from variable
    return variable->getDAGNodeExpr();
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RbObject* SyntaxAssignExpr::getValue(Frame* frame) const {

    PRINTF("Evaluating assign expression\n");

    // Get variable name, index and frame
    RbString      varName  = *variable->getIdentifier();
    VectorInteger index    = variable->getIndex(frame);
    Frame*        varFrame = variable->getVariableFrame(frame);

    // Variable storing the value of the expression if an arrow assignment
    RbObject* exprValue;

    // Deal with arrow assignments
    if (opType == ArrowAssign) {
        PRINTF("Arrow assignment\n");

        // Calculate the value of the rhs expression
        exprValue = expression->getValue(frame);

        // Does the variable exist? If not, add it - but only if it lives in current workspace
        if ( varFrame == frame  && !frame->existsVariable(varName)) {

            // It does not exist - add it to current frame
            PRINTF ("Creating new constant node %sthrough assignment\n", index.size()==0? "" : "element ");
            if (index.size() != 0)
                frame->addVariable(varName, index, new ConstantNode(exprValue));
            else
                frame->addVariable(varName, new ConstantNode(exprValue));
		}
        else {

            // It exists - use the variable frame to assign to it
            PRINTF ("Replacing %s value %sthrough assignment\n", varFrame != frame ? "member" : "simple", index.size()==0 ? "" : "element ");
            if (index.size() != 0)
                varFrame->setValElement(varName, index, exprValue);
            else
                varFrame->setValue(varName, exprValue);
		}
    }

    // Deal with equation assignments
    else if (opType == EquationAssign) {

        PRINTF("Equation assignment\n");

        // Get DAG node representation of expression
        DAGNode* dag = expression->getDAGNodeExpr(frame);
        DeterministicNode* node = dynamic_cast<DeterministicNode*>(dag);
        if (node == NULL) {
            delete dag;
            throw (RbException("Right-hand side is not a variable expression"));
        }

        // Does the variable exist?
        if ( varFrame == frame && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            PRINTF ("Creating new deterministic node %sthrough assignment\n", index.size()==0? "" : "element ");
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }
        else {

            // It exists - use the variable frame to assign to it
            PRINTF ("Replacing %s deterministic node %sthrough assignment\n", varFrame != frame ? "member" : "simple", index.size()==0 ? "" : "element ");
            if (index.size() != 0)
                varFrame->setVarElement(varName, index, node);
            else
                varFrame->setVariable(varName, node);
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
        if (varFrame == frame && !frame->existsVariable(varName)) {
 
            // It does not exist - add it
            PRINTF ("Creating new stochastic node %sthrough assignment\n", index.size()==0? "" : "element ");
            if (index.size() != 0)
                frame->addVariable(varName, index, node);
            else
                frame->addVariable(varName, node);
        }
        else {

            // It exists - use the variable frame to assign to it
            PRINTF ("Replacing %s stochastic node %sthrough assignment\n", varFrame != frame ? "member" : "simple", index.size()==0 ? "" : "element ");
            if (index.size() != 0)
                varFrame->setVarElement(varName, index, node);
            else
                varFrame->setVariable(varName, node);
        }
    }

    // Return copy of the value of the rhs expression
    if (opType == ArrowAssign)
        return exprValue->clone();
    else
        return expression->getValue(frame);
}


/** Print info about the syntax element */
void SyntaxAssignExpr::print(std::ostream& o) const {

    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[opType];
}
