/**
 * @file
 * This file contains the implementation of ObjectSlot, which is
 * a slot for a variable. It contains a specification of the
 * declared (required) type and dimensions of the object, in
 * addition to the current value.
 *
 * @brief Implementation of ObjectSlot
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "DAGNode.h"
#include "ObjectSlot.h"
#include "RbException.h"
#include "RbNames.h"

#include <sstream>


/** Constructor with NULL init */
ObjectSlot::ObjectSlot(const std::string valType, int elementDim)
    : type(valType), dim(elementDim), value(NULL) {
}


/** Constructor with object init */
ObjectSlot::ObjectSlot(RbObject* initVal) {

    // DAGNode is inserted as the wrapper and its value has the type info
    if (initVal->isType(RbNames::DAGNode::name)) {
        type  = ((DAGNode*)(initVal))->getValue()->getType();
        dim   = ((DAGNode*)(initVal))->getValue()->getElementDim();
        value = initVal;
    }
    // Container is inserted as the wrapper and elementDim and elementType has the type info
    else if (initVal->getElementDim() != 0) {
        type  = initVal->getElementType();
        dim   = initVal->getElementDim();
        value = initVal;
    }
    // Constant object is inserted and a new constant node wrapper is created
    else {
        type  = initVal->getType();
        dim   = 0;
        value = new ConstantNode(initVal);
    }   
}


/** Get description of type with indication of dimensions with empty square brackets */
std::string ObjectSlot::getTypeDescr() const {

    std::string s = type;
    for (int i=0; i<dim; i++)
        s += "[]";

    return s;
}


/** Print complete info about object */
void ObjectSlot::print(std::ostream& o) const {

	o << "ObjectSlot: " << getTypeDescr() << " = ";
    if (value == NULL)
        o << "NULL" << std::endl;
    else
        o << value->briefInfo() << std::endl;
}


/**
 * @brief Set value
 *
 * Sets the value if the type is correct, otherwise
 * it throws an error. The old value is deleted.
 *
 * @param val   The new value of the slot
 * @throws      The new value is of the wrong type
 *
 */
void ObjectSlot::setValue(RbObject* val) {

    if (dim == 0 && (val->getElementDim() != 0 || !val->isType(type))) {
        std::ostringstream msg;
        msg << "Cannot set " << getTypeDescr() << " slot with value of type ";
        if (val->getElementDim() == 0)
            msg << val->getType();
        else {
            msg << val->getElementType();
            for (int i=0; i<dim; i++)
                msg << "[]";
        }
        throw RbException(msg.str());
    }
    else if (val->getElementDim() != dim || val->getElementType() != type) {
        std::ostringstream msg;
        msg << "Cannot set " << getTypeDescr() << " slot with value of type ";
        if (val->getElementDim() == 0)
            msg << val->getType();
        else {
            msg << val->getElementType();
            for (int i=0; i<dim; i++)
                msg << "[]";
        }
        throw RbException(msg.str());
    }

    if (value != NULL)
        delete value;
    
    value = val;
}

