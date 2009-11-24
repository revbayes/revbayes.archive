/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include <sstream>

#include "Argument.h"


/** Initialize static class attribute */
//const StringVector Argument::rbClass = StringVector("argument") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Standard constructor.
 *
 */
Argument::Argument(const std::string& lbl, DAGNode* n)
    : RbObject(), label(lbl), node(n) {
}


/**
 * @brief Copy constructor
 *
 * Standard copy constructor.
 *
 */
Argument::Argument(const Argument& a)
    : RbObject(), label(a.label), node(a.node) {
}


/**
 * @brief Brief info about the object
 *
 * One-liner on the object
 *
 * @returns     The one-liner
 *
 */
std::string Argument::briefInfo() const {

    std::ostringstream info;
    printValue(info);
    return info.str();
}


RbObject* Argument::clone() const { 

	Argument* arg = new Argument(*this); 
	return (RbObject*)arg;
} 


/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject. It
 * returns equal only if the labels are identical and the
 * nodes are the same.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool Argument::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const Argument* x = dynamic_cast<const Argument*>(obj);
    if (x != NULL)
        return (label == x->label && node == x->node);

    // Try converting the value to an argument
    x = dynamic_cast<const Argument*>(obj->convertTo("argument"));
    if (x == NULL)
        return false;

    bool result = (label == x->label && node == x->node);
    delete x;
    return result;
}


/**
 * @brief Print function
 *
 * This function prints complete info about this object.
 *
 * @param o     The stream for printing
 *
 */
void Argument::print(std::ostream &o) const {

    RbObject::print(o);
    
    o << "Label = " << label << std::endl;
    o << "Node = " << node->briefInfo() << std::endl;
}


/**
 * @brief Print value
 *
 * This function prints the value of the object for
 * the user (implemented here just in case).
 *
 * @param o     The stream for printing
 *
 */
void Argument::printValue(std::ostream &o) const {

    o << "argument(" << label << ", " << node->briefInfo() << ")" << std::endl;
}

