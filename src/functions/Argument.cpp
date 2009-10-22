/*
 * Argument.cpp
 *
 *  Created on: 21 okt 2009
 *      Author: Sebastian
 */

#include <string>

#include "Argument.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"

Argument::Argument(std::string& l, DAGNode* n) : RbObject(), label(l) {
    value = n;
}

Argument::~Argument() {
    // TODO Auto-generated destructor stub
}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* Argument::clone(void) const {

    RbObject *x = new Argument( *this );
    return x;
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void Argument::print(std::ostream &c) const {
    //TODO implement

    std::string message = "print function of Argument not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void Argument::dump(std::ostream& c){
    //TODO implement

    std::string message = "dump function of Argument not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void Argument::resurrect(const RbDumpState& x){
    //TODO implement

    std::string message = "resurrect function of Argument not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief brief description
 *
 * Overload operator== for Argument to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
bool Argument::operator==(const RbObject& o) const {
    if (typeid(Argument) == typeid(o)){
        // we are from the same type, which is perfect :)
        Argument& tmp = ((Argument&)o);
        return (*this) == tmp;
    }

    return false;
}

/**
 * @brief brief description
 *
 * Overload operator== for Argument to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
bool Argument::operator==(const Argument& B) const {

    std::string message = "Operator== function of Argument not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
    return true;
}

/**
 * @brief brief description
 *
 * Overload operator== for Argument to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
std::string& Argument::getLabel() const {
    return label;
}

/**
 * @brief brief description
 *
 * Overload operator== for Argument to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
void Argument::setLabel(std::string& l) {
    label = l;
}

/**
 * @brief brief description
 *
 * Overload operator== for Argument to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
DAGNode* Argument::getDAGNode() const {
    return value;
}
