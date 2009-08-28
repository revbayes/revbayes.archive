/**
 * @file
 * This file contains the implementation of RbFunction_dim, the
 * REvBayes dim() function.
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

#include "RbDataType.h"
#include "RbFunction_dim.h"
#include "RbNan.h"
#include "SyntaxLabeledExpr.h"
#include "SymbolTable.h"
#include <list>

using namespace std;

/** Argument rules are the same for all accessor functions */

/** Add to symbol table */
symbolTable.addFunction("dim", RbFunction_dim());


/** Execute function (access attribute) */
RbDataType* RbFunction_dim::execute(void) {

    if ( arguments.size() == 0 )
        return RbNull;

    return (arguments[0]->dim());
}

/** Set attribute */
bool RbFunction_dim::setAttribute(const RbDataType* newValue) {

    /* Check type of new value */
    RbVector *newDim = (RbVector *) newValue;
    if ( !newDim ) {
        // errmsg << "Assignment to dim attribute failed; wrong type '" << newValue->getType() << "'.";
        return false;
    }

    /* Check consistency of new value */
    int size = arguments[0]->getValue()->size();
    int newSize = 1;
    for (int i=0; i<newDim->size(); i++)
        newSize *= newDim[i];
    if ( size != newSize )
        ; // errmsg << "Cannot assign to dim attribute because new value does not agree with current size.";

    /* Set dim attribute */
    arguments[0]->setDim(*newDim);

    return true;
}
