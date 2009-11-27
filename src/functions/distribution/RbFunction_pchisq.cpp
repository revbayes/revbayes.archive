/**
 * @file
 * This file contains the implementation of RbFunction_pchisq, the
 * pchisq() function.
 *
 * @brief Implementation of RbFunction_pchisq
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

#include "RbFunction_pchisq.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbStatistics.h"

const StringVector RbFunction_pchisq::rbClass = StringVector("pchisq") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_pchisq = SymbolTable::globalTable().add("pchisq", new RbFunction_pchisq());


/** Default constructor, allocate workspace */
RbFunction_pchisq::RbFunction_pchisq(void)
    : RbFunction(), value(new RbDouble(0)) {

	argRules.push_back( ArgumentRule("q", "double", NULL, new RbDouble(0.0), NULL) );
	argRules.push_back( ArgumentRule("df", "int", new RbInt(1), new RbInt(1), NULL) );
	returnType = "double";
} 

/** Copy constructor */
RbFunction_pchisq::RbFunction_pchisq(const RbFunction_pchisq& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	argRules.push_back( ArgumentRule("q", "double", NULL, new RbDouble(0.0), NULL) );
	argRules.push_back( ArgumentRule("df", "int", new RbInt(1), new RbInt(1), NULL) );
	returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_pchisq::~RbFunction_pchisq() {

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
RbObject* RbFunction_pchisq::clone(void) const {

    RbObject *x = new RbFunction_pchisq( *this );
    return x;
}

RbObject& RbFunction_pchisq::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_pchisq& x = dynamic_cast<const RbFunction_pchisq&> (obj);

        RbFunction_pchisq& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_pchisq& x = dynamic_cast<const RbFunction_pchisq&> (*(obj.convertTo("pchisq")));

            RbFunction_pchisq& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to pchisq");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_pchisq& RbFunction_pchisq::operator=(const RbFunction_pchisq& obj) {

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
void RbFunction_pchisq::print(std::ostream &c) const {

    c << "RbFunction_pchisq" << std::endl;
}

void RbFunction_pchisq::printValue(std::ostream &o) const {

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
void RbFunction_pchisq::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_pchisq not fully implemented!";
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
void RbFunction_pchisq::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_pchisq not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_pchisq::toString(void) const {

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
bool RbFunction_pchisq::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_pchisq::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_pchisq::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbInt    *df    = (RbInt*) arguments[1]->getValue();

    value->setValue(RbStatistics::ChiSquare::cdf(*df,*x));

    return value;
}

RbObject* RbFunction_pchisq::convertTo(const std::string& type) const {

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
bool RbFunction_pchisq::isConvertibleTo(const std::string& type) const {

    return false;
}
