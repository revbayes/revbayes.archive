#include "AbstractMemberFunction.h"
#include "Argument.h"
#include "Environment.h"
#include "MemberProcedure.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "SyntaxFunctionCall.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Construct global function call from function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall( const std::string &n, std::list<SyntaxLabeledExpr*>* args) :
    SyntaxElement(),
    arguments( args ),
    functionName( n ),
    baseVariable( NULL )
{
}


/** Construct member function call from base variable, function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall( SyntaxVariable* var, const std::string& n, std::list<SyntaxLabeledExpr*>* args ) :
    SyntaxElement(),
    arguments( args ),
    functionName( n ),
    baseVariable( var )
{
}


/** Deep copy constructor */
SyntaxFunctionCall::SyntaxFunctionCall(const SyntaxFunctionCall& x) :
    SyntaxElement( x ),
    arguments( NULL ),
    functionName( x.functionName ),
    baseVariable( NULL )
{
    if (x.baseVariable != NULL)
        baseVariable = x.baseVariable->clone();
    
    arguments = new std::list<SyntaxLabeledExpr*>();
    for ( std::list<SyntaxLabeledExpr*>::iterator it = x.arguments->begin(); it != x.arguments->end(); ++it )
        arguments->push_back( (*it)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionCall::~SyntaxFunctionCall()
{
    delete baseVariable;
    
    for ( std::list<SyntaxLabeledExpr*>::iterator it = arguments->begin(); it != arguments->end(); ++it )
        delete *it;
    delete arguments;
}


/** Assignment operator */
SyntaxFunctionCall& SyntaxFunctionCall::operator=( const SyntaxFunctionCall& x )
{
    if ( &x != this )
    {
        SyntaxElement::operator=( x );

        delete baseVariable;
        
        for ( std::list<SyntaxLabeledExpr*>::iterator it = arguments->begin(); it != arguments->end(); ++it )
            delete *it;

        functionName = x.functionName;

        if (x.baseVariable != NULL)
            baseVariable = x.baseVariable->clone();
        
        arguments->clear();
        for ( std::list<SyntaxLabeledExpr*>::iterator it = x.arguments->begin(); it != x.arguments->end(); ++it )
            arguments->push_back( (*it)->clone() );
    }

    return ( *this );
}


/** Type-safe clone of syntax element */
SyntaxFunctionCall* SyntaxFunctionCall::clone( void ) const
{
    return new SyntaxFunctionCall( *this );
}


/**
 * Evaluate semantic content (static version). Here we know that we only want
 * to evaluate the function once, and get a constant value back. For now,
 * it does the same thing as the evaluateDynamicContent() function with two
 * exceptions. First, it uses the static evaluate function to evaluate its
 * arguments. Second, it makes the return value a constant value.
 *
 * @todo Support this function call context better
 */
RevPtr<Variable> SyntaxFunctionCall::evaluateContent( Environment& env )
{
    // Package arguments
    std::vector<Argument> args;
    for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it != arguments->end(); ++it )
    {
#ifdef DEBUG_PARSER
        printf( "Adding argument with label \"%s\".\n", (*it)->getLabel().c_str() );
#endif
        const RlString& theLabel = (*it)->getLabel();
        RevPtr<Variable> theVar = (*it)->getExpression().evaluateContent(env);
        
        Argument theArg = Argument( theVar, theLabel.getValue() );
        args.push_back( theArg );
    }

    RevPtr<Variable> funcReturnValue = NULL;
    if ( baseVariable == NULL )
    {
        // We are trying to find a function in the current environment
        
        // First we see if the function name corresponds to a user-defined variable.
        // We can do this first because user-defined variables are not allowed to mask
        // names of function calls
        bool found = false;
        Function* func = NULL;
        if ( env.existsVariable( functionName ) )
        {
            RevObject &theObject = env.getRevObject( functionName );
            
            if ( theObject.isTypeSpec( Function::getClassTypeSpec() ) )
            {
                func = static_cast<Function&>( theObject ).clone();
                found = func->checkArguments(args, NULL, true);
            }
        }

        // If we cannot find the function name as a variable, it must be in the function table
        // This call will throw a relevant error message if the function is not found
        if ( !found )
            func = env.getFunction(functionName, args, true).clone();

        // Allow the function to process the arguments
        func->processArguments( args, true );

        // Set the execution environment of the function
        func->setExecutionEnviroment( &env );
        
        // Evaluate the function (call the static evaluation function)
        funcReturnValue = func->execute();
        
        // Delete function
        delete func;
    }
    else 
    {
        // We are trying to find a member function

        // First we get the base variable
        RevPtr<Variable> theVar = baseVariable->evaluateContent( env );
            
        // Now we get a reference to the member object inside
        RevObject &theMemberObject = theVar->getRevObject();
            
        // \todo: We shouldn't allow const casts!!!
        MethodTable& mt = const_cast<MethodTable&>( theMemberObject.getMethods() );
            
        Function* theFunction = mt.getFunction( functionName, args, true ).clone();
        theFunction->processArguments(args, true);
        
        MemberProcedure* theMemberProcedure = dynamic_cast<MemberProcedure*>( theFunction );
        if ( theMemberProcedure != NULL )
        {
            theMemberProcedure->setMemberObject( theVar );
            
            // Evaluate the function (call the dynamic evaluation function)
            funcReturnValue = theMemberProcedure->execute();
        }
        else
        {
            AbstractMemberFunction* theMemberFunction = dynamic_cast<AbstractMemberFunction*>( theFunction );
            if ( theMemberFunction != NULL )
            {
                theMemberFunction->setMemberObject( theVar );
                
                // Evaluate the function (call the dynamic evaluation function)
                funcReturnValue = theMemberFunction->execute();
            }
            else
            {
                throw RbException("Could convert member function.");
            }
        }
        
        delete theFunction;
    }

    // Return the value, which is typically a deterministic variable with the function
    // inside it, although many functions return constant values or NULL (void).
    // To make sure the value is a constant and not a deterministic variable in this
    // context, we convert the return value here to a constant value before returning it.
    if ( funcReturnValue != NULL )
        funcReturnValue->getRevObject().makeConstantValue();
    return funcReturnValue;
}


/**
 * Evaluate semantic content (dynamic version). Here we know that we want
 * to evaluate the function repeatedly, as part of a deterministic variable.
 * Therefore, we need to make a deterministic variable with the function inside
 * it. Currently, this is the default implementation for the execute function
 * in RevLanguage::Function, but we may want to differentiate between the two
 * contexts later.
 *
 * @todo Support this function call context better
 */
RevPtr<Variable> SyntaxFunctionCall::evaluateDynamicContent( Environment& env )
{
    // Package arguments
    std::vector<Argument> args;
    for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it != arguments->end(); ++it )
    {
#ifdef DEBUG_PARSER
        printf( "Adding argument with label \"%s\".\n", (*it)->getLabel().c_str() );
#endif
        const RlString& theLabel = (*it)->getLabel();
        RevPtr<Variable> theVar = (*it)->getExpression().evaluateDynamicContent(env);
        
        Argument theArg = Argument( theVar, theLabel.getValue() );
        args.push_back( theArg );
    }
    
    RevPtr<Variable> funcReturnValue = NULL;
    if ( baseVariable == NULL )
    {
        // We are trying to find a function in the current environment
        
        // First we see if the function name corresponds to a user-defined variable
        // We can do this first because user-defined variables are not allowed to mask function names
        bool found = false;
        Function* func = NULL;
        if ( env.existsVariable( functionName ) )
        {
            RevObject &theObject = env.getRevObject( functionName );
            
            if ( theObject.isTypeSpec( Function::getClassTypeSpec() ) )
            {
                func = static_cast<Function&>( theObject ).clone();
                found = func->checkArguments(args, NULL, false);
            }
        }
        
        // If we cannot find the function name as a variable, it must be in the function table
        // This call will throw with a relevant message if the function is not found
        if ( !found )
        {
            func = env.getFunction(functionName, args, false).clone();
        }
        
        // Allow the function to process the arguments
        func->processArguments( args, false );
        
        // Set the execution environment of the function
        func->setExecutionEnviroment( &env );
        
        // Evaluate the function (call the dynamic evaluation function)
        funcReturnValue = func->execute();
        
        // Delete function
        delete func;
    }
    else
    {
        // We are trying to find a member function
        
        // First we get the base variable
        RevPtr<Variable> theVar = baseVariable->evaluateContent( env );
        
        // Now we get a reference to the member object inside
        RevObject &theMemberObject = theVar->getRevObject();
        
        // \todo: We shouldn't allow const casts!!!
        MethodTable& mt = const_cast<MethodTable&>( theMemberObject.getMethods() );
        
        Function* theFunction = NULL;
//        try
//        {
//            theFunction = mt.getFunction( functionName, args, false ).clone();
//            theFunction->processArguments( args, false );
//        }
//        catch ( RbException e )
//        {
//            
//        }
//        if ( theFunction == NULL )
//        {
//            // try to convert to stochastic node
//            
//            if ( theMemberObject.getDagNode()->isStochastic() )
//            {
//                theMemberObject.getDagNode()->get
//            }
//            
//        }
        theFunction = mt.getFunction( functionName, args, false ).clone();
        theFunction->processArguments( args, false );
        
        MemberProcedure* theMemberProcedure = dynamic_cast<MemberProcedure*>( theFunction );
        if ( theMemberProcedure != NULL )
        {
            theMemberProcedure->setMemberObject( theVar );
            
            // Evaluate the function (call the dynamic evaluation function)
            funcReturnValue = theMemberProcedure->execute();
        }
        else
        {
            AbstractMemberFunction* theMemberFunction = dynamic_cast<AbstractMemberFunction*>( theFunction );
            if ( theMemberFunction != NULL )
            {
                theMemberFunction->setMemberObject( theVar );
                
                // Evaluate the function (call the dynamic evaluation function)
                funcReturnValue = theMemberFunction->execute();
            }
            else
            {
                throw RbException("Could convert member function.");
            }
        }
        
        // Delete the member function clone
        delete theFunction;
    }
    
    // Return the value, which is typically a deterministic variable with the function
    // inside it, although many functions return constant values or NULL (void). Here
    // we do not want to convert any deterministic variables to constant values, but
    // just return them as is.
    return funcReturnValue;
}


/**
 * Is the expression constant?
 * Only if all arguments are constant.
 */
bool SyntaxFunctionCall::isConstExpression(void) const {
    
    // We need to iterate over all arguments
    for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it != arguments->end(); ++it )
    {
        // We return false if this argument is not constant
        SyntaxLabeledExpr* expr = *it;
        if ( !expr->isConstExpression() )
            return false;
    }
    
    // All arguments are constant
    return true;
}


/**
 * Is the syntax element safe for use in a function
 * (as opposed to a procedure)? The function call is safe
 * if it is a call to a function, and the argument expressions
 * are all function-safe. If it is a call to a procedure, it
 * is safe if all argument expressions are function-safe, and
 * none of them retrieves an external variable.
 */
bool SyntaxFunctionCall::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    // Protect from self-checking if recursive. If that case, the function
    // does not exist yet and we tentatively assume it is safe
    if ( !env.existsFunction( functionName ) )
        return true;
    
    if ( env.isProcedure( functionName ) )
    {
        // Check base variable
        if ( baseVariable != NULL && ( !baseVariable->isFunctionSafe( env, localVars ) || baseVariable->SyntaxElement::retrievesExternVar( env, localVars, false ) ) )
            return false;
        
        // Iterate over all arguments
        for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it != arguments->end(); ++it )
        {
            // Return false if argument expression is not function-safe or retrieves an external variable
            SyntaxLabeledExpr* expr = *it;
            if ( !expr->isFunctionSafe( env, localVars ) || expr->retrievesExternVar( env, localVars, false ) )
                return false;
        }
        
    }
    else
    {
        // Check base variable
        if ( baseVariable != NULL && !baseVariable->isFunctionSafe( env, localVars ) )
            return false;
        
        // Iterate over all arguments
        for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it != arguments->end(); ++it )
        {
            // Return false if argument expression is not function-safe
            SyntaxLabeledExpr* expr = *it;
            if ( !expr->isFunctionSafe( env, localVars ) )
                return false;
        }
        
    }
    
    // All arguments and the base variable are OK
    return true;
}


/** Print info about the syntax element */
void SyntaxFunctionCall::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << functionName << std::endl;
    o << "baseVariable  = ";
    if ( baseVariable == NULL )
        o << "NULL" << std::endl;
    else
    {
        o << "[" << baseVariable << "] ";
        baseVariable->printValue(o);
        o << std::endl;
    }
    if ( arguments->size() == 0 )
        o << "arguments     = []";
    else
    {
        size_t index = 1;
        for ( std::list<SyntaxLabeledExpr*>::const_iterator it = arguments->begin(); it !=arguments->end(); ++it, ++index )
        {
            o << "arguments[" << index <<  "]  = [" << (*it) << "] ";
            (*it)->printValue(o);
            o << std::endl;
        }
    }
    o << std::endl;
}

