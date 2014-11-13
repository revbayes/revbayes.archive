#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "Argument.h"
#include "Environment.h"
#include "Integer.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlString.h"
#include "SyntaxFunctionCall.h"
#include "RevVariable.h"
#include "Workspace.h"
#include "SyntaxVariable.h"

using namespace RevLanguage;


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable( const std::string &n ) : SyntaxElement(),
    identifier( n )
{
    
}


/** Destructor deletes base variable, expression, and index */
SyntaxVariable::~SyntaxVariable()
{
    
}



/** Type-safe clone of syntax element */
SyntaxVariable* SyntaxVariable::clone () const
{
    return new SyntaxVariable( *this );
}


/**
 * @brief Evaluate rhs content
 *
 * This function returns the semantic value of the variable expression
 * when it is part of a dynamic expression, that is, the right-hand side
 * of an equation (deterministic) or tilde (stochastic) assignment.
 *
 * If the parameter dynamic == true, then we need a dynamic evaluation.
 * The method behaves the same except that control variables need to return
 * clones of themselves (temporary variables) rather than themselves,
 * so that they are not included in the DAG.
 */
RevPtr<RevVariable> SyntaxVariable::evaluateContent( Environment& env, bool dynamic)
{
    
    RevPtr<RevVariable> theVar;
    
    // Get variable from the environment (no dynamic version of identifier)
    theVar = env.getVariable( identifier );
    
    
    if ( theVar->isVectorVariable() )
    {
        int min = theVar->getMinIndex();
        int max = theVar->getMaxIndex();
        if ( min > max )
        {
            throw RbException("Cannot create a vector variable with name '" + identifier + "' because it doesn't have elements.");
        }
        std::vector<Argument> args;
        for (int i=min; i<=max; ++i)
        {
            std::string elementIdentifier = identifier + "[" + i + "]";
            RevPtr<RevVariable>& elementVar = env.getVariable( elementIdentifier );
            // check that the element is not NULL
            if ( elementVar == NULL || elementVar->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException("Cannot create vector variable with name '" + identifier + "' because element with name '" + elementIdentifier + "' is NULL." );
            }
            args.push_back( Argument( elementVar ) );
        }
        Function* func = Workspace::userWorkspace().getFunction("v",args,!dynamic).clone();
        func->processArguments(args,false);
        
        // Evaluate the function (call the static evaluation function)
        RevPtr<RevVariable> funcReturnValue = func->execute();
        
        // free the memory of our copy
        delete func;
        
        theVar->replaceRevObject( funcReturnValue->getRevObject().clone() );
        
    }
    
    if ( dynamic == true )
    {
        // Check whether we have a control variable and make a clone in that case
        if ( theVar->isWorkspaceVariable() )
        {
            theVar = new RevVariable( theVar->getRevObject().clone() );
        }
        
    }
    
    // Return the variable for assignment
    return theVar;
}



/**
 * @brief Evaluate left-hand-side content
 *
 * This function is similar to evaluateContent(). However, we
 * do not throw an error if the variable does not exist in the
 * frame; instead, we create and return a new null variable.
 */
RevPtr<RevVariable> SyntaxVariable::evaluateLHSContent( Environment& env, const std::string& elemType )
{
    RevPtr<RevVariable> theVar;
    
    // Find or create the variable
    if ( env.existsVariable( identifier ) )
    {
        theVar = env.getVariable( identifier );
    }
    else    // add it
    {
        theVar = new RevVariable( NULL, identifier );
        env.addVariable( identifier, theVar );
    }
    
    // Return the variable for assignment
    return theVar;
}


/**
 * Return nice representation of the syntax element.
 */
std::string SyntaxVariable::getFullName( Environment& env ) const
{
    std::ostringstream theName;
    
    theName << identifier;
    
    return theName.str();
}


/** Print info about the syntax element */
void SyntaxVariable::printValue(std::ostream& o) const
{
    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier      = \"" << identifier << "\"" << std::endl;
    
    o << std::endl;
    
}




/**
 * Is the syntax element safe for use in a function ( as
 * opposed to a procedure)? The variable element is safe
 * if it does not include an expression that is not function-
 * safe.
 */
bool SyntaxVariable::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    
    return true;
}


/**
 * Check whether this syntax element retrieves an external variable.
 * In certain contexts, this leads to statements that are not function-
 * safe. This function is used during compilation when checking whether
 * a statement is safe for inclusion in a function.
 *
 * If there is a base variable, we delegate to the base variable because
 * it is the first element in the base variable / variable expression
 * chain that determines whether an external variable is retrieved.
 *
 * A function-call variable expression never retrieves an external variable
 * unless it is a procedure call. In the latter case, an external variable
 * may be returned but the variable expression is not function-safe
 * anyway (see the function isFunctionSafe()), so we do not care about
 * this possibility here.
 *
 * A generic variable expression may retrieve an external variable. This
 * is the case in variable expressions of the type '(a)[1]', where 'a'
 * is an external variable. Therefore we need to ask the expression
 * whether it retrieves an external variable.
 *
 * If this element is a named variable expression, we simply check the
 * external environment to see whether the variable exists there.
 */
bool SyntaxVariable::retrievesExternVar( const Environment& env, std::set<std::string>& localVars, bool inLHS ) const
{
    
    // Named variable. Check if it is already defined as a local variable.
    if ( localVars.find( identifier ) != localVars.end() )
        return false;

    // Check whether we can and should add it as a local variable.
    if ( !env.existsVariable( identifier ) )
    {
        if ( !inLHS )
            throw RbException( "No variable named '" + identifier + "'" );

        localVars.insert( identifier );

        return false;
    }
    else
    {
        // If we are in an LHS expression, we should add the variable to
        // the local variables
        if ( inLHS )
        {
            localVars.insert( identifier );
            return false;
        }
        else
        {
            return true;
        }
    }
}

