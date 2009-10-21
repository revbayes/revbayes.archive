/*!
 * \file
 * This file contains the implementation of ArgumentRule, which is
 * used to describe rules for arguments passed to functions (and
 * distributions).
 *
 * \brief Implementation of ArgumentRule
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes core team
 * \license GPL version 3.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "RbTypeInfo.h"
#include "RbDataType.h"

/** Constructor for keyword = value arguments. Keyword can be set to "", signalling no label */
ArgumentRule::ArgumentRule (const std::string& key, const RbTypeInfo& type, const RbDataType& defVal)
    : returnType(type), label(key), defaultValue(defVal) {

}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param ot    the object to copy
 */
ArgumentRule::ArgumentRule(const ArgumentRule& ar) : returnType(ar.getReturnType()), label(ar.getLabel()), defaultValue(ar.getDefaultValue()) {

}

///** Constructor for keyword = value arguments. Keyword can be set to "", signalling no label */
//ArgumentRule::ArgumentRule (const std::string& key, const std::string& type, const RbDataType* defVal)
//    : returnType(type), label(key), defaultValue(defVal) {
//}

ArgumentRule::~ArgumentRule() {
//    delete defaultValue;
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
RbObject* ArgumentRule::clone(void) const {

    RbObject *x = new ArgumentRule( *this );
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
void ArgumentRule::print(std::ostream &c) const {
    //TODO implement
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
void ArgumentRule::dump(std::ostream& c){
    //TODO implement

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
void ArgumentRule::resurrect(const RbDumpState& x){
    //TODO implement

}

/**
 * @brief brief description
 *
 * Overload operator== for ArgumentRule to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
bool ArgumentRule::operator==(const RbObject& o) const {
    if (typeid(ArgumentRule) == typeid(o)){
        // we are from the same type, which is perfect :)
        ArgumentRule& tmp = ((ArgumentRule&)o);
        return (*this) == tmp;
    }

    return false;
}
/**
 * @brief brief description
 *
 * Overload operator== for ArgumentRule to allow static C-style arrays terminated by ArgumentRule()
 *
 * @param B     second argument rule for comparison
 * @returns     true if both argument rules are the same
 */
bool ArgumentRule::operator==(const ArgumentRule& B) const {

    const ArgumentRule& A = (*this);
    const RbTypeInfo& returnTypeA = A.getReturnType();
    const RbTypeInfo& returnTypeB = B.getReturnType();
    if (!(returnTypeA == returnTypeB))
        return false;
    if (A.getLabel() != B.getLabel())
        return false;

    /** @todo Implement operator!= for datatypes
    if (A.defaultValue() != B.defaultValue())
        return false;
    */

    return true;
}

/**
 * @brief Getter function for the return type
 *
 * Get return type
 *
 * @returns     the return type of this function
 */
const RbTypeInfo& ArgumentRule::getReturnType() const {
    return returnType;
}

/**
 * @brief get a default value
 *
 * This function returns the default value of the function
 *
 * @returns     the default value
 */
RbDataType& ArgumentRule::getDefaultValue() const {
    // TODO what do we need this for???
    RbDataType* dt = (RbDataType*) defaultValue.clone();
    return *dt;
}

/**
 * @brief Get label for the argument
 *
 * This function is the getter function for the label of the argument
 *
 * @returns     returns the label of the argument
 */
const std::string& ArgumentRule::getLabel() const {
    return label;
}

/**
 * @brief Checks if the argument is of a valid type
 *
 * This function validates the data type of the argument against the expected data type specified by the argument rule.
 *
 * @param x     the type of the argument
 * @returns     true if the type matches with the specification of the rule
 */
bool ArgumentRule::isTypeValid(const RbTypeInfo& x) const {
    return x == returnType;
}

