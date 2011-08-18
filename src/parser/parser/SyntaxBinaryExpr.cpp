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
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxBinaryExpr.h"
#include "VectorString.h"
#include "Workspace.h"


/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, SyntaxElement* lhs, SyntaxElement* rhs) : SyntaxElement(), leftOperand(lhs), rightOperand(rhs), operation(op) {
    leftOperand->retain();
    rightOperand->retain();
}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x) : SyntaxElement(x) {

    leftOperand  = x.leftOperand->clone();
    leftOperand->retain();
    rightOperand = x.rightOperand->clone();
    rightOperand->retain();
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr() {
    
    // delete leftOperand;
    if (leftOperand != NULL) {
        leftOperand->release();
        if (leftOperand->isUnreferenced()) {
            delete leftOperand;
        }
    }
    
    //delete rightOperand;
    if (rightOperand != NULL) {
        rightOperand->release();
        if (rightOperand->isUnreferenced()) {
            delete rightOperand;
        }
    }
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x) {

    if (&x != this) {
        
        // delete leftOperand;
        if (leftOperand != NULL) {
            leftOperand->release();
            if (leftOperand->isUnreferenced()) {
                delete leftOperand;
            }
        }
        
        //delete rightOperand;
        if (rightOperand != NULL) {
            rightOperand->release();
            if (rightOperand->isUnreferenced()) {
                delete rightOperand;
            }
        }

        SyntaxElement::operator=(x);

        leftOperand  = x.leftOperand->clone();
        leftOperand->retain();
        rightOperand = x.rightOperand->clone();
        rightOperand->retain();
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


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
Variable* SyntaxBinaryExpr::getContentAsVariable(Environment* env) const {

    // Package the arguments
    std::vector<Argument*> args;
    DAGNode *left = leftOperand->getContentAsVariable(env)->getDagNodePtr();
    args.push_back(new Argument("", left->getVariable() ));
    DAGNode *right = rightOperand->getContentAsVariable(env)->getDagNodePtr();
    args.push_back(new Argument("", right->getVariable() ));

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    RbFunction *theFunction = Workspace::globalWorkspace().getFunction(funcName, args);
    
    return new Variable(new DeterministicNode( theFunction ));
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

