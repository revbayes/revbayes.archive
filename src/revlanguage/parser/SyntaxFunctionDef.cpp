#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "UserProcedure.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef( const std::string&            type,
                                      const std::string&            name,
                                      std::list<SyntaxFormal*>*     formals,
                                      std::list<SyntaxElement*>*    stmts,
                                      bool                          isProcDef) :
    SyntaxElement(),
    code( stmts ),
    functionName( name ),
    formalArgs( formals ),
    returnType( RlUtils::Void ),
    isProcedureDef( isProcDef )
{
    if ( type == "" )
        returnType = RevObject::getClassTypeSpec();
    else
        returnType  = TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType( type ) );
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef( const SyntaxFunctionDef& x ) :
    SyntaxElement( x ),
    functionName( x.functionName ),
    returnType( x.returnType ),
    isProcedureDef( x.isProcedureDef )
{
    for ( std::list<SyntaxFormal*>::const_iterator it = x.formalArgs->begin(); it != x.formalArgs->end(); ++it )
        formalArgs->push_back( (*it)->clone() );
    
    for ( std::list<SyntaxElement*>::const_iterator it = x.code->begin(); it != x.code->end(); ++it )
        code->push_back( (*it)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef( void )
{
    for ( std::list<SyntaxFormal*>::iterator it = formalArgs->begin(); it != formalArgs->end(); ++it )
        delete *it;
    delete formalArgs;
    
    for ( std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); ++it )
        delete *it;
    delete code;
}


/** Assignment operator */
SyntaxFunctionDef& SyntaxFunctionDef::operator=( const SyntaxFunctionDef& x )
{
    if ( &x != this )
    {
        SyntaxElement::operator=( x );

        functionName    = x.functionName;
        isProcedureDef  = x.isProcedureDef;
        returnType      = x.returnType;
        
        formalArgs->clear();
        for ( std::list<SyntaxFormal*>::const_iterator it = x.formalArgs->begin(); it != x.formalArgs->end(); ++it )
            formalArgs->push_back( (*it)->clone() );
        
        code->clear();
        for ( std::list<SyntaxElement*>::const_iterator it = x.code->begin(); it != x.code->end(); ++it )
            code->push_back( (*it)->clone() );
        
    }

    return ( *this );
}


/** Type-safe clone of syntax element */
SyntaxFunctionDef* SyntaxFunctionDef::clone( void ) const
{
    return new SyntaxFunctionDef( *this );
}



/**
 * Get semantic value: insert a user-defined function or procedure in the user workspace.
 *
 * @todo Deal with local variables hiding external variables. Ask if user wants to replace
 *       an existing function or procedure.
 */
RevPtr<Variable> SyntaxFunctionDef::evaluateContent( Environment& env )
{
    // Get argument rules from the formals
    ArgumentRules* argRules = new ArgumentRules();

    for ( std::list<SyntaxFormal*>::iterator it = formalArgs->begin(); it !=formalArgs->end(); ++it )
        argRules->push_back( (*it)->getArgumentRule()->clone() );

    // Check whether statements are function-safe if we are a function
    if ( !isProcedureDef )
    {
        // Check first that all statements are function-safe
        for( std::list<SyntaxElement*>::const_iterator it = code->begin(); it != code->end(); ++it )
        {
            if ( !(*it)->isFunctionSafe( env ) )
            {
                std::ostringstream msg;
                msg << "The code of the function includes statements that modify or potentially modify" << std::endl;
                msg << "external variables. This is not allowed in a function. Either modify the code" << std::endl;
                msg << "of your function, or redefine it as a procedure." << std::endl;
                throw RbException( msg.str() );
            }
        }

        // Finally check whether last statement (if there is one) retrieves an external variable
        std::list<SyntaxElement*>::const_reverse_iterator rit = code->rbegin();
        if ( rit != code->rend() && (*rit)->retrievesExternVar( env ) )
            throw RbException( "The code is not function-safe." );
    }

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();

    for( std::list<SyntaxElement*>::const_iterator it = code->begin(); it != code->end(); ++it )
        stmts->push_back( (*it)->clone() );

    // Create the function
    Function* theFunction;
    if ( isProcedureDef )
        theFunction = new UserProcedure( argRules, returnType, stmts );
    else
        theFunction = new UserFunction( argRules, returnType, stmts );
    
    // Insert the function/procedure in the (user) workspace
    env.addFunction( functionName, theFunction );

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxFunctionDef::printValue( std::ostream& o ) const
{
    o << "SyntaxFunctionDef:" << std::endl;

    o << "returnType   = " << returnType.getType() << std::endl;
    o << "functionName = " << functionName << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}


