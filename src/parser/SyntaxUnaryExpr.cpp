/**
 * @file
 * This file contains the implementation of SyntaxUnaryExpr, which is
 * used to hold unary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxUnaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <vector>
#include <sstream>

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "SyntaxUnaryExpr.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxUnaryExpr::opCode[] = { "UMINUS", "UPLUS", "UNOT" }; 


/** Construct from operator type and operands */
SyntaxUnaryExpr::SyntaxUnaryExpr(operatorT op, SyntaxElement* expr) 
    : SyntaxElement(), expression(expr), operation(op) {
}


/** Deep copy constructor */
SyntaxUnaryExpr::SyntaxUnaryExpr(const SyntaxUnaryExpr& x)
    : SyntaxElement(x) {

    expression  = x.expression->clone();
    operation   = x.operation;
}


/** Destructor deletes expression */
SyntaxUnaryExpr::~SyntaxUnaryExpr() {
    
    delete expression;
}


/** Return brief info about object */
std::string SyntaxUnaryExpr::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxUnaryExpr: operation = " << opCode[operation];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxUnaryExpr::clone () const {

    return (SyntaxElement*)(new SyntaxUnaryExpr(*this));
}


/** Equals comparison */
bool SyntaxUnaryExpr::equals(const SyntaxElement* elem) const {

	const SyntaxUnaryExpr* p = dynamic_cast<const SyntaxUnaryExpr*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && expression->equals(p->expression);
    result = result && (operation == p->operation);
    
    return result;
}


/** Convert element to DAG node */
DAGNode* SyntaxUnaryExpr::getDAGNode(Frame* frame) const {

    std::vector<Argument> arg;
    arg.push_back(Argument("x", expression->getDAGNode(frame)));

    std::string funcName = "." + opCode[operation];
    RbFunction *func = Workspace::globalWorkspace().getFunction(funcName, arg);

    return new DeterministicNode((RbFunction*)(func->clone()), arg);
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RbObject* SyntaxUnaryExpr::getValue(Frame* frame) const {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back(Argument("x", expression->getDAGNode(frame)));

    // Execute function
    std::string funcName = "." + opCode[operation];
    const RbObject *retVal = Workspace::globalWorkspace().executeFunction(funcName, arg);

    // Return value (we pass on management responsibility to caller)
    return retVal->clone();
}


/** Print info about the syntax element */
void SyntaxUnaryExpr::print(std::ostream& o) const {

    o << "SyntaxUnaryExpr:" << std::endl;
    o << "expression    = " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[operation];
}


