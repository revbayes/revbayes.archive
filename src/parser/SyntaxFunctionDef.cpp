/**
 * @file
 * This file contains the implementation of SyntaxFunctionDef, which is
 * used to hold definitions functions (user-defined functions).
 *
 * @brief Implementation of SyntaxFunctionDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>


/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef(   RbString* type,
                                        RbString* name,
                                        std::list<SyntaxFormal*>* formals,
                                        std::list<SyntaxElement*>* stmts)
    : SyntaxElement(), returnType(type), functionName(name), formalArgs(formals), code(stmts) {

    if (returnType == NULL)
        returnType = new RbString(RbObject_name);
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef(const SyntaxFunctionDef& x)
    : SyntaxElement(x) {

    returnType   = new RbString(*returnType);
    functionName = new RbString(*functionName);
 
    for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
        formalArgs->push_back((SyntaxFormal*)((*i)->clone()));

    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back((*i)->clone());
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef() {
    
    delete returnType;
    delete functionName;

    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        delete (*i);
    delete formalArgs;

    for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
        delete (*i);
    delete code;
}


/** Return brief info about object */
std::string SyntaxFunctionDef::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxFunctionDef:  definition of a funtion called  " << std::string(*functionName);

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxFunctionDef::clone () const {

    return (SyntaxElement*)(new SyntaxFunctionDef(*this));
}


/** Equals comparison */
bool SyntaxFunctionDef::equals(const SyntaxElement* elem) const {

	const SyntaxFunctionDef* p = dynamic_cast<const SyntaxFunctionDef*>(elem);
    if (p == NULL)
        return false;

    if (formalArgs->size() != p->formalArgs->size())
        return false;
    if (code->size() != p->code->size())
        return false;

    bool result = true;
    result = result && returnType->equals(p->returnType);
    result = result && functionName->equals(p->functionName);

    std::list<SyntaxFormal*>::iterator i1, j1;
    for (i1=formalArgs->begin(), j1=p->formalArgs->begin(); i1!=formalArgs->end(); i1++, j1++)
        result = result && (*i1)->equals(*j1);
    
    std::list<SyntaxElement*>::iterator i2, j2;
    for (i2=code->begin(), j2=p->code->begin(); i2!=code->end(); i2++, j2++)
        result = result && (*i2)->equals(*j2);
    
    return result;
}


/** Convert element to DAG node; return NULL since it is not applicable */
DAGNode* SyntaxFunctionDef::getDAGNode(Frame* formal) const {

    return NULL;
}


/** Get semantic value: insert a user-defined function in the user workspace */
RbObject* SyntaxFunctionDef::getValue(Frame* frame) const {

    // Get argument rules from the formals
    const ArgumentRule** argRules = (const ArgumentRule**) calloc (formalArgs->size()+1, sizeof(ArgumentRule*));
    int index = 0;
    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        argRules[index++] = (*i)->getArgumentRule(frame);

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
    for(std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
        stmts->push_back((*i)->clone());

    // Create copy of the environment in which the function was defined
    Frame* defineEnvironment = frame->clone();  //TODO: make deep copy of entire environment

    // Create the function
    UserFunction* theFunction = new UserFunction(argRules, returnType, stmts, defineEnvironment);

    // Insert in the user workspace
    Workspace::userWorkspace().addFunction(*functionName, theFunction);

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxFunctionDef::print(std::ostream& o) const {

    o << "SyntaxFunctionDef:" << std::endl;
    o << "returnType   = ";
    returnType->printValue(o);
    o << std::endl;
    o << "functionName = ";
    functionName->printValue(o);
    o << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}


