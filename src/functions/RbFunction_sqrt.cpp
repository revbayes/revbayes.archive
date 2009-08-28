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
 * @since 2009-08-26, version 1.0
 *
 * $Id$
 */

#include "RbDataType.h"
#include "RbFunction_sqrt.h"
#include "RbNan.h"
#include "SyntaxLabeledExpr.h"
#include "SymbolTable.h"
#include <list>

using namespace std;

/** Define argument rules */
RbFuncion_sqrt::argumentRules = {
    ArgumentRule(NULL, RbVector()),
    ArgumentRule()
};

/** Add to symbol table */
symbolTable.addFunction(RbFunction_sqrt());

/** Execute function */
RbDataType* RbFunction_sqrt::execute(void) {

    if ( !result )
        return RbNull;

    /* Get actual arguments */
    RbVector *arg = (RbVector*) arguments[0]->getValue();

    /* Resize result container */
    if ( vec->size() != arg->size() )
        vec->resize(arg->size());

    /* Compute result */
    for (int i=0; i<arg->dim(); i++) {
        if ( arg[i] < 0.0 )
            (*vec)[i] =  RbNan;
        else
            (*vec)[i] = sqrt(arg[i]);
    }
    return(vec);
}

/** Get data type for type checking */
RbDataType RbFunction_sqrt::getDataType() {

    if ( !result ) {
        return RbNull();
    }
    
    int dim = arguments[0]->getDim();
    if ( dim == 1)
        return RbScalar();
    else
        return RbVector(dim);
}

/** Set and check arguments; set result container */
bool RbFunction_sqrt::setArguments(list<SyntaxLabeledExpr*> args) {

    if ( !RbFunction::setArguments(args) )
        return false;

    result = vec = new RbVector();
}
