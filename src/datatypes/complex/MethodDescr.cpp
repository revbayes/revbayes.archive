/**
 * @file
 * This file contains the implementation of MethodDescr, which is
 * used to hold the description of a member function. The
 * description consists of a return type and a set of argument
 * rules for the function. The class is used in MethodTable.
 *
 * @brief Implementation of MethodDescr
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: MethodDescr.h 63 2009-11-18 00:05:57Z ronquist $
 */


#include "ArgumentRule.h"
#include "MethodDescr.h"
#include "RbNames.h"

#include <sstream>


/** Constructor */
MethodDescr::MethodDescr(const std::string& retType, const ArgumentRule** argRules)
    : returnType(retType), argRules(argRules) {
}


/** Get class vector describing type of object */
const StringVector& MethodDescr::getClass(void) const { 

    static StringVector rbClass = StringVector(MethodDescr_name) + RbInternal::getClass();
	return rbClass; 
}


/** Print description of function */
void MethodDescr::printValue(std::ostream& o) const {

    o << returnType << " function (";
    for (int i=0; argRules[i] != NULL; i++)
        argRules[i]->printValue(o);
    o << ")";
}


/** Complete info about object */
std::string MethodDescr::toString(void) const {

    std::ostringstream o;
    o << "MethodDescr; value = ";
    printValue(o);

    return o.str();
}

