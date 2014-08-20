#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "Argument.h"
#include "ConverterNode.h"
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
SyntaxVariable::SyntaxVariable( const std::string &n, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( n ),
    functionCall( NULL ),
    expression( NULL ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable( SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( "" ),
    functionCall( fxnCall ),
    expression( NULL ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Construct from expression and index */
SyntaxVariable::SyntaxVariable( SyntaxElement* expr, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( "" ),
    functionCall( NULL ),
    expression( expr ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x) :
    SyntaxElement( x ),
    identifier( x.identifier )
{
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    else
        functionCall = NULL;
    
    if ( x.expression != NULL )
        expression = x.expression->clone();
    else
        expression = NULL;

    if ( x.index != NULL )
    {
        index = new std::list<SyntaxElement*>();
        for ( std::list<SyntaxElement*>::const_iterator it = x.index->begin(); it != x.index->end(); ++it )
            index->push_back( (*it)->clone() );
    }
    else
        index = NULL;

    if ( x.baseVariable != NULL )
        baseVariable = x.baseVariable->clone();
    else
        baseVariable = NULL;
}


/** Destructor deletes base variable, expression, and index */
SyntaxVariable::~SyntaxVariable()
{
    if ( functionCall != NULL )
        delete functionCall;
    
    if ( expression != NULL )
        delete expression;
    
    if ( index != NULL )
    {
        for ( std::list<SyntaxElement*>::iterator it = index->begin(); it != index->end(); ++it )
            delete *it;
        delete index;
    }

    if ( baseVariable != NULL )
        delete baseVariable;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=( const SyntaxVariable& x )
{
    if (&x != this)
    {
        SyntaxElement::operator=( x );

        if ( functionCall != NULL )
            delete functionCall;

        if ( expression != NULL )
            delete expression;
        
        if ( index != NULL )
        {
            for ( std::list<SyntaxElement*>::iterator it = index->begin(); it != index->end(); ++it )
                delete *it;
            delete index;
            index = NULL;
        }
        
        if ( baseVariable != NULL )
            delete baseVariable;

        identifier   = x.identifier;

        if ( x.functionCall != NULL )
            functionCall = x.functionCall->clone();
        else
            functionCall = NULL;
        
        if ( x.expression != NULL )
            expression = x.expression->clone();
        else
            expression = NULL;
        
        if ( x.index != NULL )
        {
            index = new std::list<SyntaxElement*>();
            for ( std::list<SyntaxElement*>::const_iterator it = x.index->begin(); it != x.index->end(); ++it )
                index->push_back( (*it)->clone() );
        }
        else
            index = NULL;

        if ( x.baseVariable != NULL )
            baseVariable = x.baseVariable->clone();
        else
            baseVariable = NULL;
    }

    return (*this);
}



/** Type-safe clone of syntax element */
SyntaxVariable* SyntaxVariable::clone () const
{
    return new SyntaxVariable( *this );
}


/**
 * Get constant (static) value of one-offset indices. We use this when the
 * variable is part of a left-hand side expression, and when it is part
 * of a static right-hand side expression.
 */
std::vector<size_t> SyntaxVariable::computeStaticIndex( Environment& env ) const
{
    std::vector<size_t> oneOffsetIndices;
    
    size_t count = 1;
    for ( std::list<SyntaxElement*>::const_iterator it = index->begin(); it != index->end(); ++it, ++count )
    {
        if ( (*it) == NULL )
        {
            oneOffsetIndices.push_back( 0 );
        }
        else
        {
            // Evaluate the index variable statically
            RevPtr<Variable> indexVar = (*it)->evaluateContent( env );

            // Get the index
            size_t oneOffsetIndex = getIndex( indexVar, env );

            // Push it onto the vector
            oneOffsetIndices.push_back( oneOffsetIndex );
        }
    }
    
    // Return index
    return oneOffsetIndices;
}


/**
 * Evaluate the dynamic content of the one-offset index variables.
 */
std::vector< RevPtr<Variable> > SyntaxVariable::computeDynamicIndex( Environment& env )
{
    std::vector< RevPtr<Variable> > indexVars;

    std::list<SyntaxElement*>::const_iterator it;
    for ( it = index->begin(); it != index->end(); ++it )
    {
        RevPtr<Variable> theIndex = ( *it )->evaluateDynamicContent( env );
    
        // We ensure that the variables are of type Natural or can be converted to Natural numbers.
        // No sense in checking indices against permissible range here; errors are thrown later by
        // the container or member object if we are out of range.
        if ( !theIndex->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) )
        {
            if( theIndex->getRevObject().isConstant() && theIndex->getRevObject().isConvertibleTo( Natural::getClassTypeSpec(), true ) &&
                Natural::getClassTypeSpec().isDerivedOf( theIndex->getRevObjectTypeSpec() ) )
            {
                // Type promotion
                theIndex->setRevObject( theIndex->getRevObject().convertTo( Natural::getClassTypeSpec() ) );
                theIndex->setRevObjectTypeSpec( Natural::getClassTypeSpec() );
            }
            else if ( theIndex->getRevObject().isConvertibleTo( Natural::getClassTypeSpec(), false ) )
            {
                // Dynamic type conversion
                ConverterNode<Natural>* converterNode = new ConverterNode<Natural>( "", theIndex, Natural::getClassTypeSpec() );
                theIndex = new Variable( new Natural( converterNode ) );
            }
            else
                throw RbException( "No known conversion of type '" + theIndex->getRevObject().getType() + "' to 'Natural', required for index");
        }
        
        indexVars.push_back( theIndex );
    }
    
    return indexVars;
}

        
/**
 * @brief Evaluate semantic content (r-value) (general case)
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "baseVariable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable, we try to find it in the member variable
 * frame of a composite variable found by another SyntaxVariable element.
 *
 * An expression, such as a function call, may replace the identifier. If so,
 * then there will always be a list of indices, whereas the list of indices is
 * optional for an identifier.
 */
RevPtr<Variable> SyntaxVariable::evaluateContent( Environment& env)
{
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
        else if ( expression != NULL )
        {
            // Get the return variable of the expression
            theVar = expression->evaluateContent( env );
        }
        else
        {
            // Get named variable from the environment
            theVar = env.getVariable( identifier );
        }
    }
    else
    {
        // Note that the function call is always NULL if there is
        // a base variable, because any variables that are base to
        // the function call are handled by the function call. Note
        // also that generic expressions can only occur in base
        // variables, so we need not worry about any expression if
        // we are not a base variable.
        
        // Get the base variable
        theVar = baseVariable->evaluateContent( env );
        
        // Find member variable
        theVar = theVar->getRevObject().getMember( identifier );
    }
    
    // Get static index
    std::vector< size_t > oneOffsetIndices = computeStaticIndex( env );
    
    // Get element if indices are provided.
    while ( !oneOffsetIndices.empty() )
    {
        // Get the element...
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
        {
            // ... from a container

            // Get the container indices
            std::vector<size_t> containerOneOffsetIndices;
            for ( size_t i = 0; i < theVar->getRevObject().getDim(); ++i )
            {
                if ( !oneOffsetIndices.empty() )
                {
                    containerOneOffsetIndices.push_back( oneOffsetIndices[0] );
                    oneOffsetIndices.erase( oneOffsetIndices.begin() );
                }
                else
                    containerOneOffsetIndices.push_back( 0 );
            }

            // Get the element using the getElement function
            theVar = theVar->getRevObject().getElement( containerOneOffsetIndices );
        }
        else
        {
            // ... or from a subscript operator
            
            // Note that we do not name the element here; either the member object gives out
            // a variable it names itself, or it gives out a temporary variable copy, which
            // should not be named
            
            // Create the single argument for the index operator
            std::vector<Argument> args;
            RevPtr<Variable> indexVar = new Variable( new Natural( oneOffsetIndices[0] ) );
            args.push_back( Argument( indexVar ) );

            // Get the variable using the subscript operator function
            // TODO: This needs to be made generic for user-defined member objects
            theVar = theVar->getRevObject().executeMethod( "[]", args );
            
            // Erase the index
            oneOffsetIndices.erase( oneOffsetIndices.begin() );
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
    // Get static index. No dynamic evaluation here
    std::vector<size_t> oneOffsetIndices = computeStaticIndex( env );
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
        else if ( expression != NULL )
        {
            // Get the return variable of the expression
            theVar = expression->evaluateContent( env );
        }
        else
        {
            // Find or create the variable
            if ( env.existsVariable( identifier ) )
                theVar = env.getVariable( identifier );
            else    // add it
            {
                if ( oneOffsetIndices.size() == 0 )
                    theVar = new Variable( NULL, identifier );
                else
                    theVar = new Variable( Workspace::userWorkspace().makeNewEmptyContainer( elemType, oneOffsetIndices.size() ), identifier );
                env.addVariable( identifier, theVar );
            }
        }
    }
    else
    {
        // Note that the function call is always NULL if there is
        // a base variable, because any variables that are base to
        // the function call are handled by the function call. Note
        // also that generic expressions can only occur in base
        // variables, so we need not worry about any expression if
        // we are not a base variable.
        
        // Get the base variable. Note that we do not create the variable in this case.
        theVar = baseVariable->evaluateContent( env );

        // Find member variable based on its name
        theVar = theVar->getRevObject().getMember( identifier );
    }
    
    // Get element if indices are provided.
    while ( !oneOffsetIndices.empty() )
    {
        // Get the element...
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
        {
            // ... from a container
            
            // Get the container indices
            std::vector<size_t> containerOneOffsetIndices;
            for ( size_t i = 0; i < theVar->getRevObject().getDim(); ++i )
            {
                if ( !oneOffsetIndices.empty() )
                {
                    containerOneOffsetIndices.push_back( oneOffsetIndices[0] );
                    oneOffsetIndices.erase( oneOffsetIndices.begin() );
                }
                else
                    containerOneOffsetIndices.push_back( 0 );
            }
            
            // Get the element using the findOrCreateElement function
            theVar = theVar->getRevObject().findOrCreateElement( containerOneOffsetIndices );
        }
        else
        {
            // ... or from a subscript operator
            
            // Note that we do not name the element here; either the member object gives out
            // a variable it names itself, or it gives out a temporary variable copy, which
            // should not be named. A subscript operator cannot be used to assign to a non-
            // existing variable.
            
            // Create the single argument for the index operator
            std::vector<Argument> args;
            RevPtr<Variable> indexVar = new Variable( new Natural( oneOffsetIndices[0] ) );
            args.push_back( Argument( indexVar ) );
            
            // Get the variable using the subscript operator function
            // TODO: This needs to be made generic for user-defined member objects
            theVar = theVar->getRevObject().executeMethod( "[]", args );
            
            // Erase the index
            oneOffsetIndices.erase( oneOffsetIndices.begin() );
        }
    }

    // Return the variable for assignment
    return theVar;
}


/**
 * @brief Evaluate dynamic rhs content
 *
 * This function returns the semantic value of the variable expression
 * when it is part of a dynamic expression, that is, the right-hand side
 * of an equation (deterministic) or tilde (stochastic) assignment.
 *
 * It differs from the standard evaluateContent() in several ways. First,
 * control variables need to return clones of themselves (temporary
 * variables) rather than themselves, so that they are not included in
 * the DAG. Second, we cannot compute a static index for indexed variables.
 * Instead, we need to deliver an index conisting of variables resulting
 * from dynamic evaluation of the index variables. These need to be put
 * in a dynamic lookup variable.
 */
RevPtr<Variable> SyntaxVariable::evaluateDynamicContent( Environment& env)
{
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the dynamic return variable of the function call
            theVar = functionCall->evaluateDynamicContent( env );
        }
        else if ( expression  != NULL )
        {
            // Get the dynamic return variable of the expression
            theVar = expression->evaluateDynamicContent( env );
        }
        else
        {
            // Get variable from the environment (no dynamic version of identifier)
            theVar = env.getVariable( identifier );
        }
    }
    else
    {
        // Note that the function call is always NULL if there is
        // a base variable, because any variables that are base to
        // the function call are handled by the function call. Note
        // also that generic expressions can only occur in base
        // variables, so we need not worry about any expression if
        // we are not a base variable.
        
        // Get the base variable
        theVar = baseVariable->evaluateDynamicContent( env );
        
        // Find member variable (no dynamic version of identifier)
        theVar = theVar->getRevObject().getMember( identifier );
    }

    // Get dynamic index
    std::vector< RevPtr<Variable> > oneOffsetIndexVars = computeDynamicIndex( env );
    
    // Check if we need a dynamic lookup
    // TODO: Make sure we check for any named upstream constant DAG nodes that can be changed by user
    bool dynamicLookup = false;
    for ( std::vector< RevPtr<Variable> >::iterator it = oneOffsetIndexVars.begin(); it != oneOffsetIndexVars.end(); ++it )
    {
        if ( !(*it)->getRevObject().isConstant() || (*it)->getName() != "" )
        {
            dynamicLookup = true;
            break;
        }
    }
    
    // If the variable we are looking up things in does not have a DAG node, we do not need a dynamic lookup regardless
    if ( theVar->getRevObject().hasDagNode() == false )
        dynamicLookup = false;

    // Get dynamic element from container or subscript operator
    while ( !oneOffsetIndexVars.empty() )
    {
        // Get the element...
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
        {
            // ... from a container
            
            // Get the container index variables
            std::vector< RevPtr<Variable> > containerOneOffsetIndexVars;
            for ( size_t i = 0; i < theVar->getRevObject().getDim(); ++i )
            {
                if ( !oneOffsetIndexVars.empty() )
                {
                    containerOneOffsetIndexVars.push_back( oneOffsetIndexVars[0] );
                    oneOffsetIndexVars.erase( oneOffsetIndexVars.begin() );
                }
                else
                    containerOneOffsetIndexVars.push_back( new Variable( new Natural( 0 ) ) );
            }
            
            if ( dynamicLookup )
            {
                throw RbException( "Dynamic element lookups not working yet" );
                
                // Make a dynamic element lookup
                theVar = new Variable( theVar->getRevObject().makeElementLookup( theVar, containerOneOffsetIndexVars ) );
            }
            else
            {
                // We want a static element lookup

                // Get the container indices statically
                std::vector<size_t> containerOneOffsetIndices;
                for ( size_t i = 0; i < containerOneOffsetIndexVars.size(); ++i )
                {
                    containerOneOffsetIndices.push_back( getIndex( containerOneOffsetIndexVars[i], env ) );
                }
                
                // Get the element using the getElement function
                theVar = theVar->getRevObject().getElement( containerOneOffsetIndices );
            }
        }
        else
        {
            // ... or from a subscript operator
            
            // Note that we do not name the element here; either the member object gives out
            // a variable it names itself, or it gives out a temporary variable copy, which
            // should not be named
            
            // Create the single argument for the index operator (statically always for now)
            std::vector<Argument> args;
            args.push_back( Argument( new Variable( new Natural( getIndex( oneOffsetIndexVars[0], env ) ) ) ) );

           // Get the variable using the subscript operator function
            // TODO: This needs to be made generic for user-defined member objects
            // TODO: This needs to check that there is a subscript operator function and not procedure,
            // and then return a dynamic element lookup
            theVar = theVar->getRevObject().executeMethod( "[]", args );
            
            // Erase the index
            oneOffsetIndexVars.erase( oneOffsetIndexVars.begin() );
        }
    }
    
    // Check whether we have a control variable and make a clone in that case
    if ( theVar->isControlVar() )
        theVar = new Variable( theVar->getRevObject().clone() );
    
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
        theName << baseVariable->getFullName( env ) << ".";
    
    if ( functionCall != NULL )
        theName << "<functionCall>";
    else if ( expression != NULL )
        theName << "unnamed";
    else
        theName << identifier;

    for ( size_t i = 0; i < index->size(); ++i )
        theName << "[...]";

    return theName.str();
}


/** Get an index from an index variable in a static context */
size_t SyntaxVariable::getIndex( const RevPtr<Variable>& indexVar, Environment& env ) const
{
    if ( indexVar->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) )
    {
        // Get a Natural one-offset index
        size_t oneOffsetIndex = static_cast<Natural &>( indexVar->getRevObject() ).getValue();
        
        // Check validity
        if ( oneOffsetIndex < 1 )
        {
            std::ostringstream msg;
            msg << "Index for ";
            if ( baseVariable != NULL )
                msg << baseVariable->getFullName( env ) << ".";
            msg << identifier;
            msg << " smaller than 1";
            throw RbException( msg );
        }
        
        // Return the index
        return oneOffsetIndex;
    }
    else if ( indexVar->getRevObject().isConvertibleTo( Natural::getClassTypeSpec(), true ) )
    {
        // Convert to Natural
        RevObject* theNaturalIndex = indexVar->getRevObject().convertTo( Natural::getClassTypeSpec() );
        size_t oneOffsetIndex = static_cast<Natural*>( theNaturalIndex )->getValue();
        delete theNaturalIndex;
        
        // Check validity
        if ( oneOffsetIndex < 1 )
        {
            std::ostringstream msg;
            msg << "Index for ";
            if ( baseVariable != NULL )
                msg << baseVariable->getFullName( env ) << ".";
            msg << identifier;
            msg << " smaller than 1";
            throw RbException( msg );
        }
        
        // Return the index
        return oneOffsetIndex;
    }
    else if ( indexVar->getRevObject().isTypeSpec( RlString::getClassTypeSpec() ) )
    {
        throw RbException( "String indexes not supported (yet)");
    }
    else
    {
        std::ostringstream msg;
        msg << "Index for ";
        if ( baseVariable != NULL )
            msg << baseVariable->getFullName( env ) << ".";
        msg << identifier;
        msg << " of wrong type (neither " << Natural::getClassType() << " nor " << RlString::getClassType() << ")";
        throw RbException( msg );
    }
}


/**
 * Is the syntax element safe for use in a function ( as
 * opposed to a procedure)? The variable element is safe
 * if it does not include an expression that is not function-
 * safe.
 */
bool SyntaxVariable::isFunctionSafe( const Environment& env ) const
{
    if ( functionCall != NULL && !functionCall->isFunctionSafe( env ) )
        return false;
    
    if ( expression != NULL && !expression->isFunctionSafe( env ) )
        return false;

    if ( baseVariable != NULL && !baseVariable->isFunctionSafe( env ) )
        return false;
    
    return true;
}


/** Print info about the syntax element */
void SyntaxVariable::printValue(std::ostream& o) const
{
    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier      = \"" << identifier << "\"" << std::endl;

    if ( functionCall != NULL )
    {
        o << "function call   = <" << functionCall << "> ";
        functionCall->printValue(o);
        o << std::endl;
    }
    
    if ( expression != NULL )
    {
        o << "expression      = <" << expression << "> ";
        expression->printValue(o);
        o << std::endl;
    }
    
    size_t count = 1;
    for ( std::list<SyntaxElement*>::const_iterator it = (*index).begin(); it != (*index).end(); ++it, ++count )
    {
        if ( (*it) != NULL ) {
            o << "index " << count << " = <" << (*it) << "> ";
            (*it)->printValue(o);
            o << std::endl;
        }
        else {
            o << "index " << count << " = < NULL >" << std::endl;
        }
    }
    o << std::endl;

    if ( baseVariable != NULL )
    {
        o << "base variable   = <" << baseVariable << "> ";
        baseVariable->printValue(o);
        o << std::endl;
    }
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
bool SyntaxVariable::retrievesExternVar( const Environment& env ) const
{
    if ( baseVariable != NULL )
        return baseVariable->retrievesExternVar( env );
    
    if ( expression!= NULL )
        return expression->retrievesExternVar( env );
    
    if ( functionCall == NULL )
    {
        // Named variable; look for it in the environment
        if ( env.existsVariable( identifier ) )
            return true;
    }
    
    return false;
}


/** Set base variable of the variable expression */
void SyntaxVariable::setBaseVariable( SyntaxVariable* var )
{
    baseVariable = var;
}

