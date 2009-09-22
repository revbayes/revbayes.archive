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
#include "../../modelLanguage/parser/SymbolTable.h"
#include "../../datatypes/RbDataType.h"
#include "../../datatypes/primitive/RbDouble.h"
#include "../../main/RbObject.h"
#include <cmath>


/** Define the argument rules */
const ArgumentRule RbFunction_sqrt::argRules[] = {

    ArgumentRule("x", RbReal()),
    ArgumentRule()
};

/** Add to symbol table */
static bool fxn_sqrt = SymbolTable::globalTable().add("sqrt", new RbFunction_sqrt());


/** Default constructor, allocate workspace */
RbFunction_sqrt::RbFunction_sqrt()
    : AbstractFunction(), value(new RbDouble()) {
} 

/** Copy constructor */
RbFunction_sqrt::RbFunction_sqrt(const RbFunction_sqrt& s)
    : AbstractFunction(s), value(new RbDouble()) {
}

/** Destructor, delete workspace */
RbFunction_sqrt::~RbFunction_sqrt() {

    delete value;
}

/** Return copy */
RbObject* RbFunction_sqrt::clone() const {

    return new RbFunction_sqrt(*this);
}
    
/** Get argument rules */
const ArgumentRule* RbFunction_sqrt::getArgumentRules() const {

    return argRules;
}

/** Execute function */
RbDataType* RbFunction_sqrt::execute(void) {

    /* Get actual argument */
    RbDouble *arg = (RbDouble*) arguments[0]->getValue();

    /* Compute result */
    if ( (*arg)[i] < 0.0 )
        (*value) =  1E-100;
    else
        (*value) = std::sqrt((*arg)[i]);
    }
    return value;
}

/** Print some info */
void RbFunction_sqrt::print(std::ostream& c) const {

    c << "RbFunction_sqrt: arg=";
    arguments[0]->print(c);
    c << " -- result=";
    value->print(c);
    c << std::endl;
}
