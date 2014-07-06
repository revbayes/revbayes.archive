#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Signals.h"
#include "TypedUserFunction.h"
#include "TypeSpec.h"
#include "UserFunctionCall.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;


/** Constructor */
UserFunctionCall::UserFunctionCall( const UserFunction* fxn   ) :
    Function(), userFunction(fxn)
{

    // Create new function frame with user function execution environment as parent
    functionFrame = new Environment( fxn->getEnvironment() );
        
    // Add the arguments from the user function fxn to our environment
    const std::vector<Argument>& fxnArgs = fxn->getArguments();
    for (std::vector<Argument>::const_iterator it = fxnArgs.begin(); it != fxnArgs.end(); ++it)
    {
        // Note: We can add also temporary variable arguments as references because we
        // currently store them as arguments of the Rev function in UserFunctionArgs
        // as long as the UserFunctionCall exists.
        RevPtr<const Variable> theVar = it->getVariable();
        functionFrame->addReferenceVariable( it->getLabel(), theVar );
    }
}


/** Copy constructor */
UserFunctionCall::UserFunctionCall(const UserFunctionCall &f)
    : Function( f ), userFunction( f.userFunction ), functionFrame( f.functionFrame->clone() ) {
    
}


UserFunctionCall& UserFunctionCall::operator=(const UserFunctionCall &f) {
    
    if ( this != &f ) {

        // Call the base class assignment operator
        Function::operator=(f);
        
        userFunction  = f.userFunction;
        functionFrame = f.functionFrame->clone();
    }
    
    return *this;
}


/** Destructor */
UserFunctionCall::~UserFunctionCall() {

    delete functionFrame;
}


/** Clone the object */
UserFunctionCall* UserFunctionCall::clone(void) const {
    
    return new UserFunctionCall(*this);
}


/** In this function we execute the Rev code for the function (uncompiled syntax tree for now) */
RevObject* UserFunctionCall::execute( void ) {
    
    // Clear signals
    Signals::getSignals().clearFlags();
    
    // Set initial return value
    RevPtr<Variable> retVar = NULL;
    
    // Execute code
    const std::list<SyntaxElement*>& code = userFunction->getCode();
    for ( std::list<SyntaxElement*>::const_iterator i=code.begin(); i!=code.end(); i++ ) {
        
        SyntaxElement* theSyntaxElement = *i;
        retVar = theSyntaxElement->evaluateContent( *functionFrame );
        
        if ( Signals::getSignals().isSet( Signals::RETURN ) )
            break;
    }

    // Return the return value
    return retVar->getRevObject().clone();
    
}


/** Get class name of object */
const std::string& UserFunctionCall::getClassName(void) {
    
    static std::string rbClassName = "UserFunctionCall";
    
	return rbClassName;
}


/** Get class type spec describing type of object */
const TypeSpec& UserFunctionCall::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/** Get type spec */
const TypeSpec& UserFunctionCall::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& UserFunctionCall::getArgumentRules(void) const {
    
    return userFunction->getArgumentRules();
}


/** Get the parameters from the function frame */
std::vector<const RevBayesCore::DagNode*> UserFunctionCall::getParameters(void) const {
    
    const std::map<std::string, VariableSlot* >& varTable = functionFrame->getVariableTable();
    
    std::vector<const RevBayesCore::DagNode*> params;
    std::map<std::string, VariableSlot*>::const_iterator it;
    for ( it = varTable.begin(); it != varTable.end(); it++ )
    {
        if ( it->second->getRevObject().hasDagNode() )
            params.push_back( it->second->getRevObject().getDagNode() );
    }
    
    return params;
}


/** Get return type */
const TypeSpec& UserFunctionCall::getReturnType(void) const {
    
    return userFunction->getReturnType();
}

