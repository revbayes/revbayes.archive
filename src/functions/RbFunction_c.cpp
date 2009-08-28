/**
 * @file
 * This file contains the implementation of RbFunction_c, the
 * c() function.
 *
 * @brief Implementation of RbFunction_c
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
#include "RbFunction.h"
#include "RbNull.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/* No argument rules needed */

/** Add to symbol table */
symbolTable.add("c",RbFunction_c());


/** Execute function */
RbDataType* RbFunction_c::execute(void) {

    if ( !result )
        return result;

    result->clear();
    for (list<SyntaxLabeledExpr*>::iterator i=args.begin(), i!=args.end(); i++) {
        RbDataType* val = i->getValue();
        if (val->isType(result->getType()))
            result->addElements(val->getElements());
        else
            result->addElements((*val).as(result->getType()).getElements());
    }
    return(result);
}

/** Get data type for type checking */
RbDataType RbFunction_c::getDataType() {

    if ( arguments.size() == 0) {
        return RbNull();
    }
    
    return arguments[0]->getDataType().getVectorType();
}

/** Set and check arguments */
bool RbFunction_c::setArguments(list<SyntaxLabeledExpr*> args) {

    /* check that all arguments lack a label */
    for (list<SyntaxLabeledExpr*>::iterator i=args.begin(); i!= args.end(); i++) {
        if ( i->getLabel() != NULL ) {
            // errmsg << "Not expecting labeled arguments." << endl;
        }
        if ( args.size() == 0 ) {
            // errmsg << "Expecting at least one argument." << endl;
        }
    }

    /* check that all arguments are of compatible type */
    RbDataType dataType;
    for (list<SyntaxLabeledExpr*>::iterator i=args.begin(); i!= args.end(); i++, count++) {
        SyntaxElement *expr = i->getExpression();
        if ( i == args.begin() )
            dataType = expr->getDataType().getVectorType();
        else {
            if ( (expr->getDataType()).as(dataType->getType()) == RbNull() ) {
                // errmsg << "Element " << count << " is of type '" << expr->getDataType.getType() << "' which cannot be coerced to type "' << dataType.getType() << "'";
            }
        }
        arguments.push_back(expr);
    }

    /* set vector of correct data type to hold results */
    result = arguments[0]->getDataType().getVectorType().copy();
}
