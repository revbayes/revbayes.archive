/**
 * @file
 * This file contains the implementation of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Implementation of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "RbFunction_sqrt.h"
#include "SymbolTable.h"
#include "RbDataType.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbTypeInfo.h"
#include "RbException.h"
#include <cmath>
#include <typeinfo>


/** Define the argument rules */
const ArgumentRule RbFunction_sqrt::argRules[] = {
    ArgumentRule("x", RbTypeInfo(typeid(RbDouble)), RbDouble(-1))
};

/** Add to symbol table */
static bool fxn_sqrt = SymbolTable::globalTable().add("sqrt", new RbFunction_sqrt());


/** Default constructor, allocate workspace */
RbFunction_sqrt::RbFunction_sqrt()
    : RbAbstractFunction(), value(new RbDouble(0)) {
} 

/** Copy constructor */
RbFunction_sqrt::RbFunction_sqrt(const RbFunction_sqrt& s)
    : RbAbstractFunction(s), value(new RbDouble(0)) {
}

/** Destructor, delete workspace */
RbFunction_sqrt::~RbFunction_sqrt() {

    delete value;
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
RbObject* RbFunction_sqrt::clone(void) const {

    RbObject *x = new RbFunction_sqrt( *this );
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
void RbFunction_sqrt::print(std::ostream &c) const {
    c << "RbFunction_sqrt: arg=";
    arguments[0]->print(c);
    c << " -- result=";
    value->print(c);
    c << std::endl;
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
void RbFunction_sqrt::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_sqrt not fully implemented!";
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
void RbFunction_sqrt::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_sqrt not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_sqrt::operator==(const RbObject& o) const {

    if (typeid(RbFunction_sqrt) == typeid(o)){
        // we are from the same type, which is perfect :)
        RbFunction_sqrt& tmp = ((RbFunction_sqrt&)o);
        return (*this) == tmp;
    }

    return false;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_sqrt::operator==(const RbFunction_sqrt& o) const {
    //TODO might check the arguments as well

    //TODO might check the values?!?
//    RbDataType* dt = o.execute();
//    if ((*value) == *dt)
//        return false;

    return true;
}
    
/** Get argument rules */
const ArgumentRule* RbFunction_sqrt::getArgumentRules() const {

    return argRules;
}

/** Get number of argument rules */
const int RbFunction_sqrt::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbDataType* RbFunction_sqrt::execute(void) {

    /* Get actual argument */
    RbDouble *arg = (RbDouble*) arguments[0]->getValue();

    /* Compute result */
    if ( arg->getValue() < 0.0 )
        value->setValue(1E-100);
    else
        value->setValue(std::sqrt(arg->getValue()));

    return value;
}
