/**
 * @file
 * This file contains the implementation of RbFunction_gtrQ, the
 * gtrQ() function that constructs a scaled instantaneous
 * rate matrix from two simplex parameters, one describing the
 * stationary state frequencies, the other the exchangeability
 * rates.
 *
 * @brief Implementation of RbFunction_gtrQ
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
#include "RbFunction_gtrQ.h"
#include "RbNan.h"
#include "SyntaxLabeledExpr.h"
#include "SymbolTable.h"
#include <list>

using namespace std;

/** Define argument rules */
RbFuncion_gtrQ::argRules = {
    ArgumentRule("pi", RbSimplex()),
    ArgumentRule("revrate", RbSimplex()),
    ArgumentRule()
};

/** Add to symbol table */
symbolTable.addFunction("gtrQ", RbFunction_gtrQ(RbFunction_gtrQ::argRules));

/** Execute function */
RbDataType* RbFunction_gtrQ::execute(void) {

    if ( !result )
        return RbNull;

    RbSimplex *pi = (RbSimplex *) arguments[0]->getValue();
    RbSimplex *r  = (RbSimplex *) arguments[1]->getValue();
    
    /* We rely on pi and r to be of constant dimension */
    int dim = qmat->dim();

    /* Set values */
    for (int i=0; i<dim; i++)
        (*qmat)[i][i] = 0.0;
    for (int i=0; i<dim; i++) {
        for (int j=i+1; j<dim; j++) {
            (*qmat)[i][i] -= (qmat[i][j] = pi[j] * r[index(i,j)]);
            (*qmat)[j][j] -= (qmat[j][i] = pi[i] * r[index(i,j)]);
        }
    }

    /* Scale values */
    double scaler = 0.0;
    for (int i=0; i<dim; i++) {
        scaler += (*qmat)[i][i];
    }
    scaler = -scaler;
    for (int i=0; i<dim; i++) {
        for (int j=0; j<dim; j++) {
            (*qmat)[i][j] /= scaler;
        }
    }

    return(qmat);
}

/** Get data type for type checking */
RbDataType RbFunction_gtrQ::getDataType() {

    if ( !argsSet )
        return RbNull();
    
    return RbQMatrix(dim);
}

/** Set and check arguments */
bool RbFunction_gtrQ::setArguments(list<SyntaxLabeledExpr*> args) {

    if ( !RbFunction::setArguments(args) )
        return false;

    /* Check that dimensions are compatible */
    int dim  = arguments[0]->getDim();
    int dimR = arguments[1]->getDim();
    if ( dimR != (dim*dim - dim) / 2 ) {
        ; // errmsg << "The arguments 'pi' and 'revmat' do not match in dimensions."
    }

    /* Create matrix to hold result */
    result = qmat = new RbQMatrix(dim);

    return true;
}
