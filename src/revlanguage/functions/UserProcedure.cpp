#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "Signals.h"
#include "TypeSpec.h"
#include "UserProcedure.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor */
UserProcedure::UserProcedure( const ArgumentRules*          argRules,
                              const TypeSpec&               retType ,
                              std::list<SyntaxElement*>*    stmts   )   :
    Function(),
    argumentRules( argRules ),
    returnType( retType ),
    code( stmts )
{
}


/** Copy constructor */
UserProcedure::UserProcedure( const UserProcedure& x ) :
    Function( x ),
    argumentRules( new ArgumentRules( *x.argumentRules ) ),
    returnType( x.returnType ),
    code( NULL )
{
    // Create a new list for the code
    code = new std::list<SyntaxElement*>();
    for ( std::list<SyntaxElement*>::const_iterator it = x.code->begin(); it != x.code->end(); ++it )
    {
        SyntaxElement* element = (*it)->clone();
        code->push_back( element );
    }
}


/** Assignment operator. Deal with argument rules and code. */
UserProcedure& UserProcedure::operator=( const UserProcedure &f )
{
    if ( this != &f ) {
        
        // Call the base class assignment operator
        Function::operator=(f);
        
        // Copy argument rules
        delete argumentRules;
        argumentRules = new ArgumentRules(*f.argumentRules);
        
        // Delete the old code
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++)
        {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        code->clear();
        
        // Store the new code
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++)
        {
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserProcedure::~UserProcedure()
{
    delete argumentRules;
    
    for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++)
    {
        SyntaxElement* theSyntaxElement = *it;
        delete theSyntaxElement;
    }
    
    delete code;
}


/** Clone the object */
UserProcedure* UserProcedure::clone(void) const
{
    return new UserProcedure(*this);
}


/**
 * Execute the procedure. We simply execute the code; a procedure is never part of
 * a deterministic variable, so we need not worry about dynamic, multiple execution
 * of the procedure from a deterministic variable.
 *
 * Arguments are added to our environment as aliases, so we can both modify them and
 * assign to them.
 */
RevPtr<Variable> UserProcedure::execute( void )
{
    // Clear signals
    Signals::getSignals().clearFlags();
    
    // Set initial return value
    RevPtr<Variable> retVar = NULL;
    
    // Create new evaluation frame with function base class execution environment as parent
    Environment* procedureFrame = new Environment( getEnvironment() );
    
    // Add the arguments to our environment as alias variables (modifiable and assignable)
    for (std::vector<Argument>::iterator it = args.begin(); it != args.end(); ++it)
    {
        procedureFrame->addAlias( it->getLabel(), it->getVariable() );
    }
    
    // Execute code
    for ( std::list<SyntaxElement*>::const_iterator i=code->begin(); i!=code->end(); i++ ) {
        
        SyntaxElement* theSyntaxElement = *i;
        retVar = theSyntaxElement->evaluateContent( *procedureFrame );
        
        if ( Signals::getSignals().isSet( Signals::RETURN ) )
        {
            Signals::getSignals().clearFlags();
            break;
        }
    }
    
    // Delete evaluation frame
    delete procedureFrame;
    
    // Return the return value
    return retVar;
}


/** Get class type (static) */
const std::string& UserProcedure::getClassType(void)
{
    static std::string revType = "UserProcedure";
    
	return revType;
}


/** Get class type spec (static) */
const TypeSpec& UserProcedure::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get a reference to the code for execution outside of this class */
const std::list<SyntaxElement*>& UserProcedure::getCode(void) const
{
    return *code;
}


/** Get type spec (dynamic) */
const TypeSpec& UserProcedure::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}


/** Get argument rules */
const ArgumentRules& UserProcedure::getArgumentRules(void) const
{
    return *argumentRules;
}


/** Get return type */
const TypeSpec& UserProcedure::getReturnType(void) const
{
    return returnType;
}

