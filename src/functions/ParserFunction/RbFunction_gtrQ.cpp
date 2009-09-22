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

#include "RbFunction_gtrQ.h"
#include "RbSimplex.h"
#include "SymbolTable.h"


/** Define the argument rules */
const ArgumentRule RbFunction_gtrQ::argRules[] = {

    ArgumentRule("pi",      RbSimplex()),
    ArgumentRule("revrate", RbSimplex()),
    ArgumentRule()
};

/** Add to symbol table */
static bool fxn_gtrQ = SymbolTable::globalTable().add("gtrQ", new RbFunction_gtrQ());


/** Default constructor, allocate workspace */
RbFunction_gtrQ::RbFunction_gtrQ()
    : RbStandardFxn(), qmatVec(new RbRateMatrix()) {
}

/** Copy constructor */
RbFunction_gtrQ::RbFunction_gtrQ(const RbFunction_gtrQ& s)
    : RbStandardFxn(s), qmatVec(new RbRateMatrix()) {
}

/** Destructor, delete workspace */
RbFunction_gtrQ::~RbFunction_gtrQ() {

    delete qmatVec;
}

/** Return copy */
RbFunction_gtrQ* RbFunction_gtrQ::copy() const {

    return new RbFunction_gtrQ(*this);
}

/** Get argument rules */
const ArgumentRule* RbFunction_gtrQ::getArgRules() const {

    return argRules;
}

/** Get data type of result */
const std::string& RbFunction_gtrQ::getDataType() const {

    return qmatVec->getType();
}

/** Execute function */
RbDataType* RbFunction_gtrQ::execute(void) {

    RbSimplex *piVec = (RbSimplex *) arguments[0]->getValue();
    RbSimplex *rVec  = (RbSimplex *) arguments[1]->getValue();
    
    /* Get dimensions */
    int dim1 = (int) piVec->size();
    int dim2 = (int) rVec->size();
    int dim0 = dim1 > dim2 ? dim1 : dim2;
    qmatVec->resize(dim0);

    /* Loop over vectors */
    for (int i=0; i<dim0; i++) {
        RateMatrix& qmat = (*qmatVec)[i];
        Simplex&    pi   = (*piVec)  [i%dim1];
        Simplex&    r    = (*rVec)   [i%dim2];

        int         rDim    = r.size();
        int         piDim   = pi.size();

        if (rDim != (piDim*piDim - piDim)/2) {
            (*qmatVec)[i] = RateMatrix();
            continue;
        }
        else if (qmat.size() != piDim)
            (*qmatVec)[i] = RateMatrix(piDim);

        /* Calculate raw rate matrix */
        for (int i=0; i<piDim; i++)
            qmat[i][i] = 0.0;
        for (int i=0; i<piDim; i++) {
            for (int j=i+1; j<piDim; j++) {
                qmat[i][i] -= (qmat[i][j] = pi[j] * r[upper(i,j)]);
                qmat[j][j] -= (qmat[j][i] = pi[i] * r[upper(i,j)]);
            }
        }   

        /* Scale values and correct calculating errors */
        double scaler = 0.0;
        for (int i=0; i<piDim; i++) {
            scaler += pi[i]*qmat[i][i];
        }
        scaler = -scaler;
        for (int i=0; i<piDim; i++) {
            for (int j=0; j<piDim; j++) {
                qmat[i][j] /= scaler;
                if (qmat[i][j] < 0.0)
                    qmat[i][j] = 0.0;
            }
        }
    }

    /* Return rate matrix vector */
    return(qmatVec);
}

/** Print some info */
void RbFunction_gtrQ::print(std::ostream& c) const {

    c << "RbFunction_gtrQ: pi=";
    arguments[0]->print(c);
    c << " -- rates=";
    arguments[1]->print(c);
    c << " -- result=";
    qmatVec->print(c);
    c << std::endl;
}

/** Calculate rate index */
int RbFunction_gtrQ::upper(int i, int j) const {

    int diff = i < j ? j - i - 1 : i - j - 1;

    return (((2 + i) * (1 + i)) / 2) + diff;
}
