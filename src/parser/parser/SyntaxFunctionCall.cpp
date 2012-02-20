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
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Environment.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxFunctionCall::typeSpec(SyntaxFunctionCall_name);

/** Construct global function call from function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(RbString* id, std::list<SyntaxLabeledExpr*>* args)
    : SyntaxElement(), arguments(args), functionName(id), variable(NULL) {
}


/** Construct member function call from variable, function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(SyntaxVariable* var, RbString* id, std::list<SyntaxLabeledExpr*>* args)
    : SyntaxElement(), arguments(args), functionName(id), variable(var) {
}


/** Deep copy constructor */
SyntaxFunctionCall::SyntaxFunctionCall(const SyntaxFunctionCall& x) : SyntaxElement(x), variable( NULL ) {

    functionName = x.functionName->clone();
    
    if (x.variable != NULL) {
        variable = x.variable->clone();
    }
    
    arguments = new std::list<SyntaxLabeledExpr*>();
    for (std::list<SyntaxLabeledExpr*>::iterator i=x.arguments->begin(); i!=x.arguments->end(); i++)
        arguments->push_back( (*i)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionCall::~SyntaxFunctionCall() {
    
    delete functionName;
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

        if (functionName != NULL) {
            delete functionName;
        }
        functionName = x.functionName->clone();
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


/** Get class vector describing type of object */
const VectorString& SyntaxFunctionCall::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxFunctionCall_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to a deterministic function node. */
RbVariablePtr SyntaxFunctionCall::evaluateContent(Environment& env) {

    // Package arguments
    std::vector<Argument> args;
    for (std::list<SyntaxLabeledExpr*>::const_iterator i=arguments->begin(); i!=arguments->end(); i++) {
        PRINTF( "Adding argument with label \"%s\".\n", (*i)->getLabel().getValue().c_str() );
        const RbString& theLabel = (*i)->getLabel();
        RbVariablePtr theVar = (*i)->getExpression().evaluateContent(env);
        Argument theArg = Argument( theLabel, theVar );
        args.push_back( theArg );
    }

    RbFunction* func;
    if (variable == NULL) {

        func = Workspace::userWorkspace().getFunction(*functionName, args);
        if (func == NULL)
            throw(RbException("Could not find function called '" + functionName->getValue() +
                "' taking specified arguments"));
    }
    else {

        DAGNode* theNode = variable->evaluateContent( env )->getDagNode();
        if ( theNode == NULL || !theNode->getValue().isTypeSpec( TypeSpec(MemberObject_name) ) )
            throw RbException( "Variable does not have member functions" );

        MemberObject& theMemberObject = dynamic_cast<MemberObject&>( theNode->getValue() );
//        args.insert( args.begin(), new Argument( "", memberNode ) );
        // TODO: We shouldn't allow const casts!!!
        MethodTable& mt = const_cast<MethodTable&>( theMemberObject.getMethods() );
        MemberFunction* theMemberFunction = static_cast<MemberFunction*>( mt.getFunction( *functionName, args ) );
        theMemberFunction->setMemberObject(theMemberObject);
        func = theMemberFunction;
    }

    return RbVariablePtr( new Variable( new DeterministicNode( func ) ) );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxFunctionCall::getTypeSpec(void) const {
    return typeSpec;
}



/** Print info about the syntax element */
void SyntaxFunctionCall::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << *functionName << std::endl;
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

