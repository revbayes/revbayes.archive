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

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
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


/** Return brief info about object */
std::string SyntaxFunctionCall::briefInfo () const {

    std::ostringstream   o;
    if (variable == NULL)
        o << "SyntaxFunctionCall:  global call to " << std::string(*functionName) << "(?)";
    else
        o << "SyntaxFunctionCall:  member call to " << std::string(*functionName) << "(?)";

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxFunctionCall::clone () const {

    return (SyntaxElement*)(new SyntaxFunctionCall(*this));
}


/** Equals comparison */
bool SyntaxFunctionCall::equals(const SyntaxElement* elem) const {

	const SyntaxFunctionCall* p = dynamic_cast<const SyntaxFunctionCall*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && functionName->equals(p->functionName);
    result = result && variable->equals(p->variable);

    std::list<SyntaxLabeledExpr*>::iterator i, j;
    for (i=arguments->begin(), j=p->arguments->begin(); i!=arguments->end(); i++, j++)
        result = result && (*i)->equals(*j);
    
    return result;
}


/** Get class vector describing type of object */
const StringVector& SyntaxFunctionCall::getClass(void) const { 

    static StringVector rbClass = StringVector(SyntaxFunctionCall_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node */
DAGNode* SyntaxFunctionCall::getDAGNode(Frame* formal) const {

    std::vector<Argument> args;
    if (variable != NULL) {
        args.push_back(Argument("object", variable->getDAGNode()));
        args.push_back(Argument("function", new ConstantNode(functionName)));
    }
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        args.push_back(Argument(*(*i)->getLabel(), (*i)->getDAGNode(formal)));    

    RbFunction* func;
    if (variable == NULL) {
        func = Workspace::userWorkspace().getFunction(*functionName, args);
        if (func == NULL)
            throw(RbException("Could not find function called '" + functionName->getString() +
                "' taking specified arguments"));
    }
    else
        func = Workspace::globalWorkspace().getFunction(".memberCall", args);

    return new DeterministicNode((RbFunction*)(func));
}


/**
 * @brief Get semantic value
 *
 * We look up the function or member function and calculate the value.
 *
 */
RbObject* SyntaxFunctionCall::getValue(Frame* frame) const {

    // Package arguments
    std::vector<Argument> args;
    for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++)
        args.push_back(Argument(*(*i)->getLabel(), (*i)->getDAGNode(frame)));    

    // Get function pointer and execute function
    if (variable == NULL) {
        const RbObject* retVal =  Workspace::userWorkspace().executeFunction(*functionName, args);
        return retVal->clone();
    }
    else {
        RbComplex* theObject = dynamic_cast<RbComplex*>(variable->getValue());
        if (theObject == NULL)
            throw(RbException("Object does not have member functions"));
        const RbObject* retVal = theObject->executeMethod(*functionName, args);    
        return retVal->clone();    
    }
}


/** Print info about the syntax element */
void SyntaxFunctionCall::print(std::ostream& o) const {

    o << "SyntaxFunctionCall:" << std::endl;
    o << "functionName  = " << functionName->briefInfo() << std::endl;
    o << "variable      = " << variable->briefInfo() << std::endl;
    if (arguments->size() == 0)
        o << "arguments     = []";
    else {
        int index = 1;
        for (std::list<SyntaxLabeledExpr*>::iterator i=arguments->begin(); i!=arguments->end(); i++, index++)
            o << "arguments[" << index <<  "]  = " << (*i)->briefInfo() << std::endl;
    }
}


