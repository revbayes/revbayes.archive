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
#include <cmath>


/** Define the argument rules */
const ArgumentRule RbFunction_sqrt::argRules[] = {

    ArgumentRule(NULL, RbReal()),
    ArgumentRule()
};

/** Add to symbol table */
static bool fxn_sqrt = SymbolTable::globalTable().add("sqrt", new RbFunction_sqrt());


/** Default constructor, allocate workspace */
RbFunction_sqrt::RbFunction_sqrt()
    : RbStandardFxn(), resultVec(new RbReal()) {
} 

/** Copy constructor */
RbFunction_sqrt::RbFunction_sqrt(const RbFunction_sqrt& s)
    : RbStandardFxn(s), resultVec(new RbReal()) {
}

/** Destructor, delete workspace */
RbFunction_sqrt::~RbFunction_sqrt() {

    delete resultVec;
}

/** Return copy */
RbFunction_sqrt* RbFunction_sqrt::copy() const {

    return new RbFunction_sqrt(*this);
}
    
/** Get argument rules */
const ArgumentRule* RbFunction_sqrt::getArgRules() const {

    return argRules;
}

/** Get data type of result */
const std::string& RbFunction_sqrt::getDataType() const {

    return resultVec->getType();
}

/** Execute function */
RbDataType* RbFunction_sqrt::execute(void) {

    /* Get actual argument */
    RbReal *arg = (RbReal*) arguments[0]->getValue();

    /* Resize result container */
    if ( resultVec->size() != arg->size() )
        resultVec->resize(arg->size());

    /* Compute result */
    for (int i=0; i<arg->size(); i++) {
        if ( (*arg)[i] < 0.0 )
            (*resultVec)[i] =  1E-100;
        else
            (*resultVec)[i] = std::sqrt((*arg)[i]);
    }
    return(resultVec);
}

/** Print some info */
void RbFunction_sqrt::print(std::ostream& c) const {

    c << "RbFunction_sqrt: arg=";
    arguments[0]->print(c);
    c << " -- result=";
    resultVec->print(c);
    c << std::endl;
}
