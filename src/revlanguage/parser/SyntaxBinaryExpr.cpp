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
#include "RbException.h"
#include "RlFunction.h"
#include "RbUtil.h"
#include "SyntaxBinaryExpr.h"
#include "Workspace.h"

using namespace RevLanguage;

/** Static vector of RlStrings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "mod", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, SyntaxElement* lhs, SyntaxElement* rhs) : SyntaxElement(),
    leftOperand(lhs),
    rightOperand(rhs),
    operation(op)
{

}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x) : SyntaxElement(x)
{

    leftOperand  = x.leftOperand->clone();
    rightOperand = x.rightOperand->clone();
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr()
{
    delete leftOperand;
    delete rightOperand;
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x)
{

    if (&x != this)
    {
        delete leftOperand;
        delete rightOperand;

        SyntaxElement::operator=(x);

        leftOperand  = x.leftOperand->clone();
        rightOperand = x.rightOperand->clone();
        operation    = x.operation;
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxBinaryExpr::clone () const
{

    return (new SyntaxBinaryExpr(*this));
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RevPtr<Variable> SyntaxBinaryExpr::evaluateContent( Environment& env)
{

    // Package the arguments
    std::vector<Argument> args;
    RevPtr<Variable> left = leftOperand->evaluateContent(env);
    args.push_back( Argument( left, "") );
    RevPtr<Variable> right = rightOperand->evaluateContent(env);
    args.push_back( Argument( right, "") );

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    
    Function* theFunction = Workspace::globalWorkspace().getFunction(funcName, args).clone();
    theFunction->processArguments( args );
    
    RevPtr<Variable> theReturnValue = theFunction->execute();
    
    // free the memory of the function
    delete theFunction;
    
    return theReturnValue;
}


/** Is the expression constant?
 *  Only if the arguments are constant.
 */
bool SyntaxBinaryExpr::isConstExpression(void) const
{
    return leftOperand->isConstExpression() && rightOperand->isConstExpression();
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::printValue(std::ostream& o) const
{

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

