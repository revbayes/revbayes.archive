/**
 * @file
 * This file contains the implementation of RbFunction_dnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dnorm
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

#include "DAGNode.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbFunction_dnorm.h"
#include "RbObject.h"
#include "RbStatistics.h"
#include <cmath>
#include <string>

/** Define the static class variables */
const StringVector RbFunction_dnorm::rbClass    = StringVector("dnorm") + RbFunction::rbClass;
const ArgumentRule** RbFunction_dnorm::argRules = { new ArgumentRule("x", "double"), 
												    new ArgumentRule("mu", "double", new RbDouble(0.0)),
													new ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL),
													NULL };
const std::string RbFunction_dnorm::returnType  = "double";

/** Add to symbol table */
//static bool fxn_dnorm = SymbolTable::globalTable().add("dnorm", new RbFunction_dnorm());


/** Default constructor, allocate workspace */
RbFunction_dnorm::RbFunction_dnorm(void)
    : RbFunction(), value(new RbDouble(0)) {

} 

/** Copy constructor */
RbFunction_dnorm::RbFunction_dnorm(const RbFunction_dnorm& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	//argRules.push_back( ArgumentRule("x", "double") );
	//argRules.push_back( ArgumentRule("mu", "double", new RbDouble(0.0)) );
	//argRules.push_back( ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	//returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_dnorm::~RbFunction_dnorm() {

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
RbObject* RbFunction_dnorm::clone(void) const {

    RbObject *x = new RbFunction_dnorm( *this );
    return x;
}

RbObject& RbFunction_dnorm::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_dnorm& x = dynamic_cast<const RbFunction_dnorm&> (obj);

        RbFunction_dnorm& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_dnorm& x = dynamic_cast<const RbFunction_dnorm&> (*(obj.convertTo("dnorm")));

            RbFunction_dnorm& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to dnorm");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_dnorm& RbFunction_dnorm::operator=(const RbFunction_dnorm& obj) {

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
void RbFunction_dnorm::print(std::ostream &c) const {

    c << "RbFunction_dnorm" << std::endl;
}

void RbFunction_dnorm::printValue(std::ostream &o) const {

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
void RbFunction_dnorm::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_dnorm not fully implemented!";
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
void RbFunction_dnorm::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_dnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_dnorm::toString(void) const {

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
bool RbFunction_dnorm::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_dnorm::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_dnorm::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

    value->setValue(RbStatistics::Normal::pdf(*mu,*sigma,*x));

    return value;
}

RbObject* RbFunction_dnorm::convertTo(const std::string& type) const {

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
bool RbFunction_dnorm::isConvertibleTo(const std::string& type) const {

    return false;
}
