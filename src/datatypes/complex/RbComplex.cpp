/**
 * @file
 * This file contains the partial implementation of RbComplex,
 * which is the RevBayes abstract base class for complex objects.
 *
 * @brief Partial implementation of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */


#include <string>
#include <ostream>
#include <iostream>

#include "Environment.h"
#include "RbComplex.h"
//#include "StringVector.h"


/** Initialize static class attribute */
//const StringVector RbComplex::rbClass = StringVector("complex") + RbObject::rbClass;


/** Initialize static member template */
const ArgumentRule RbComplex::memberTemplate[] = {

    ArgumentRule()
};


/** Initialize static function array */
const FunctionDef RbComplex::methodDefs[] = {

    FunctionDef()
};


/** Add to base environment */
static bool type_complex = Workspace::globalWorkspace().addType("complex", "object", memberTemplate, methodDefs);


/**
 * @brief Constructor
 *
 * The constructor uses the global workspace (the base environment)
 * to initialize the member variables and retrieve a reference to the
 * function table.
 *
 */
RbComplex::RbComplex() {

	members = Workspace::globalWorkspace().getMembers("complex");
    methods = Workspace::globalWorkspace().getMethodsRef("complex");
}


/**
 * @brief Destructor
 *
 * The destructor deletes the objects that may be stored
 * in the object slots of the members map.
 *
 */
RbComplex::~RbComplex() {

    members.erase();
}


/**
 * @brief Get class attribute
 *
 * Get the class attribute of the object.
 *
 * @returns     A string vector of class names, from
 *              the most derived class to the base class
 *
 */
const StringVector& RbComplex::getClass() const {

	return rbClass;
}


/**
 * @brief Get member variable
 *
 * This function gets the value of a member variable.
 *
 * @param name  The name of the variable
 * @returns     The current value of the variable
 *
 */
const RbObject* RbComplex::getMember(const std::string& name) const {

	ObjectSlot* theSlot = getSlot(name);
    return theSlot->getValue();
}


/**
 * @brief Get method
 *
 * This function gets the named method. It simply asks the global
 * workspace for the method.
 *
 * @param name  The name of the method
 * @returns     A pointer to the method
 *
 */
const RbFunction* RbComplex::getMethod(const std::string& name) const {

    return Workspace::globalWorkSpace.getMethod("complex", name);
}


/**
 * @brief Get methods
 *
 * This function gets all methods of the class, including the inherited
 * ones. The function simply asks the global workspace for the methods.
 *
 * @returns     A map of names to methods
 *
 */
std::map<const std::string, RbFunction*>  RbComplex::getMethods() const {

    return Workspace::globalWorkSpace.getMethods("complex", name);
}


/**
 * @brief Get slot
 *
 * This function gets the slot for the specifed member variable. An error
 * is thrown if the member variable cannot be found.
 *
 * @param name  The name of the member variable
 * @returns     The slot of the variable
 * @throws      Throws an exception if the variable is not found
 *
 */
const ObjectSlot*  RbComplex::getSlot(const std::string& name) const {

    if (members.find(name) != members.end())
        return members[name];

    istringstream msg << "The class 'complex' does not have a member called '" << name << "'" << std:.endl;
    throw (RbException(msg));

    return NULL; // In case the compiler complains about missing return statement
}


/**
 * @brief Print object info
 *
 * Print object info to ostream, including type, member variables
 * and their values, and member functions.
 *
 * @param o The ostream for printing
 *
 */
void RbComplex::print(std::ostream& o) const {

	RbObject::print(o);

    for (std::map<const std::string, ObjectSlot>::iterator i=members.begin(); i!=members.end(); i++) {
        o << "." << i->first << " = ";
        getMember(i->first).printBrief(o);
    }

    std::map<const std::string, RbFunction*> methods = getMethods();

    for (std::map<const std::string, RbFunction*>::iterator i=methods.begin(); i!=methods.end(); i++) {
        vector<string> usages = i->second->getUsage();
        for (vector<string>::iterator i=usages.begin(); i!=usages.end(); i++)
            o << "." << (*i) << std::endl;
    }
}


/**
 * @brief Print object value
 *
 * Print object value to ostream. This includes object class and
 * values of member variables.
 *
 * @param o The ostream for printing
 *
 */
void RbComplex::printValue(std::ostream& o) const {

	o << "Complex object of type " << rbClass[0] << std::endl;

    for (std::map<const std::string, ObjectSlot>::iterator i=members.begin(); i!=members.end(); i++) {
        o << "." << i->first << " = ";
        getMember(i->first).printBrief(o);
        o << std::endl;
    }
}


/**
 * @brief Set member variable
 *
 * This function sets the value of a member variable.
 *
 * @param name  The name of the variable
 * @param val   The new value to be used for the variable
 * @throws      Throws exception if member variable not found
 *
 */
void RbComplex::setMember(const std::string& name, RbObject* val) const {

	ObjectSlot* theSlot = getSlot(name);

    if (theSlot == NULL) {
        istream msg << "No member variable named " << name;
        throw RbException(msg);
    }

    if (!val->isType(theSlot->getType())) {
        istream msg << "The slot for member variable " << name << " is of type "
             << theSlot->getType()[0] << std::endl;
        msg << "It cannot be set with a value of type " << theSlot->getType[0];
        throw RbException(msg);
    }

    if (theSlot->getValue() != NULL)
        theSlot->deleteValue();

    theSlot->setValue(val);
}


