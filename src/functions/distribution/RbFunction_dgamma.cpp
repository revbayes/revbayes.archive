/**
 * @file
 * This file contains the implementation of RbFunction_dgamma, the
 * dgamma() function.
 *
 * @brief Implementation of RbFunction_dgamma
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

#include "RbFunction_dgamma.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbStatistics.h"
#include <cmath>

const StringVector RbFunction_dgamma::rbClass = StringVector("dgamma") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_dgamma = SymbolTable::globalTable().add("dgamma", new RbFunction_dgamma());


/** Default constructor, allocate workspace */
RbFunction_dgamma::RbFunction_dgamma(void)
    : RbFunction(), value(new RbDouble(0)) {

	argRules.push_back( ArgumentRule("x", "double", NULL, new RbDouble(0.0), new RbDouble(1.0)) );
	argRules.push_back( ArgumentRule("alpha", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	argRules.push_back( ArgumentRule("gamma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
} 

/** Copy constructor */
RbFunction_dgamma::RbFunction_dgamma(const RbFunction_dgamma& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	argRules.push_back( ArgumentRule("x", "double", NULL, new RbDouble(0.0), new RbDouble(1.0)) );
	argRules.push_back( ArgumentRule("alpha", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	argRules.push_back( ArgumentRule("gamma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_dgamma::~RbFunction_dgamma() {

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
RbObject* RbFunction_dgamma::clone(void) const {

    RbObject *x = new RbFunction_dgamma( *this );
    return x;
}

RbObject& RbFunction_dgamma::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_dgamma& x = dynamic_cast<const RbFunction_dgamma&> (obj);

        RbFunction_dgamma& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_dgamma& x = dynamic_cast<const RbFunction_dgamma&> (*(obj.convertTo("dgamma")));

            RbFunction_dgamma& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to dgamma");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_dgamma& RbFunction_dgamma::operator=(const RbFunction_dgamma& obj) {

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
void RbFunction_dgamma::print(std::ostream &c) const {

    c << "RbFunction_dgamma" << std::endl;
}

void RbFunction_dgamma::printValue(std::ostream &o) const {

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
void RbFunction_dgamma::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_dgamma not fully implemented!";
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
void RbFunction_dgamma::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_dgamma not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_dgamma::toString(void) const {

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
bool RbFunction_dgamma::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_dgamma::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_dgamma::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

    value->setValue(RbStatistics::gamma::pdf(*mu,*sigma,*x));

    return value;
}

RbObject* RbFunction_dgamma::convertTo(const std::string& type) const {

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
bool RbFunction_dgamma::isConvertibleTo(const std::string& type) const {

    return false;
}
