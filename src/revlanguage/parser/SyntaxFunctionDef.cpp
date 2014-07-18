#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef( const std::string &type, const std::string &name, std::list<SyntaxFormal*>* formals, std::list<SyntaxElement*>* stmts ) :
    SyntaxElement(),
    code( stmts ),
    functionName( name ),
    formalArgs( formals ),
    returnType( RlUtils::Void )
{
    if ( type == "" )
        returnType = RevObject::getClassTypeSpec();
    else
        returnType  = TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType( type ) );
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef( const SyntaxFunctionDef& x ) :
    SyntaxElement( x ),
    returnType( x.returnType )
{
    functionName = functionName;
 
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

        returnType   = x.returnType;
        functionName = x.functionName;
        
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



/** Get semantic value: insert a user-defined function in the user workspace */
RevPtr<Variable> SyntaxFunctionDef::evaluateContent( Environment& env )
{
    // Get argument rules from the formals
    ArgumentRules* argRules = new ArgumentRules();

    for ( std::list<SyntaxFormal*>::iterator it = formalArgs->begin(); it !=formalArgs->end(); ++it )
        argRules->push_back( (*it)->getArgumentRule()->clone() );

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();

    for( std::list<SyntaxElement*>::const_iterator it = code->begin(); it != code->end(); ++it )
        stmts->push_back( (*it)->clone() );

    // Create the function
    Function* theFunction = new UserFunction( argRules, returnType, stmts );
        
    // Insert the function in the (user) workspace
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


