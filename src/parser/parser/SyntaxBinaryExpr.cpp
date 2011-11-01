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
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, RbPtr<SyntaxElement> lhs, RbPtr<SyntaxElement> rhs) : SyntaxElement(), leftOperand(lhs), rightOperand(rhs), operation(op) {

}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x) : SyntaxElement(x) {

    leftOperand  = RbPtr<SyntaxElement>( x.leftOperand->clone() );
    rightOperand = RbPtr<SyntaxElement>( x.rightOperand->clone() );
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr() {
    
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        leftOperand  = RbPtr<SyntaxElement>( x.leftOperand->clone() );
        rightOperand = RbPtr<SyntaxElement>( x.rightOperand->clone() );
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


/** Get class vector describing type of object */
const VectorString& SyntaxBinaryExpr::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxBinaryExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RbPtr<Variable> SyntaxBinaryExpr::evaluateContent(RbPtr<Environment> env) {

    // Package the arguments
    std::vector<RbPtr<Argument> > args;
    RbPtr<Variable> left = leftOperand->evaluateContent(env);
    args.push_back(RbPtr<Argument>(new Argument("", left ) ));
    RbPtr<Variable> right = rightOperand->evaluateContent(env);
    args.push_back(RbPtr<Argument>(new Argument("", right ) ));

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    
    RbPtr<RbFunction> theFunction = Workspace::globalWorkspace()->getFunction(funcName, args);
    
    return RbPtr<Variable>(new Variable(RbPtr<DAGNode>(new DeterministicNode( theFunction )) ) );
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

