/**
 * @file
 * This file contains the partial implementation of MemberObject, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of MemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"


/** Constructor: we set member variables here from member rules, derived classes deal with functions */
MemberObject::MemberObject(const ArgumentRule** memberRules) : RbComplex() {

    /* Fill member table based on member rules */
    for (int i=0; memberRules[i]!=NULL; i++) {
        std::string name = memberRules[i]->getLabel();
        ConstantNode* theVar;
        if (memberRules[i]->getDefaultValue().isType(RbUndefined_name))
            theVar = NULL;
        else {
            theVar = new ConstantNode(memberRules[i]->getDefaultValue().clone());
            theVar->setName(memberRules[i]->getLabel());
        }
        members.insert(std::pair<std::string, DAGNode*>(name, theVar));
    }
}


/** Get class vector describing type of object */
const StringVector& MemberObject::getClass(void) const {

    static StringVector rbClass = StringVector(MemberObject_name) + RbComplex::getClass();
    return rbClass;
}


/** Execute member function with preprocessed arguments */
const RbObject* MemberObject::executeMethod(const std::string& name) {

    /* Find the method */
    std::map<std::string, MemberFunction*>::iterator it = methods.find(name);
    if (it == methods.end())
        throw RbException("No method named '" + name + "'");
    MemberFunction* theMethod = (*it).second;

    /* Get preprocessed arguments */
    std::vector<DAGNode*> arguments = theMethod->getProcessedArguments();

    /* Execute the operation */
    return executeOperation(name, arguments);
}


/** Execute member function */
const RbObject* MemberObject::executeMethod(const std::string& name, std::vector<Argument>& args) {

    /* Find the method */
    std::map<std::string, MemberFunction*>::iterator it = methods.find(name);
    if (it == methods.end())
        throw RbException("No method named '" + name + "'");
    MemberFunction* theMethod = (*it).second;

    /* Process the arguments */
    if (!theMethod->processArguments(args))
        throw RbException("Arguments do not match method '" + name + "'");
    std::vector<DAGNode*> arguments = theMethod->getProcessedArguments();

    /* Execute the operation */
    return executeOperation(name, arguments);
}


/** Get value of a member variable */
const RbObject* MemberObject::getValue(const std::string& name) const {

    return getVariable(name)->getValue();
}


/** Get a member variable */
const DAGNode* MemberObject::getVariable(const std::string& name) const {

    std::map<std::string, DAGNode*>::const_iterator it = members.find(name);
    if (it == members.end())
        throw RbException("No member variable called '" + name + "'");

    return (*it).second;
}


/** Set arguments of a member function */
void MemberObject::setArguments(const std::string& name, std::vector<Argument>& args) {

    /* Find the method */
    std::map<std::string, MemberFunction*>::iterator it = methods.find(name);
    if (it == methods.end())
        throw RbException("No method named '" + name + "'");
    MemberFunction* theMethod = (*it).second;

    /* Process the arguments */
    if (!theMethod->processArguments(args))
        throw RbException("Arguments do not match method '" + name + "'");
}


/** Set value of a member variable */
void MemberObject::setValue(const std::string& name, RbObject* val) {

    /* Wrap in a constant node */
    ConstantNode* theVar = new ConstantNode(val);

    /* Set variable with the constant node */
    setVariable(name, theVar);
}


/** Set a member variable */
void MemberObject::setVariable(const std::string& name, DAGNode* var) {

    /* Find the rule */
    const ArgumentRule** theRules = getMemberRules();
    int i;
    for (i=0; theRules[i]!=NULL; i++) {
        if (theRules[i]->getLabel() == name)
            break;
    }
    if (theRules[i] == NULL)
        throw RbException("No member variable '" + name + "'");

    /* Check if var is valid */
    if (!theRules[i]->isArgValid(var))
        throw RbException("Invalid assignment: type mismatch");

    /* Find variable and remove */
    MemberTable::iterator it=members.find(name);
    (*it).second->setName("");
    if ((*it).second != NULL && (*it).second->numRefs() == 0)
        delete (*it).second;

    /* Set variable */
    var->setName(name);
    (*it).second = var;
}


