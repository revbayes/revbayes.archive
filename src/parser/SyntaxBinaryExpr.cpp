/**
 * @file
 * This file contains the implementation of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxBinaryExpr
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

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "SyntaxBinaryExpr.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "RANGE", "ADD", "SUB", "MUL", "DIV", "EXP", "LT", "LE",
                                           "EQ", "NE", "GE", "GT", "AND", "OR", "AND2", "OR2"}; 


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


/** Equals comparison */
bool SyntaxBinaryExpr::equals(const SyntaxElement* elem) const {

	const SyntaxBinaryExpr* p = dynamic_cast<const SyntaxBinaryExpr*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && leftOperand->equals(p->leftOperand);
    result = result && rightOperand->equals(p->rightOperand);
    result = result && (operation == p->operation);
    
    return result;
}


/** Convert element to DAG node */
DAGNode* SyntaxBinaryExpr::getDAGNode(Environment* env) const {

    std::vector<Argument> args;
    args.push_back(Argument("x", leftOperand->getDAGNode(env)));
    args.push_back(Argument("y", rightOperand->getDAGNode(env)));

    std::string funcName = "." + opCode[operation];
    RbFunction *func = Workspace::globalWorkspace().getFunction(funcName, args);

    return new DeterministicNode((RbFunction*)(func->clone()), args);
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RbObject* SyntaxBinaryExpr::getValue(Environment* env) {

    // Package the arguments
    std::vector<Argument> args;
    args.push_back(Argument("x", leftOperand->getDAGNode(env)));
    args.push_back(Argument("y", rightOperand->getDAGNode(env)));

    // Get a function pointer
    std::string funcName = "." + opCode[operation];
    RbFunction *func = Workspace::globalWorkspace().getFunction(funcName, args);

    // Return value (we pass on management responsibility to caller)
    return func->execute(args);
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::print(std::ostream& o) const {

    o << "SyntaxBinaryExpr:" << std::endl;
    o << "left operand  = " << leftOperand->briefInfo() << std::endl;
    o << "right operand = " << rightOperand->briefInfo() << std::endl;
    o << "operation     = " << opCode[operation];
}


