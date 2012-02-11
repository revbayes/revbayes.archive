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

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxUnaryExpr.h"
#include "VectorString.h"
#include "Workspace.h"

#include <iostream>
#include <vector>
#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxUnaryExpr::typeSpec(SyntaxUnaryExpr_name);

/** Static vector of strings giving names of operator types */
std::string SyntaxUnaryExpr::opCode[] = { "uminus", "uplus", "unot" };


/** Construct from operator type and operands */
SyntaxUnaryExpr::SyntaxUnaryExpr(operatorT op, SyntaxElement* expr)  : SyntaxElement(), expression(expr), operation(op) {
}


/** Deep copy constructor */
SyntaxUnaryExpr::SyntaxUnaryExpr(const SyntaxUnaryExpr& x) : SyntaxElement(x) {

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

        SyntaxElement::operator=(x);

        expression  = x.expression->clone();
        operation   = x.operation;
    }

    return (*this);
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


/** We cannot perform this function and throw and error */
Variable* SyntaxUnaryExpr::evaluateContent( void ) {
    throw RbException("Cannot evaluate the content in SyntaxUnaryExpr without environment!");
}


/** Convert element to DAG node expression */
Variable* SyntaxUnaryExpr::evaluateContent(Environment& env) {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back( Argument("", expression->evaluateContent(env) ) );

    // Find the function
    std::string funcName = "_" + opCode[operation];
    RbFunction* func = Workspace::globalWorkspace().getFunction(funcName, arg);

    // Return new function node
    return new Variable( new DeterministicNode(func) );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxUnaryExpr::getTypeSpec(void) const {
    return typeSpec;
}



/** Print info about the syntax element */
void SyntaxUnaryExpr::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxUnaryExpr:" << std::endl;
    o << "expression    = [" << expression << "] ";
    expression->printValue(o);
    o << std::endl;
    o << "operation     = " << opCode[operation];
    o << std::endl;

    expression->printValue(o);
}

