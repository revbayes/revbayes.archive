/**
 * @file
 * This file contains the implementation of UserFunction, which is
 * used to hold user-defined functions.
 *
 * @brief Implementation of UserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-21, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Frame.h"
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxElement.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <list>
#include <sstream>

/** Basic constructor */
UserFunction::UserFunction( const ArgumentRule**        argRules,
                            const RbString*             retType,
                            std::list<SyntaxElement*>*  stmts,
                            Frame*                      defineEnv) : RbFunction() {
    
    argumentRules       = argRules;
    returnType          = retType;
    code                = stmts;
    defineEnvironment   = defineEnv;
}


/** Copy constructor */
UserFunction::UserFunction(const UserFunction &x) : RbFunction(x) {

    returnType          = new RbString(*(x.returnType));
    defineEnvironment   = x.defineEnvironment->clone();

    int numRules=0;
    while (x.argumentRules[numRules++] != NULL)
        ;
    argumentRules = (const ArgumentRule**) calloc (numRules, sizeof(ArgumentRule*));
    for (int i=0; x.argumentRules[i] != NULL; i++)
        argumentRules[i] = x.argumentRules[i]->clone();

    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back((*i)->clone());
}


/** Destructor */
UserFunction::~UserFunction() {

    for (int i=0; argumentRules[i] != NULL; i++)
        delete argumentRules[i];
    free (argumentRules);

    delete returnType;

    for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
        delete (*i);
    delete code;

    // defineEnvironment->destroyEnclosure();   //TODO: or something like that
    delete defineEnvironment;
}


/** Brief ino on the function */
std::string UserFunction::briefInfo(void) const {

    std::ostringstream o;
    o << "UserFunction: ";
    printValue(o);

    return o.str();
}


/** Clone function */
UserFunction* UserFunction::clone(void) const {

    return new UserFunction(*this);
}


/** Equals comparison */
bool UserFunction::equals(const RbObject* x) const {

	const UserFunction* p = dynamic_cast<const UserFunction*>(x);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && defineEnvironment == p->defineEnvironment;   // Environments MUST be same pointer
    result = result && returnType == p->returnType;                 // Return type must be same

    int index;
    for (index=0; argumentRules[index]!= NULL && p->argumentRules[index]!=NULL; index++)
        result = result && argumentRules[index] == p->argumentRules[index];         // TODO: Equals comparison is needed!

    if (argumentRules[index] != NULL || p->argumentRules[index] != NULL)
        return false;
    
    if (code->size() != p->code->size())
        return false;

    std::list<SyntaxElement*>::const_iterator i, j;
    for (i=code->begin(), j=p->code->begin(); i!=code->end(); ++i, ++j)
        result = result && ((*i)->equals(*j));

    return result;
}



/** Execute function */
const RbObject* UserFunction::executeOperation(const std::vector<RbObjectWrapper*>& args) const {

    std::cerr << "I am a user-defined function." << std::endl;
    std::cerr << "I know who I am but I do not know how to execute myself." << std::endl;

    //TODO: Execute code: call getvalue, maintain a call stack and a frame stack, watch for return signal

    return NULL;
}


/** Get argument rules */
const ArgumentRule** UserFunction::getArgumentRules() const {

    return argumentRules;
}


/** Get class vector describing type of object */
const StringVector& UserFunction::getClass() const {

    static StringVector rbClass = StringVector(UserFunction_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string UserFunction::getReturnType() const {

    return *returnType;
}


/** Complete info about object */
std::string UserFunction::toString() const {

    int numRules;
    for (numRules = 0; argumentRules[numRules] != NULL; numRules++)
        ;

    std::ostringstream o;
    o << "User-defined function:" << std::endl;
    o << "formals     = " << numRules << " formal arguments" << std::endl;
    o << "return type = ";
    returnType->printValue(o);
    o << std::endl;
    o << "code        = " << code->size() << " lines of code" << std::endl;
    o << "definition environment:" << std::endl;
    defineEnvironment->printValue(o);

    return o.str();
}

