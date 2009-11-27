/**
 * @file
 * This file contains the implementation of RbFunction_rnorm, the
 * rnorm() function.
 *
 * @brief Implementation of RbFunction_rnorm
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

#include "RbFunction_rnorm.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbStatistics.h"

const StringVector RbFunction_rnorm::rbClass = StringVector("rnorm") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_rnorm = SymbolTable::globalTable().add("rnorm", new RbFunction_rnorm());


/** Default constructor, allocate workspace */
RbFunction_rnorm::RbFunction_rnorm(void)
    : RbFunction(), value(new RbDouble(0)) {

	argRules.push_back( ArgumentRule("n", "int", new RbInt(1), new RbInt(0), NULL) );
	argRules.push_back( ArgumentRule("mu", "double", new RbDouble(0.0)) );
	argRules.push_back( ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
} 

/** Copy constructor */
RbFunction_rnorm::RbFunction_rnorm(const RbFunction_rnorm& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	argRules.push_back( ArgumentRule("n", "int", new RbInt(1), new RbInt(0), NULL) );
	argRules.push_back( ArgumentRule("mu", "double", new RbDouble(0.0)) );
	argRules.push_back( ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_rnorm::~RbFunction_rnorm() {

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
RbObject* RbFunction_rnorm::clone(void) const {

    RbObject *x = new RbFunction_rnorm( *this );
    return x;
}

RbObject& RbFunction_rnorm::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_rnorm& x = dynamic_cast<const RbFunction_rnorm&> (obj);

        RbFunction_rnorm& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_rnorm& x = dynamic_cast<const RbFunction_rnorm&> (*(obj.convertTo("rnorm")));

            RbFunction_rnorm& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to rnorm");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_rnorm& RbFunction_rnorm::operator=(const RbFunction_rnorm& obj) {

    argRules = obj.argRules;
    returnType = obj.returnType;
    (*value) = (*obj.value);
    return (*this);
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
void RbFunction_rnorm::print(std::ostream &c) const {

    c << "RbFunction_rnorm" << std::endl;
}

void RbFunction_rnorm::printValue(std::ostream &o) const {

    o << value << std::endl;
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
void RbFunction_rnorm::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_rnorm not fully implemented!";
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
void RbFunction_rnorm::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_rnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_rnorm::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value->getValue());
	std::string tempStr = temp;
    return "Value = " + tempStr;
}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_rnorm::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_rnorm::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_rnorm::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *n     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

// TODO need to implement
//    value->setValue(RbStatistics::Normal::rnorm(*mu,*sigma,*x));

    return value;
}

RbObject* RbFunction_rnorm::convertTo(const std::string& type) const {

    return NULL;
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbFunction_rnorm::isConvertibleTo(const std::string& type) const {

    return false;
}
