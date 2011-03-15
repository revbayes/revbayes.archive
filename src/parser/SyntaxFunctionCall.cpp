/**
 * @file
 * This file contains the implementation of SyntaxFunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * @brief Implementation of SyntaxFunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "SyntaxFunctionCall.h"
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
DAGNode* SyntaxFunctionCall::getDAGNodeExpr(Frame* frame) const {

    // Package arguments
    std::vector<Argument> args;
    if (variable != NULL) {
        args.push_back(Argument("object", variable->getDAGNodeExpr(frame)));
        args.push_back(Argument("function", new ConstantNode(functionName)));
    }
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        args.push_back(Argument(*(*i)->getLabel(), (*i)->getExpression()->getDAGNodeExpr(frame)));    

    RbFunction* func;
    if (variable == NULL) {
        func = Workspace::userWorkspace().getFunction(*functionName, args);
        if (func == NULL)
            throw(RbException("Could not find function called '" + functionName->getValue() +
                "' taking specified arguments"));
    }
    else
        func = Workspace::globalWorkspace().getFunction("_memberCall", args);

    return new DeterministicNode(func);
}


/**
 * Look up the function or member function and calculate the value. We cannot simply get the
 * value of each argument because it could be a reference argument and we do not know whether
 * it is before we have matched the arguments to a specific function call. Instead, we get the
 * DAG node expression corresponding to the argument and dereference it if it turns out to
 * match a reference argument.
 */
DAGNode* SyntaxFunctionCall::getValue(Frame* frame) const {

    // Package arguments
    std::vector<Argument> args;
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        args.push_back(Argument(*(*i)->getLabel(), (*i)->getExpression()->getDAGNodeExpr(frame)));    

    // Get function pointer and execute function
    DAGNode* retVal;
    if (variable == NULL) {
        retVal = Workspace::userWorkspace().executeFunction(*functionName, args);
    }
    else {
        const RbComplex* objectPtr = dynamic_cast<const RbComplex*>(variable->getValuePtr(frame));
        if (objectPtr == NULL)
            throw(RbException("Object does not have member functions"));
        RbComplex* theObject = const_cast<RbComplex*>(objectPtr);
        retVal = theObject->executeMethod(*functionName, args);
    }

    return retVal;
}


/** Print info about the syntax element */
void SyntaxFunctionCall::print(std::ostream& o) const {

    o << "[" << this << "] SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << functionName << std::endl;
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

