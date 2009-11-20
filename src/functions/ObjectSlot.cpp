/**
 * @file
 * This file contains the implementation of ObjectSlot, which is
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

#include "ObjectSlot.h"
#include "RbException.h"

//static StringVector ObjectSlot::rbClass = StringVector("objectSlot") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from a type and an (optional) default
 * value.
 *
 * @param v     Value of the object
 *
 */
ObjectSlot::ObjectSlot(const std::string valType, RbObject* defaultVal)
    : RbObject(), type(valType), value(defaultVal) {
}


/**
 * @brief Brief info about the object
 *
 * Provides brief info about the object in a string.
 *
 * @returns     Short string describing object
 *
 */
std::string ObjectSlot::briefInfo() const {

	std::ostringstream o;

    printValue(o);
    
    return o.str();
}


/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject. It
 * returns equal only if the slots contain the same object,
 * which should never happen.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool ObjectSlot::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const ObjectSlot* x = dynamic_cast<const ObjectSlot*>(obj);
    if (x != NULL)
        return (value == x->value && type == x->type);

    // Try converting the value to an objectSlot
    x = dynamic_cast<const ObjectSlot*>(obj->convertTo("objectSlot"));
    if (x == NULL)
        return false;

    bool result = (value == x->value && type == x->type);
    delete x;
    return result;
}

/**
 * @brief Print complete info
 *
 * Prints complete object info.
 *
 * @param o     The stream for printing
 *
 */
void ObjectSlot::print(std::ostream &o) const {

	RbObject::print(o);
    o << "Type = " << type << std::endl;
    o << "Value = ";
    if (value == NULL)
        o << "null" << std::endl;
    else
        o << value->briefInfo() << std::endl;
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
void ObjectSlot::printValue(std::ostream &o) const {

    o << "objectSlot (" << type << ", ";

    if (value == NULL)
        o << "null)" << std::endl;
    else
        o << value->briefInfo() << ")" << std::endl;
}


/**
 * @brief Set value
 *
 * Sets the value if the type is correct, otherwise
 * it throws an error.
 *
 * @param val   The new value of the slot
 * @throws      The new value is of the wrong type
 *
 */
void ObjectSlot::setValue(RbObject* val) {

    if (!val->isType(type)) {
        std::ostringstream msg;
        msg << "The slot requires a value of type " << type << std::endl;
        msg << "You tried to set it to a value of type " << /* val->getClass()[0] << */ std::endl;
        throw RbException(msg.str());
    }
    
    if (value != NULL)
        delete value;
    
    value = val;
}

