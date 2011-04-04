/**
 * @file
 * This file contains the implementation of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxBinaryExpr
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

#include "Argument.h"
#include "DAGNode.h"
#include "FunctionNode.h"
#include "RbException.h"
#include "SyntaxBinaryExpr.h"
#include "VectorString.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, SyntaxElement* lhs, SyntaxElement* rhs) 
    : SyntaxElement(), leftOperand(lhs), rightOperand(rhs), operation(op) {
}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x)
    : SyntaxElement(x) {

    leftOperand  = x.leftOperand->clone();
    rightOperand = x.rightOperand->clone();
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr() {
    
    delete leftOperand;
    delete rightOperand;
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x) {

    if (&x != this) {
    
        delete leftOperand;
        delete rightOperand;

        SyntaxElement::operator=(x);

        leftOperand  = x.leftOperand->clone();
        rightOperand = x.rightOperand->clone();
        operation    = x.operation;
    }

    return *this;
}


/** Return brief info about object */
std::string SyntaxBinaryExpr::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxBinaryExpr: operation = " << opCode[operation];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxBinaryExpr::clone () const {

    return (SyntaxElement*)(new SyntaxBinaryExpr(*this));
}


/** Convert element to DAG node */
DAGNode* SyntaxBinaryExpr::getDAGNodeExpr(Frame* frame) const {

    // Package arguments
    std::vector<Argument> args;
    args.push_back(Argument("", leftOperand->getDAGNodeExpr(frame)));
    args.push_back(Argument("", rightOperand->getDAGNodeExpr(frame)));

    // Get function
    std::string funcName = "." + opCode[operation];
    RbFunction *func = Workspace::globalWorkspace().getFunction(funcName, args);

    // Return new function node
    return new FunctionNode(func);
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
DAGNode* SyntaxBinaryExpr::getValue(Frame* frame) const {

    // Package the arguments
    std::vector<Argument> args;
    args.push_back(Argument("", leftOperand->getDAGNodeExpr(frame)));
    args.push_back(Argument("", rightOperand->getDAGNodeExpr(frame)));

    // Execute function and return value
    std::string funcName = "_" + opCode[operation];
    return Workspace::globalWorkspace().executeFunction(funcName, args);
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::print(std::ostream& o) const {

    o << "[" << this << "] SyntaxBinaryExpr:" << std::endl;
    o << "left operand  = [" << leftOperand  << "]" << leftOperand->briefInfo()  << std::endl;
    o << "right operand = [" << rightOperand << "]" << rightOperand->briefInfo() << std::endl;
    o << "operation     = " << opCode[operation];
    o << std::endl;

    leftOperand->print(o);
    rightOperand->print(o);
}

