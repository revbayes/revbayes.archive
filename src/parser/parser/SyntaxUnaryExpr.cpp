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


/** Get class name of object */
const std::string& SyntaxUnaryExpr::getClassName(void) { 
    
    static std::string rbClassName = "Unary expression";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxUnaryExpr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxUnaryExpr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Convert element to DAG node expression */
RbVariablePtr SyntaxUnaryExpr::evaluateContent(Environment& env) {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back( Argument("", expression->evaluateContent(env) ) );

    // Find the function
    std::string funcName = "_" + opCode[operation];
    RbFunction* func = Workspace::globalWorkspace().getFunction(funcName, arg);

    // Return new function node
    return RbVariablePtr( new Variable( new DeterministicNode(func) ) );
}


/** Is the expression constant?
 *  Only if the argument is constant.
 */
bool SyntaxUnaryExpr::isConstExpression(void) const {
    return expression->isConstExpression();
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

