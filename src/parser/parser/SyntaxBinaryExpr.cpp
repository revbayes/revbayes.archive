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
#include <vector>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxBinaryExpr.h"
#include "VectorString.h"
#include "Workspace.h"


// Definition of the static type spec member
const TypeSpec SyntaxBinaryExpr::typeSpec(SyntaxBinaryExpr_name);

/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, SyntaxElement* lhs, SyntaxElement* rhs) : SyntaxElement(), leftOperand(lhs), rightOperand(rhs), operation(op) {

}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x) : SyntaxElement(x) {

    leftOperand  = x.leftOperand->clone();
    rightOperand = x.rightOperand->clone();
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr() {
    
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x) {

    if (&x != this) {

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

    return (new SyntaxBinaryExpr(*this));
}


/** Get class vector describing type of object */
const VectorString& SyntaxBinaryExpr::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxBinaryExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** We cannot perform this function and throw and error */
Variable* SyntaxBinaryExpr::evaluateContent( ) {
    throw RbException("Cannot evaluate the content in SyntaxBinaryExpr without environment!");
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
Variable* SyntaxBinaryExpr::evaluateContent( Environment& env) {

    // Package the arguments
    std::vector<Argument*> args;
    Variable* left = leftOperand->evaluateContent(env);
    args.push_back( new Argument("", left ) );
    Variable* right = rightOperand->evaluateContent(env);
    args.push_back(new Argument("", right ) );

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    
    RbFunction* theFunction = Workspace::globalWorkspace().getFunction(funcName, args);
    
    return new Variable(new DeterministicNode( theFunction ) );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxBinaryExpr::getTypeSpec(void) const {
    return typeSpec;
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

