#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "Signals.h"
#include "TypedUserFunction.h"
#include "TypeSpec.h"
#include "UserFunction.h"
#include "UserFunctionCall.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor */
UserFunction::UserFunction( const ArgumentRules*        argRules,
                            const TypeSpec&             retType ,
                            std::list<SyntaxElement*>*  stmts   )   :
    Function(),
    argumentRules( argRules ),
    returnType( retType ),
    code( stmts )
{
}


/** Copy constructor */
UserFunction::UserFunction( const UserFunction& x ) :
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
UserFunction& UserFunction::operator=( const UserFunction &f )
{
    if ( this != &f ) {
        
        // Call the base class assignment operator
        Function::operator=(f);

        delete argumentRules;
        argumentRules = new ArgumentRules(*f.argumentRules);
        
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++)
        {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        code->clear();
        
        // Create a new list for the code
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++)
        {
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserFunction::~UserFunction()
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
UserFunction* UserFunction::clone(void) const
{
    return new UserFunction(*this);
}


/** Execute function. Here we create a deterministic node if applicable, otherwise we just execute the code */
RevPtr<Variable> UserFunction::execute( void )
{
    
    // If the return type object has a DAG node inside it, we return an appropriate model/container/factor object
    // with a deterministic node inside it. Otherwise we return a "flat" RevObject without a dag node inside it.

    RevObject* retVal = Workspace::userWorkspace().makeNewDefaultObject( returnType.getType() );

    if ( retVal->hasDagNode() )
    {
        // Make a deterministic variable
        UserFunction*     fxn = this->clone();
        UserFunctionCall* call = new UserFunctionCall( fxn );
        
        retVal->makeDeterministicValue( call, fxn );

        return new Variable( retVal );
    }
    else
    {
        // "Flat" call: Simply execute and return the variable
        delete retVal;  // We don't need this

        UserFunctionCall fxnCall( this );

        return fxnCall.execute();
    }
}


/** Get Rev type of object */
const std::string& UserFunction::getClassType(void)
{
    static std::string revType = "UserFunction";
    
	return revType; 
}


/** Get a reference to the code for execution outside of this class */
const std::list<SyntaxElement*>& UserFunction::getCode(void) const
{
    return *code;
}


/** Get class type spec describing type of object */
const TypeSpec& UserFunction::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& UserFunction::getTypeSpec( void ) const
{
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules(void) const
{
    return *argumentRules;
}


/** Get return type */
const TypeSpec& UserFunction::getReturnType(void) const
{
    return returnType;
}

