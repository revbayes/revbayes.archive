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

#include "DAGNode.h"
#include "RbDouble.h"
#include "RbFunction_pow.h"
#include "RbObject.h"
#include <cmath>



/** Default constructor */
RbFunction_pow::RbFunction_pow(void) : RbFunction() {

	value = new RbDouble(0.0);
} 

/** Copy constructor */
RbFunction_pow::RbFunction_pow(const RbFunction_pow& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}

/** Destructor */
RbFunction_pow::~RbFunction_pow(void) {

    delete value;
}

/** Clone */
RbObject* RbFunction_pow::clone(void) const {

    RbObject *x = new RbFunction_pow( *this );
    return x;
}

/** Execute the function */
RbObject* RbFunction_pow::executeOperation(const std::vector<DAGNode*>& arguments) {

    RbDouble *arg = (RbDouble*) arguments[0]->getValue();
    if ( arg->getValue() < 0.0 )
        value->setValue(1E-100);
    else
        value->setValue( std::log(arg->getValue()) );
    return value;
}

/** Get the argument rules */
const ArgumentRule** RbFunction_pow::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}

/** Get string showing inheritance */
const StringVector& RbFunction_pow::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_ln_name) + RbFunction::getClass();
	return rbClass;
}

/** Get the return type */
const std::string RbFunction_pow::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}

/** Get string showing value */
std::string RbFunction_pow::toString(void) const {

    RbDouble *x = (RbDouble*) arguments[0]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << value->getValue() << "Log_e( " << x->getValue() << " ) = " << value->getValue();
    return o.str();
}





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

#include "RbFunction_pow.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include <cmath>

const StringVector RbFunction_pow::rbClass = StringVector("power") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_pow = SymbolTable::globalTable().add("pow", new RbFunction_pow());


/** Default constructor, allocate workspace */
RbFunction_pow::RbFunction_pow(void)
    : RbFunction(), value(new RbDouble(0)) {

	argRules.push_back( ArgumentRule("x", "double") );
	argRules.push_back( ArgumentRule("y", "double") );
	returnType = "double";
} 

/** Copy constructor */
RbFunction_pow::RbFunction_pow(const RbFunction_pow& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	argRules.push_back( ArgumentRule("x", "double") );
	argRules.push_back( ArgumentRule("y", "double") );
	returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_pow::~RbFunction_pow() {

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
RbObject* RbFunction_pow::clone(void) const {

    RbFunction_pow *x = new RbFunction_pow( *this );
    return (RbObject*)x;
}

RbObject& RbFunction_pow::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_pow& x = dynamic_cast<const RbFunction_pow&> (obj);

        RbFunction_pow& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_pow& x = dynamic_cast<const RbFunction_pow&> (*(obj.convertTo("pow")));

            RbFunction_pow& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to pow");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_pow& RbFunction_pow::operator=(const RbFunction_pow& obj) {
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
void RbFunction_pow::print(std::ostream &c) const {

    c << "RbFunction_pow" << std::endl;
}

void RbFunction_pow::printValue(std::ostream &o) const {

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
void RbFunction_pow::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_pow not fully implemented!";
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
void RbFunction_pow::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_pow not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_pow::toString(void) const {

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
bool RbFunction_pow::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_pow::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_pow::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
   RbDouble *arg1 = (RbDouble*) arguments[0]->getValue();
   RbDouble *arg2 = (RbDouble*) arguments[1]->getValue();

    /* Compute result */
   value->setValue(std::pow( arg1->getValue(), arg2->getValue() ));

    return value;
}

RbObject* RbFunction_pow::convertTo(const std::string& type) const {

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
bool RbFunction_pow::isConvertibleTo(const std::string& type) const {

    return false;
}
