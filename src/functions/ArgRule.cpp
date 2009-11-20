/**
 * @file
 * This file contains the implementation of ArgRule, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of the class RbInt
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include <iostream>

#include "ArgRule.h"
#include "RbException.h"

//static StringVector ArgRule::rbClass = StringVector("argRule") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from a type and an (optional) default
 * value.
 *
 * @param v     Value of the object
 *
 */
ArgRule::ArgRule(const std::string& lab, ObjectSlot* slot)
    : RbObject(), label(lab), objectSlot(slot) {
}


/**
 * @brief Brief info about the object
 *
 * Provides brief info about the object in a string.
 *
 * @returns     Short string describing object
 *
 */
std::string ArgRule::briefInfo() const {

	std::ostringstream o;
    printValue(o);
    return o.str();
}


/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject. It
 * returns equal only if the type and label are identical, and
 * the slots contain the same object, which should never happen.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool ArgRule::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const ArgRule* x = dynamic_cast<const ArgRule*>(obj);
    if (x != NULL)
        return (label == x->label && getValue() == x->getValue() && getType() == x->getType());

    // Try converting the value to an argRule
    x = dynamic_cast<const ArgRule*>(obj->convertTo("argRule"));
    if (x == NULL)
        return false;

    bool result = (label == x->label && getValue() == x->getValue() && getType() == x->getType());
    delete x;
    return result;
}


/**
 * @brief Test if argument is valid
 *
 * Checks that the type is right and returns based on this.
 *
 * @param val   The object to be tested
 * @returns     Result of comparison
 *
 */
bool ArgRule::isArgValid(const RbObject* obj) const {

    return obj->isType(getType());
}


/**
 * @brief Print complete info
 *
 * Prints complete object info.
 *
 * @param o     The stream for printing
 *
 */
void ArgRule::print(std::ostream &o) const {

	RbObject::print(o);
    o << "Label = " << label << std::endl;
    o << "Type = " << objectSlot->getType() << std::endl;
    o << "Value = ";
    if (objectSlot->getValue() == NULL)
        o << "null" << std::endl;
    else
        o << objectSlot->getValue()->briefInfo() << std::endl;
}


/**
 * @brief Print value
 *
 * Prints value for user (just in case we later make
 * objects of this kind available to the parser).
 *
 * @param o     The stream for printing
 *
 */
void ArgRule::printValue(std::ostream &o) const {

    o << "argRule (" << label << ", ";
    o << objectSlot->briefInfo() << ")" << std::endl;
}


