#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Signals.h"
#include "TypeSpec.h"
#include "UserFunction.h"
#include "UserFunctionDef.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor */
UserFunction::UserFunction( UserFunctionDef* def ) :
    Function(),
    functionDef( def )
{
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

    RevObject* retVal = Workspace::userWorkspace().makeNewDefaultObject( getReturnType().getType() );

    if ( retVal->isModelObject() )
    {
        retVal->makeUserFunctionValue( this->clone() );

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
    const std::list<SyntaxElement*>& code = functionDef->getCode();
    for ( std::list<SyntaxElement*>::const_iterator it = code.begin(); it != code.end(); ++it )
    {
        SyntaxElement* theSyntaxElement = *it;
        retVar = theSyntaxElement->evaluateContent( *functionFrame );
        
        if ( Signals::getSignals().isSet( Signals::RETURN ) )
        {
//            Signals::getSignals().clearFlags();
            break;
        }
    }
    
    // Return the return value
    return retVar;
}


/** Get Rev type (static) */
const std::string& UserFunction::getClassType(void)
{
    static std::string revType = "UserFunction";
    
	return revType; 
}


/** Get Rev type spec (static) */
const TypeSpec& UserFunction::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec; 
}


/** Get the parameters from the argument vector */
std::set<const RevBayesCore::DagNode*> UserFunction::getParameters(void) const
{
    std::set<const RevBayesCore::DagNode*> params;

    for (std::vector<Argument>::const_iterator it = args.begin(); it != args.end(); ++it )
    {
        if ( (*it).getVariable()->getRevObject().isModelObject() )
            params.insert( (*it).getVariable()->getRevObject().getDagNode() );
    }

    return params;
}


/** Get Rev type spec (from an instance) */
const TypeSpec& UserFunction::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}


/** Get argument rules */
const ArgumentRules& UserFunction::getArgumentRules(void) const
{
    return functionDef->getArgumentRules();
}


/** Get return type */
const TypeSpec& UserFunction::getReturnType(void) const
{
    return functionDef->getReturnType();
}

