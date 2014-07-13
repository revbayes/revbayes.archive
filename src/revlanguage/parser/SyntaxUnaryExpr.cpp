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
#include "Environment.h"
#include "RbException.h"
#include "RlFunction.h"
#include "RbUtil.h"
#include "SyntaxUnaryExpr.h"
#include "Workspace.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace RevLanguage;

/** Static vector of RlStrings giving names of operator types */
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


/** Convert element to DAG node expression */
RevPtr<Variable> SyntaxUnaryExpr::evaluateContent(Environment& env) {

    // Package the argument
    std::vector<Argument> arg;
    arg.push_back( Argument( expression->evaluateContent(env), "" ) );

    // Find the function
    std::string funcName = "_" + opCode[operation];
    Function* func = Workspace::globalWorkspace().getFunction(funcName, arg).clone();
    func->processArguments( arg );
    
    RevPtr<Variable> funcReturnValue = func->execute();
    delete func;

    // Return new function node
    return funcReturnValue;
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

