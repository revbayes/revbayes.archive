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

#include "ConstArgument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Plate.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "SyntaxBinaryExpr.h"
#include "Workspace.h"



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


/** Clone syntax element */
SyntaxElement* SyntaxBinaryExpr::clone () const {

    return (new SyntaxBinaryExpr(*this));
}


/** Get class name of object */
const std::string& SyntaxBinaryExpr::getClassName(void) { 
    
    static std::string rbClassName = "Binary expression";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxBinaryExpr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxBinaryExpr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RbPtr<Variable> SyntaxBinaryExpr::evaluateContent( Environment& env) {

    // Package the arguments
    std::vector<RbPtr<Argument> > args;
    RbPtr<const Variable> left( (Variable *) leftOperand->evaluateContent(env) );
    args.push_back( new ConstArgument(left, "") );
    RbPtr<const Variable> right( (Variable *) rightOperand->evaluateContent(env) );
    args.push_back( new ConstArgument(right, "") );

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    
    RbFunction* theFunction = Workspace::globalWorkspace().getFunction(funcName, args).clone();
    theFunction->processArguments( args );
    
    return RbPtr<Variable>( new Variable(new DeterministicNode( theFunction, NULL ) ) );
}


/** Is the expression constant?
 *  Only if the arguments are constant.
 */
bool SyntaxBinaryExpr::isConstExpression(void) const {
    return leftOperand->isConstExpression() && rightOperand->isConstExpression();
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxBinaryExpr:" << std::endl;
    o << "left operand  = [" << leftOperand  << "]";
    leftOperand->printValue(o);
    o << std::endl;
    o << "right operand = [" << rightOperand << "]" ;
    rightOperand->printValue(o);
    o << std::endl;
    o << "operation     = " << opCode[operation];
    o << std::endl;

    leftOperand->printValue(o);
    rightOperand->printValue(o);
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to both elements.
 */
void SyntaxBinaryExpr::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    leftOperand->replaceVariableWithConstant(name, c);
    rightOperand->replaceVariableWithConstant(name, c);
}

