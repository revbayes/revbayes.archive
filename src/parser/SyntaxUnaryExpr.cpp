/**
 * @file
 * This file contains the implementation of SyntaxUnaryExpr, which is
 * used to hold unary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxUnaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <vector>
#include <sstream>

#include "Argument.h"
#include "DAGNode.h"
#include "FunctionNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "SyntaxUnaryExpr.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxUnaryExpr::opCode[] = { "uminus", "uplus", "unot" };


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


/** Assignment operator */
SyntaxUnaryExpr& SyntaxUnaryExpr::operator=(const SyntaxUnaryExpr& x) {

    if (&x != this) {

        delete expression;

        SyntaxElement::operator=(x);

        expression  = x.expression->clone();
        operation   = x.operation;
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxUnaryExpr::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxUnaryExpr: operation = " << opCode[operation];

    return o.str();
}


/** Clone syntax element */
SyntaxUnaryExpr* SyntaxUnaryExpr::clone () const {

    return new SyntaxUnaryExpr(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxUnaryExpr::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxUnaryExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node expression */
DAGNode* SyntaxUnaryExpr::getDAGNodeExpr(Frame* frame) const {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back(Argument("", expression->getDAGNodeExpr(frame)));

    // Find the function
    std::string funcName = "_" + opCode[operation];
    RbFunction *func = Workspace::globalWorkspace().getFunction(funcName, arg);

    // Return new function node
    return new FunctionNode(func);
}


/** Look up the function and calculate the value. The argument is a value argument, so it is safe to get its value. */
DAGNode* SyntaxUnaryExpr::getValue(Frame* frame) const {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back(Argument("", expression->getValue(frame)));

    // Execute function and return value
    std::string funcName = "_" + opCode[operation];
    return Workspace::globalWorkspace().executeFunction(funcName, arg);
}


/** Print info about the syntax element */
void SyntaxUnaryExpr::print(std::ostream& o) const {

    o << "[" << this << "] SyntaxUnaryExpr:" << std::endl;
    o << "expression    = [" << expression << "] " << expression->briefInfo() << std::endl;
    o << "operation     = " << opCode[operation];
    o << std::endl;

    expression->print(o);
}

