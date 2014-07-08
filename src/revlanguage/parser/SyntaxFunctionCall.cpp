/**
 * @file
 * This file contains the implementation of SyntaxFunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * @brief Implementation of SyntaxFunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Argument.h"
#include "Environment.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Construct global function call from function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(const std::string &n, std::list<SyntaxLabeledExpr*>* args) : SyntaxElement(), arguments(args), functionName(n), variable(NULL) {
}


/** Construct member function call from variable, function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(SyntaxVariable* var, const std::string& n, std::list<SyntaxLabeledExpr*>* args) : SyntaxElement(), arguments(args), functionName(n), variable(var) {
}


/** Deep copy constructor */
SyntaxFunctionCall::SyntaxFunctionCall(const SyntaxFunctionCall& x) : SyntaxElement(x), variable( NULL ) {

    functionName = x.functionName;
    
    if (x.variable != NULL) {
        variable = x.variable->clone();
    }
    
    arguments = new std::list<SyntaxLabeledExpr*>();
    for (std::list<SyntaxLabeledExpr*>::iterator i=x.arguments->begin(); i!=x.arguments->end(); i++)
        arguments->push_back( (*i)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionCall::~SyntaxFunctionCall() {
    
    delete variable;
    
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++) {
        delete *i;
    }
    
    delete arguments;

}


/** Assignment operator */
SyntaxFunctionCall& SyntaxFunctionCall::operator=(const SyntaxFunctionCall& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        functionName = x.functionName;
        if (variable != NULL)
            delete variable;
        variable     = x.variable->clone();
        
        for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++) {
            delete *i;
        }
        arguments->clear();
        for (std::list<SyntaxLabeledExpr*>::iterator i=x.arguments->begin(); i!=x.arguments->end(); i++)
            arguments->push_back( (*i)->clone() );
    }

    return (*this);
}


/** Clone syntax element */
SyntaxFunctionCall* SyntaxFunctionCall::clone () const {

    return new SyntaxFunctionCall(*this);
}


/** Convert element to a deterministic function node. */
RevPtr<Variable> SyntaxFunctionCall::evaluateContent(Environment& env) {

    // Package arguments
    std::vector<Argument> args;
    for (std::list<SyntaxLabeledExpr*>::const_iterator i=arguments->begin(); i!=arguments->end(); i++) 
    {
        
#ifdef DEBUG_PARSER
        printf( "Adding argument with label \"%s\".\n", (*i)->getLabel().c_str() );
#endif
        
        const RlString& theLabel = (*i)->getLabel();
        RevPtr<Variable> theVar = (*i)->getExpression().evaluateContent(env);
        
        Argument theArg = Argument( theVar, theLabel.getValue() );
        args.push_back( theArg );
    }

    Function* func = NULL;
    if (variable == NULL) 
    {
        // We are trying to find a function in the current environment
        
        // First we see if the function name corresponds to a user-defined variable
        // We can do this first because user-defined variables are not allowed to mask function names
        bool found = false;
        if ( env.existsVariable( functionName ) )
        {
            RevObject &theObject = env.getRevObject( functionName );
            
            if ( theObject.isTypeSpec( Function::getClassTypeSpec() ) )
            {
                func = &( static_cast<Function&>( theObject ) );
                found = func->checkArguments(args, NULL);
            }
        }

        // If we cannot find the function name as a variable, it must be in the function table
        // This call will throw with a relevant message if the function is not found
        if ( !found )
        {
            func = &( env.getFunction(functionName, args) );
        }

        // Allow the function to process the arguments
        func->processArguments( args );

        // Set the execution environment of the function
        func->setExecutionEnviroment( &env );
    }
    else 
    {
        // We are trying to find a member function

        RevPtr<Variable> theVar = variable->evaluateContent( env );
            
        RevObject &theMemberObject = theVar->getRevObject();
            
        // \todo: We shouldn't allow const casts!!!
        MethodTable& mt = const_cast<MethodTable&>( theMemberObject.getMethods() );
            
        Function* theFunction = mt.getFunction( functionName, args ).clone();
        theFunction->processArguments(args);
        MemberFunction* theMemberFunction = static_cast<MemberFunction*>( theFunction );
        theMemberFunction->setMemberObject( theVar );
        func = theMemberFunction;

    }

    // Evaluate a "flat" function or create a deterministic node
    RevPtr<Variable> funcReturnValue = func->execute();

    // Clear arguments from function
    func->clear();

    return funcReturnValue;
}


/** Is the expression constant?
 *  Only if all arguments are constant.
 */
bool SyntaxFunctionCall::isConstExpression(void) const {
    
    // we need to iterate over all arguments
    for (std::list<SyntaxLabeledExpr*>::const_iterator i = arguments->begin(); i != arguments->end(); i++) {
        // we return false is this argument is not constant
        SyntaxLabeledExpr* expr = *i;
        if ( !expr->isConstExpression() ) {
            return false;
        }
    }
    
    // all arguments are constant
    return true;
}



/** Print info about the syntax element */
void SyntaxFunctionCall::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << functionName << std::endl;
    o << "variable      = ";
    if (variable == NULL)
        o << "NULL" << std::endl;
    else {
        o << "[" << variable << "] ";
        variable->printValue(o);
        o << std::endl;
    }
    if (arguments->size() == 0)
        o << "arguments     = []";
    else {
        int index = 1;
        for (std::list<SyntaxLabeledExpr*>::const_iterator i=arguments->begin(); i!=arguments->end(); i++, index++) {
            o << "arguments[" << index <<  "]  = [" << (*i) << "] ";
            (*i)->printValue(o);
            o << std::endl;
        }
    }
    o << std::endl;

    if (variable != NULL)
        variable->printValue(o);

    for (std::list<SyntaxLabeledExpr*>::const_iterator i=arguments->begin(); i!=arguments->end(); i++)
        (*i)->printValue(o);
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the arguments.
 */
void SyntaxFunctionCall::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    for (std::list<SyntaxLabeledExpr*>::iterator i = arguments->begin(); i != arguments->end(); i++) {
        (*i)->replaceVariableWithConstant(name, c);
    }
    
    // replace base variable too if this is a method
    if ( variable != NULL ) {
        variable->replaceVariableWithConstant(name, c);
    }
}

