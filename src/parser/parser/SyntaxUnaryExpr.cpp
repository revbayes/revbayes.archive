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
#include "ConstArgument.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "Plate.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "SyntaxUnaryExpr.h"
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
RbPtr<Variable> SyntaxUnaryExpr::evaluateContent(Environment& env) {

    // Package the argument
    std::vector<RbPtr<Argument> > arg;
    arg.push_back( new ConstArgument( RbPtr<const Variable>( (Variable *)expression->evaluateContent(env)) , "" ) );

    // Find the function
    std::string funcName = "_" + opCode[operation];
    RbFunction* func = Workspace::globalWorkspace().getFunction(funcName, arg).clone();
    func->processArguments( arg );

    // Return new function node
    return RbPtr<Variable>( new Variable( new DeterministicNode(func, NULL) ) );
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


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the element.
 */
void SyntaxUnaryExpr::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    
    // the expression itself
    expression->replaceVariableWithConstant(name, c);
    
}

