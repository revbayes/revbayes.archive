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

#include <list>

#include "ConstantNode.h"
#include "RbException.h"
#include "UserFunction.h"


/** Basic constructor */
UserFunction::UserFunction(const ArgumentRule** rules, const std::string& retType,
                            std::vector<SyntaxElement*> stmts)
    : RbFunction(rules, retType) {

    for (std::vector<SyntaxElement*>::iterator i=stmts.begin(); i!=stmts.end(); i++)
        code.push_back((*i)->clone());

    //TODO: Store environment
}


/** Copy constructor */
UserFunction::UserFunction(const UserFunction &uf)
    : RbFunction(uf), code(uf.code) {

    for (std::vector<SyntaxElement*>::const_iterator i=uf.code.begin(); i!=uf.code.end(); i++)
        code.push_back((*i)->clone());
}


/** Destructor */
UserFunction::~UserFunction() {

    for (std::vector<SyntaxElement*>::iterator i=code.begin(); i!=code.end(); i++)
        delete (*i);

    delete environment;
}


/** Get class vector describing type of object */
const StringVector& UserFunction::getClass() const {

    static StringVector rbClass = StringVector(RbNames::UserFunction::name) + RbFunction::getClass();
    return rbClass;
}


/** Execute function */
RbObject* UserFunction::execute(std::vector<DAGNode*> arguments) {

    //TODO: Create and add to local environment

    //TODO: Execute code: call getvalue, watch for return signal

    return NULL;
}


