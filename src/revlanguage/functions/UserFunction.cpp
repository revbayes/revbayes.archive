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
        UserFunction* fxn = this->clone();
        UserFunction* code = this->clone();
        
        retVal->makeDeterministicValue( fxn, code );

        return new Variable( retVal );
    }
    else
    {
        // "Flat" call: Simply execute and return the variable
        delete retVal;  // We don't need this

        return executeCode();
    }
}


/** In this function we execute the Rev code for the function (compiled syntax tree) */
RevPtr<Variable> UserFunction::executeCode( void )
{
    // Create new evaluation frame with function base class execution environment as parent
    Environment* functionFrame = new Environment( getEnvironment() );
    
    // Add the arguments to our environment
    for ( std::vector<Argument>::iterator it = args.begin(); it != args.end(); ++it )
    {
        // Note: We can add also temporary variable arguments as references because we
        // currently store them as arguments of the Rev function in UserFunctionArgs
        // as long as the UserFunctionCall exists.
        functionFrame->addReference( it->getLabel(), it->getVariable() );
    }

    // Clear signals
    Signals::getSignals().clearFlags();
    
    // Set initial return value
    RevPtr<Variable> retVar = NULL;
    
    // Execute code
    for ( std::list<SyntaxElement*>::const_iterator it = code->begin(); it != code->end(); ++it )
    {
        SyntaxElement* theSyntaxElement = *it;
        retVar = theSyntaxElement->evaluateContent( *functionFrame );
        
        if ( Signals::getSignals().isSet( Signals::RETURN ) )
        {
            Signals::getSignals().clearFlags();
            break;
        }
    }
    
    // Return the return value
    return retVar;
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


/** Get the parameters from the argument vector */
std::set<const RevBayesCore::DagNode*> UserFunction::getParameters(void) const
{
    std::set<const RevBayesCore::DagNode*> params;

    for (std::vector<Argument>::const_iterator it = args.begin(); it != args.end(); ++it )
    {
        if ( (*it).getVariable()->getRevObject().hasDagNode() )
            params.insert( (*it).getVariable()->getRevObject().getDagNode() );
    }

    return params;
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

