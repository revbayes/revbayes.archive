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
#include "Variable.h"
#include "Workspace.h"
#include "SyntaxVariable.h"

using namespace RevLanguage;


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable( const std::string &n ) :
SyntaxElement(),
identifier( n ),
baseVariable( NULL )
{
}


/** Construct from base variable (member object), identifier and index */
SyntaxVariable::SyntaxVariable( SyntaxElement* baseVar, const std::string &n ) :
SyntaxElement(),
identifier( n ),
baseVariable( baseVar )
{
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x) :
SyntaxElement( x ),
identifier( x.identifier )
{
    
    if ( x.baseVariable != NULL )
    {
        baseVariable = x.baseVariable->clone();
    }
    else
    {
        baseVariable = NULL;
    }
    
}


/** Destructor deletes base variable, expression, and index */
SyntaxVariable::~SyntaxVariable()
{
    
    delete baseVariable;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=( const SyntaxVariable& x )
{
    if (&x != this)
    {
        SyntaxElement::operator=( x );
        
        
        delete baseVariable;
        
        identifier   = x.identifier;
        
        if ( x.baseVariable != NULL )
        {
            baseVariable = x.baseVariable->clone();
        }
        else
        {
            baseVariable = NULL;
        }
    }
    
    return (*this);
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
RevPtr<Variable> SyntaxVariable::evaluateContent( Environment& env, bool dynamic)
{
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        // Get variable from the environment (no dynamic version of identifier)
        theVar = env.getVariable( identifier );
    }
    else
    {
        
        // Get the base variable
        theVar = baseVariable->evaluateContent( env, dynamic );
        
        // Find member variable (no dynamic version of identifier)
        theVar = theVar->getRevObject().getMember( identifier );
    }
    
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
            RevPtr<Variable>& elementVar = env.getVariable( elementIdentifier );
            // check that the element is not NULL
            if ( elementVar == NULL || elementVar->getRevObject() == RevNullObject::getInstance() )
            {
                throw RbException("Cannot create vector variable with name '" + identifier + "' because element with name '" + elementIdentifier + "' is NULL." );
            }
            args.push_back( Argument( elementVar ) );
        }
        Function& func = Workspace::userWorkspace().getFunction("v",args,false);
        func.processArguments(args,false);
        
        // Evaluate the function (call the static evaluation function)
        RevPtr<Variable> funcReturnValue = func.execute();
        
        // Clear arguments from function
        func.clear();
        
        theVar->setRevObject( funcReturnValue->getRevObject().clone() );
        
    }
    
    if ( dynamic == true )
    {
        // Check whether we have a control variable and make a clone in that case
        if ( theVar->isWorkspaceVariable() )
        {
            theVar = new Variable( theVar->getRevObject().clone() );
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
RevPtr<Variable> SyntaxVariable::evaluateLHSContent( Environment& env, const std::string& elemType )
{
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        // Find or create the variable
        if ( env.existsVariable( identifier ) )
        {
            theVar = env.getVariable( identifier );
        }
        else    // add it
        {
            theVar = new Variable( NULL, identifier );
            env.addVariable( identifier, theVar );
        }
    }
    else {
        
        // Get the base variable. Note that we do not create the variable in this case.
        theVar = baseVariable->evaluateContent( env );
        
        // Find member variable based on its name
        theVar = theVar->getRevObject().getMember( identifier );
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
    if ( baseVariable != NULL )
    {
        SyntaxVariable *v = dynamic_cast<SyntaxVariable*>( baseVariable );
        if ( v != NULL )
        {
            theName << v->getFullName( env ) << ".";
        }
    }
    
    theName << identifier;
    
    return theName.str();
}


/** Print info about the syntax element */
void SyntaxVariable::printValue(std::ostream& o) const
{
    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier      = \"" << identifier << "\"" << std::endl;
    
    o << std::endl;
    
    if ( baseVariable != NULL )
    {
        o << "base variable   = <" << baseVariable << "> ";
        baseVariable->printValue(o);
        o << std::endl;
    }
}




/**
 * Is the syntax element safe for use in a function ( as
 * opposed to a procedure)? The variable element is safe
 * if it does not include an expression that is not function-
 * safe.
 */
bool SyntaxVariable::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    
    if ( baseVariable != NULL && !baseVariable->isFunctionSafe( env, localVars ) )
        return false;
    
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
    if ( baseVariable != NULL )
        return baseVariable->retrievesExternVar( env, localVars, inLHS );
    
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

