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


/** Construct global function call from function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(RbString* id, std::list<SyntaxLabeledExpr*>* args)
    : SyntaxElement(), arguments(args), functionName(id), variable(NULL) {
}


/** Construct member function call from variable, function name and arguments */
SyntaxFunctionCall::SyntaxFunctionCall(SyntaxVariable* var, RbString* id, std::list<SyntaxLabeledExpr*>* args)
    : SyntaxElement(), arguments(args), functionName(id), variable(var) {
}


/** Deep copy constructor */
SyntaxFunctionCall::SyntaxFunctionCall(const SyntaxFunctionCall& x)
    : SyntaxElement(x) {

    functionName = new RbString(*functionName);
    variable     = new SyntaxVariable(*x.variable);
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        arguments->push_back(new SyntaxLabeledExpr(*(*i)));
}


/** Destructor deletes members */
SyntaxFunctionCall::~SyntaxFunctionCall() {
    
    delete functionName;
    delete variable;
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        delete (*i);
    delete arguments;
}


/** Assignment operator */
SyntaxFunctionCall& SyntaxFunctionCall::operator=(const SyntaxFunctionCall& x) {

    if (&x != this) {
    
        delete functionName;
        delete variable;
        for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
            delete (*i);
        delete arguments;

        SyntaxElement::operator=(x);

        functionName = new RbString(*functionName);
        variable     = new SyntaxVariable(*x.variable);
        for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
            arguments->push_back(new SyntaxLabeledExpr(*(*i)));
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxFunctionCall::briefInfo () const {

    std::ostringstream   o;
    if (variable == NULL)
        o << "SyntaxFunctionCall:  global call to " << std::string(*functionName) << " with " << arguments->size() << " arguments";
    else
        o << "SyntaxFunctionCall:  member call to " << std::string(*functionName) << " with " << arguments->size() << " arguments";

    return o.str();
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
Variable* SyntaxFunctionCall::getContentAsVariable(Environment* env) const {

    // Package arguments
    std::vector<Argument*> args;
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++) {
        PRINTF( "Adding argument with label \"%s\".\n", (*i)->getLabel()->getValue().c_str() );
        Argument *theArg = new Argument(*(*i)->getLabel(), (*i)->getExpression()->getContentAsVariable(env) );
        theArg->retain();
        args.push_back(theArg);
    }

    RbFunction* func;
    if (variable == NULL) {

        func = Workspace::userWorkspace().getFunction(*functionName, args);
        if (func == NULL)
            throw(RbException("Could not find function called '" + functionName->getValue() +
                "' taking specified arguments"));
    }
    else {

        DAGNode* theNode = variable->getContentAsVariable( env )->getDagNodePtr();
        if ( theNode == NULL || !theNode->getValue()->isType(MemberObject_name) )
            throw RbException( "Variable does not have member functions" );

        MemberObject *theMemberObject = dynamic_cast<MemberObject*>(theNode->getValuePtr());
//        args.insert( args.begin(), new Argument( "", memberNode ) );
        MemberFunction *theMemberFunction = (MemberFunction*) theMemberObject->getMethods().getFunction( *functionName, args );
        theMemberFunction->setMemberObject(theMemberObject);
        func = theMemberFunction;
    }

    return new Variable(new DeterministicNode(func));
}



/** Print info about the syntax element */
void SyntaxFunctionCall::print(std::ostream& o) const {

    o << "[" << this << "] SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << *functionName << std::endl;
    o << "variable      = ";
    if (variable == NULL)
        o << "NULL" << std::endl;
    else
        o << "[" << variable << "] " << variable->briefInfo() << std::endl;
    if (arguments->size() == 0)
        o << "arguments     = []";
    else {
        int index = 1;
        for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++, index++)
            o << "arguments[" << index <<  "]  = [" << (*i) << "] " << (*i)->briefInfo() << std::endl;
    }
    o << std::endl;

    if (variable != NULL)
        variable->print(o);

    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        (*i)->print(o);
}

