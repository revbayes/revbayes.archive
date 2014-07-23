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

/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "mod", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr( operatorT op, SyntaxElement* lhs, SyntaxElement* rhs ) :
    SyntaxElement(),
    leftOperand( lhs ),
    rightOperand( rhs ),
    operation( op )
{
}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr( const SyntaxBinaryExpr& x ) :
    SyntaxElement(x)
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
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=( const SyntaxBinaryExpr& x )
{

    if ( &x != this )
    {
        SyntaxElement::operator=( x );
        
        delete leftOperand;
        delete rightOperand;

        leftOperand  = x.leftOperand->clone();
        rightOperand = x.rightOperand->clone();
        operation    = x.operation;
    }

    return *this;
}


/** Type-safe clone of syntax element */
SyntaxElement* SyntaxBinaryExpr::clone () const
{
    return (new SyntaxBinaryExpr(*this));
}


/**
 * @brief Get semantic value (static version)
 *
 * We simply look up the function and calculate the value. Note that
 * we look up the function in the global workspace, which means that
 * we do not allow users to override internal functions corresponding
 * to binary expressions.
 *
 * Currently, we call the standard execute() method of the function,
 * which is typically set up to produce a deterministic variable.
 * This is more appropriate behavior for the dynamic version of this
 * function.
 *
 * It is not necessary to clone the function, we simply clear out
 * the arguments after we are done.
 *
 * Note that we call the static evaluate function of the arguments,
 * and make sure we return a constant value.
 *
 * @todo Support this evaluation context better
 */
RevPtr<Variable> SyntaxBinaryExpr::evaluateContent( Environment& env )
{

    // Package the arguments
    std::vector<Argument> args;

    RevPtr<Variable> left = leftOperand->evaluateContent( env );
    args.push_back( Argument( left, "" ) );

    RevPtr<Variable> right = rightOperand->evaluateContent( env );
    args.push_back( Argument( right, "" ) );

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[ operation ];
    Function& theFunction = Workspace::globalWorkspace().getFunction( funcName, args );
    theFunction.processArguments( args );
    
    RevPtr<Variable> theReturnValue = theFunction.execute();
    
    // Clear the arguments in the function
    theFunction.clear();
    
    // Return the return value of the function after making it constant
    if ( theReturnValue != NULL )
        theReturnValue->getRevObject().makeConstantValue();
    return theReturnValue;
}


/**
 * @brief Get semantic value (dynamic version)
 *
 * In the dynamic version of this function, we look up the function
 * and ask it to generate a deterministic variable with the function
 * inside. For now, we simply call the standard execute() function,
 * which is set up to produce a deterministic variable in most
 * functions.
 *
 * Note that we look up the function in the global workspace, which means
 * that we do not allow users to override internal functions corresponding
 * to binary expressions.
 *
 * Even in the dynamic version of this function, it is not necessary to
 * clone the function. We simply use it to generate a deterministic
 * variable after setting the appropriate arguments. After executing
 * the function, we make sure to clear out the arguments of the function,
 * so that it is ready for the next round of execution.
 *
 * Unlike the static version of this function, we call the dynamic evaluate
 * function of the arguments. We also return the return value as is, without
 * making it a constant value first.
 */
RevPtr<Variable> SyntaxBinaryExpr::evaluateDynamicContent( Environment& env )
{
    
    // Package the arguments
    std::vector<Argument> args;
    
    RevPtr<Variable> left = leftOperand->evaluateContent( env );
    args.push_back( Argument( left, "" ) );
    
    RevPtr<Variable> right = rightOperand->evaluateContent( env );
    args.push_back( Argument( right, "" ) );
    
    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[ operation ];
    Function& theFunction = Workspace::globalWorkspace().getFunction( funcName, args );
    theFunction.processArguments( args );
    
    RevPtr<Variable> theReturnValue = theFunction.execute();
    
    // Clear the arguments in the function
    theFunction.clear();
    
    return theReturnValue;
}


/** Is the expression constant?
 *  Only if the arguments are constant.
 */
bool SyntaxBinaryExpr::isConstExpression( void ) const
{
    return leftOperand->isConstExpression() && rightOperand->isConstExpression();
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::printValue( std::ostream& o ) const
{
    o << "[" << this << "] SyntaxBinaryExpr:" << std::endl;

    o << "left operand  = [" << leftOperand  << "]";
    leftOperand->printValue( o );
    o << std::endl;

    o << "right operand = [" << rightOperand << "]" ;
    rightOperand->printValue( o );
    o << std::endl;

    o << "operation     = " << opCode[operation];
    o << std::endl;
}

