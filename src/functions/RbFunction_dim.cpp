/**
 * @file
 * This file contains the implementation of RbFunction_dim,
 * the dim() function.
 *
 * @brief Implementation of RbFunction_dim
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "RbFunction_dim.h"
#include "SymbolTable.h"

/** Argument rules are the same for all accessor functions */

/** Add to symbol table */
static bool fxn_dim = SymbolTable::globalTable().add("dim", new RbFunction_dim());


/** Execute function (access attribute) */
RbDataType* RbFunction_dim::execute(void) {

    if ( arguments.size() == 0 )
        return NULL;

    return (arguments[0]->getValue()->getDim()->copy());
}

/** Set attribute */
bool RbFunction_dim::setAttribute(const RbDataType* newValue) {

    /* Check type of new value */
    if (newValue->getType() != RbInt::dataType) {
        cerr << "New dim attribute must be a vector of int values";
        return false;
    }

    RbInt& newDim = *((RbInt*) newValue);

    /* Check consistency of new value */
    int size = arguments[0]->getValue()->getSize();
    int newSize = 1;
    for (int i=0; i<newDim.size(); i++)
        newSize *= newDim[i];
    if ( size != newSize ) {
        cerr << "Cannot assign to dim attribute because new value does not agree with current size";
    }

    /* Set dim attribute */
    arguments[0]->getValue()->setDim(newDim);

    /* Success */
    return true;
}
