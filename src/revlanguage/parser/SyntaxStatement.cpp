#include "RlBoolean.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "Signals.h"
#include "SyntaxForLoop.h"
#include "SyntaxStatement.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;


/** Static vector of string giving names of statement types */
std::string SyntaxStatement::stmtName[] = { "IF", "IF_ELSE", "FOR", "WHILE", "NEXT", "BREAK", "RETURN" }; 


/** Construct from statement type */
SyntaxStatement::SyntaxStatement( statementT type ) :
    SyntaxElement(),
    statementType( type ),
    expression( NULL ),
    statements1( NULL ),
    statements2( NULL )
{
}


/** Construct from statement type and expression (RETURN expression) */
SyntaxStatement::SyntaxStatement( statementT type, SyntaxElement* expr ) :
    SyntaxElement(),
    statementType( type ),
    expression( expr ),
    statements1( NULL ),
    statements2( NULL )
{
}


/** Construct from statement type, condition and statement list */
SyntaxStatement::SyntaxStatement( statementT type, SyntaxElement* cond, std::list<SyntaxElement*>* stmts ) :
    SyntaxElement(),
    statementType( type ),
    expression( cond ),
    statements1( stmts ),
    statements2( NULL )
{
}


/** Construct from statement type, condition and two statement lists */
SyntaxStatement::SyntaxStatement(statementT                   type,
                                 SyntaxElement*               cond,
                                 std::list<SyntaxElement*>*   stmts1,
                                 std::list<SyntaxElement*>*   stmts2) :
    SyntaxElement(),
    statementType( type ),
    expression( cond ),
    statements1( stmts1 ),
    statements2( stmts2 )
{
}


/** Deep copy constructor */
SyntaxStatement::SyntaxStatement(const SyntaxStatement& x) :
    SyntaxElement( x )
{

    statementType   = x.statementType;
    expression      = x.expression->clone();

    statements1 = new std::list<SyntaxElement*>();
    if ( x.statements1 != NULL )
    {
        for ( std::list<SyntaxElement*>::const_iterator it = x.statements1->begin(); it != x.statements1->end(); ++it )
            statements1->push_back( (*it)->clone() );
    }
    
    statements2 = new std::list<SyntaxElement*>();
    if ( x.statements2 != NULL )
    {
        for ( std::list<SyntaxElement*>::const_iterator it = x.statements2->begin(); it != x.statements2->end(); ++it )
            statements2->push_back( (*it)->clone() );
    }
}


/** Destructor deletes expression and statements */
SyntaxStatement::~SyntaxStatement() {
    
    delete expression;
    
    if (statements1 != NULL)
    {
        for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
            delete *it;
        delete statements1;
    }
    
    if (statements2 != NULL)
    {
        for ( std::list<SyntaxElement*>::iterator it = statements2->begin(); it != statements2->end(); ++it )
            delete *it;
        delete statements2;
    }
}


/** Assignment operator */
SyntaxStatement& SyntaxStatement::operator=( const SyntaxStatement& x )
{
    if ( &x != this )
    {
        SyntaxElement::operator=( x );

        if ( expression != NULL )
        {
            delete expression;
            expression = NULL;
        }
        
        if ( statements1 != NULL )
        {
            for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
                delete *it;
            delete statements1;
            statements1 = NULL;
        }
        
        if ( statements2 != NULL )
        {
            for ( std::list<SyntaxElement*>::iterator it = statements2->begin(); it != statements2->end(); ++it )
                delete *it;
            delete statements2;
            statements2 = NULL;
        }

        statementType   = x.statementType;

        if ( x.expression != NULL )
            expression = x.expression->clone();

        if ( x.statements1 != NULL )
        {
            for ( std::list<SyntaxElement*>::const_iterator it = x.statements1->begin(); it != x.statements1->end(); ++it )
                statements1->push_back( (*it)->clone() );
        }

        if ( x.statements2 != NULL )
        {
            for ( std::list<SyntaxElement*>::const_iterator it = x.statements2->begin(); it != x.statements2->end(); ++it )
                statements2->push_back( (*it)->clone() );
        }
    }

    return ( *this );
}


/** Type-safe clone of syntax element */
SyntaxStatement* SyntaxStatement::clone( void ) const
{
    return new SyntaxStatement( *this );
}


/**
 * Get semantic value: it is here that we execute the statement. We
 * branch out depending on the statement type and process the
 * statement appropriately.
 *
 * Unlike R, Rev distinguishes between expressions and statements.
 * Statements evaluate to NULL and cannot be part of expressions.
 * In R, statements can be part of expressions, and conditional
 * statements (if and if-else statements) evaluate to the result
 * of the last statement.
 *
 * The 'return' statement, however, returns the result of the
 * evaluation of its expression to the enclosing environment
 * (the frame of the caller).
 *
 * All statements are quiet, that is, they do not print results of
 * enclosed statements, even if those results would be printed
 * otherwise.
 */
RevPtr<Variable> SyntaxStatement::evaluateContent(Environment& env) {

    RevPtr<Variable> result = NULL;
    
    if (statementType == For)
    {
        // Convert expression to for condition
        SyntaxForLoop* forLoop = dynamic_cast<SyntaxForLoop*>( expression );
        assert ( forLoop != NULL );

        // Initialize for loop. We use current environment for the loop variables (as in R)
        Signals::getSignals().clearFlags();
        forLoop->initializeLoop( env );

        // Now loop over statements inside the for loop
        while ( !forLoop->isFinished() )
        {
            // Get next loop state. This will update the value of the loop variable
            forLoop->getNextLoopState();

            for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
            {
                // Get a convenient pointer to the syntax element
                SyntaxElement* theSyntaxElement = *it;
                
                // Execute statement
                result = theSyntaxElement->evaluateContent( env );

                // Catch signal
                if ( !Signals::getSignals().isGood() )
                    break;
            }
            
            // Handle signals appropriately
            if ( Signals::getSignals().isSet( Signals::BREAK ) )
            {
                // We encountered 'break': clear flags and break
                Signals::getSignals().clearFlags();
                break;
            }
            else if ( Signals::getSignals().isSet( Signals::CONTINUE ) )
            {
                // We encountered 'continue': clear flags and continue with next loop state
                Signals::getSignals().clearFlags();
            }
            else if ( Signals::getSignals().isSet( Signals::RETURN ) )
            {
                // We encountered 'return': do not clear flags, simply break
                break;
            }
        }

        // Return NULL unless the RETURN flag is set
        if ( !Signals::getSignals().isSet( Signals::RETURN ) )
            result =  NULL;
        
        // Finalize the loop (resets the forloop state for next execution round)
        forLoop->finalizeLoop();
        
    }
    else if ( statementType == Break )
    {
        // Set BREAK signal
        Signals::getSignals().set(Signals::BREAK);
    }
    else if (statementType == Next)
    {
        // Set CONTINUE signal
        Signals::getSignals().set(Signals::CONTINUE);
    }
    else if ( statementType == While )
    {
        // Loop over statements inside the while loop, first checking the expression
        while ( isTrue( expression, env ) )
        {
            for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
            {
                SyntaxElement* theSyntaxElement = *it;

                // Execute statement
	            result = theSyntaxElement->evaluateContent( env );
                
	            // Catch signal
	            if ( !Signals::getSignals().isGood() )
                    break;
            }

            // Handle signals appropriately
            if ( Signals::getSignals().isSet( Signals::BREAK ) )
            {
                // We encountered 'break': clear flags and break
                Signals::getSignals().clearFlags();
                break;
            }
            else if ( Signals::getSignals().isSet(Signals::CONTINUE) )
            {
                // We encountered 'continue': clear flags and continue with next loop state
                Signals::getSignals().clearFlags();
            }
            else if ( Signals::getSignals().isSet( Signals::RETURN ) )
            {
                // We encountered 'return': do not clear flags, simply break
                break;
            }
        }

        // Return NULL unless the RETURN flag is set
        if ( !Signals::getSignals().isSet( Signals::RETURN ) )
            result =  NULL;
    }
    else if ( statementType == Return )
    {
        // Set result to expression value and set RETURN signal.
        // Note that the RETURN signal may be modified by evaluating
        // the expression, so we need to set it after the evalution
        // code has been processed.
        result = expression->evaluateContent( env );
        Signals::getSignals().set(Signals::RETURN);
    }
    else if ( statementType == If )
    {
        // Process statements inside the if clause if expression is true
        if ( isTrue( expression, env ) )
        {
            for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
            {
                // Execute statement
                result = (*it)->evaluateContent(env);
                
                // Catch a return signal
                if ( Signals::getSignals().isSet( Signals::RETURN ) )
                {
                    break;
                }
            }
        }
        // Return NULL unless the RETURN flag is set
        if ( !Signals::getSignals().isSet( Signals::RETURN ) )
            result =  NULL;
    }
    else if ( statementType == IfElse )
    {
        // Process statements inside the if clause if expression is true,
        // otherwise process statements in else clause
        if ( isTrue( expression, env ) )
        {
            for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
            {
                // Execute statement
                result = (*it)->evaluateContent( env );
                
                // Catch a return signal
                if ( Signals::getSignals().isSet( Signals::RETURN ) )
                {
                    break;
                }
            }
        }
        else
        {
            for ( std::list<SyntaxElement*>::iterator it = statements2->begin(); it != statements2->end(); ++it )
            {
                // Execute statement
                result = (*it)->evaluateContent( env );
                
                // Catch a return signal
                if ( Signals::getSignals().isSet( Signals::RETURN ) )
                {
                    break;
                }
            }
        }
        // Return NULL unless the RETURN flag is set
        if ( !Signals::getSignals().isSet( Signals::RETURN ) )
            result =  NULL;
    }

    return result;
}


/**
 * This is a help function that evaluates the expression and then checks
 * whether the result is true or false, or can be interpreted as a RlBoolean
 * true or false value.
 */
bool SyntaxStatement::isTrue( SyntaxElement* expr, Environment& env ) const
{
    RevPtr<Variable> temp = expr->evaluateContent( env );
    
    if ( temp == NULL )
        return false;
    
    if ( temp->getRevObject().isTypeSpec( RlBoolean::getClassTypeSpec() ) )
    {
        bool retValue = static_cast<const RlBoolean&>( temp->getRevObject() ).getValue();
        
        return retValue;
    }
    else
    {
        RevObject *tempObject = temp->getRevObject().convertTo( RlBoolean::getClassTypeSpec() );
        RlBoolean* tempBool = static_cast<RlBoolean*>( tempObject );
        bool     retValue = tempBool->getValue();
        
        delete tempBool;
        
        return   retValue;
    }
}


/**
 * Is this syntax element safe for inclusion in a function (as opposed
 * to a procedure)? A 'return' statement needs to check whether the
 * expression is function-safe. It also needs to check whether the
 * expression retrieves a non-local variable (which is not allowed in
 * functions). For all other statement types, we simply check the
 * expressions and statements that are enclosed in the statement.
 */
bool SyntaxStatement::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    if ( statementType == Return )
    {
        if( !expression->isFunctionSafe( env, localVars ) || expression->retrievesExternVar( env, localVars, false ) )
            return false;
        else
            return true;
    }
    
    if ( expression != NULL && !expression->isFunctionSafe( env, localVars ) )
        return false;

    if ( statements1 != NULL )
    {
        for ( std::list<SyntaxElement*>::iterator it = statements1->begin(); it != statements1->end(); ++it )
        {
            if ( !(*it)->isFunctionSafe( env, localVars ) )
                return false;
        }
    }
    
    if ( statements2 != NULL )
    {
        for ( std::list<SyntaxElement*>::iterator it = statements2->begin(); it != statements2->end(); ++it )
        {
            if ( !(*it)->isFunctionSafe( env, localVars ) )
                return false;
        }
    }
    
    return true;
}

 
/** Print info about the syntax element */
void SyntaxStatement::printValue(std::ostream& o) const
{
    o << "SyntaxStatement:" << std::endl;
    o << "statementType = " << stmtName[ statementType ] << std::endl;
    if (expression == NULL)
        o << "expression    = NULL" << std::endl;
    else 
    {
        o << "expression    = [" << expression << "] ";
        expression->printValue(o);
        o << std::endl;
    }
    if ( statements1 == NULL )
        o << "statements1   = NULL" << std::endl;
    else 
    {
        o << "statements1   = <" << statements1->size() << " statements>" << std::endl;
        int count = 1;
        for ( std::list<SyntaxElement*>::const_iterator it = statements1->begin(); it !=statements1->end(); ++it, ++count )
        {
            o << "   stmt " << count << " = [" << (*it) << "] ";
            (*it)->printValue(o);
            o << std::endl;
        }
    }
    if ( statements2 == NULL )
        o << "statements2   = NULL" << std::endl;
    else 
    {
        o << "statements2   = <" << statements2->size() << " statements>" << std::endl;
        int count = 1;
        for ( std::list<SyntaxElement*>::const_iterator it = statements2->begin(); it != statements2->end(); ++it, ++count )
        {
            o << "   stmt " << count << " = [" << (*it) << "] ";
            (*it)->printValue(o);
            o << std::endl;
        }
    }
    o << std::endl;
}

