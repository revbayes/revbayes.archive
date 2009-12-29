/**
 * @file
 * This file contains the implementation of ObjectSlot, which
 * holds a slot for a variable.
 *
 * @brief Implementation of ObjectSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "ObjectSlot.h"
#include "RbException.h"
#include "Workspace.h"

#include <sstream>


/** Construct from variable */
ObjectSlot::ObjectSlot(DAGNode* var) {

    type     = var->getValueType();
    dim      = var->getDim();
    variable = var;
}


/** Construct from type info */
ObjectSlot::ObjectSlot(const std::string& valType, int valDim) {

    type     = type;
    dim      = valDim;
    variable = NULL;
}


/** Copy constructor */
ObjectSlot::ObjectSlot(const ObjectSlot& x) {

    type = x.type;
    dim  = x.dim;
    if (variable != NULL)
        variable = x.variable->clone();
    else
        variable = NULL;
}


/** Destructor */
ObjectSlot::~ObjectSlot(void) {

    if (variable != NULL)
        delete variable;
}


/** Assignment operator */
ObjectSlot& ObjectSlot::operator=(const ObjectSlot& x) {

    if (this != &x) {
        type = x.type;
        dim  = x.dim;

        if (variable != NULL)
            delete variable;
        variable = x.variable->clone();
    }

    return (*this);
}


/** Get type description with dimensions indicated with "[]" */
std::string ObjectSlot::getTypeDescr(void) const {

    std::string temp = type;
    for (int i=0; i<dim; i++)
        temp += "[]";

    return temp;
}


/** Print value for user */
void ObjectSlot::printValue(std::ostream& o) const {

    if (variable == NULL)
        o << "NULL";
    else
        variable->printValue(o);
}


/** Set variable */
void ObjectSlot::setVariable(DAGNode* var) {

    /* Throw an error if the value is wrong */
    if (var != NULL && !Workspace::userWorkspace().isXOfTypeY(var->getValueType(), type)) {
        std::ostringstream msg;
        msg << getTypeDescr() << " slot does not take variable of type " << var->getValueType();
        for (int i=0; i<var->getDim(); i++)
            msg << "[]";
        delete var;
        throw RbException(msg.str());
    }

    /* Set the slot */
    if (variable != NULL)
        delete variable;
    variable = var;
}


/** Complete info about object */
std::string ObjectSlot::toString(void) const {

    std::ostringstream o;
    o << "ObjectSlot:" << std::endl;
    o << "type     =" << type << std::endl;
    o << "dim      =" << dim << std::endl;
    o << "variable = ";
    variable->printValue(o);

    return o.str();
}


