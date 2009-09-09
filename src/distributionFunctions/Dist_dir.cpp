/**
 * @file
 * This file contains the implementation of Dist_dir, the
 * REvBayes Dirichlet distribution function.
 *
 * @brief Implementation of Dist_dir
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

#include "Dist_dir.h"
#include "RbStatistics.h"
#include <cmath>


/** Define argument rules */
Dist_dir::argRules[] = {
    ArgumentRule("weights", RbVector()),
    ArgumentRule()
};

/** Add to symbol table */
symbolTable.addDistribution("dir", Dist_dir());

/** Ln probability ratio of observation(s) in stochasticNode
 *
 *  We have
 *
 *             (1 / Beta(Sum(weights))) * Prod_i (xNew_i^(weights_i - 1))
 *     ratio = ----------------------------------------------------------
 *             (1 / Beta(Sum(weights))) * Prod_i (xOld_i^(weights_i - 1))
 *
 *  where Beta(.) is the multinomial beta function
 *
 *  This simplifies to
 *
 *     log(ratio) = Sum_i ((weights_i - 1) * log(xNew/xOld))
 */
double Dist_dir::lnProbRatioObs() {

    RbSimplex* xOld    = (RbSimplex*) stochasticNode->getStoredValue();
    RbSimplex* xNew    = (RbSimplex*) stochasticNode->getValue();
    RbVector* weights  = (RbVector*)   arguments[0]->getValue();

    int dim0      = xOld->size();
    int dim1      = weights->size();
    it simplexDim = (*weights)[0].size();

    double lnRatio = 0.0;
    for (int i=0; i<dim0; i++) {
        for (int j=0; j<simplexDim; j++) {
            double xPrim = (*xNew)[i][j];
            double x     = (*xOld)[i][j];
            lnRatio += ((*weights)[i%dim1][j] - 1.0) * (log(xPrim) - log(x));
        }
    }

    return lnRatio;
}

/** Ln probability ratio of parameters given observation(s) in stochasticNode
 *
 *  We have
 *
 *             (1 / Beta(Sum(weights))) * Prod_i (xNew_i^(weights_i - 1))
 *     ratio = ----------------------------------------------------------
 *             (1 / Beta(Sum(weights))) * Prod_i (xOld_i^(weights_i - 1))
 *
 * where Beta(.) is the multinomial beta function:
 *
 *               Prod_i (Gamma(weights_i))
 *     Beta(x) = -------------------------
 *                Gamma (Sum_i weights_i)
 *
 *  This simplifies to
 *
 *     log(ratio) = Sum_i (lnGamma(weightsOld_i) - lnGamma(weightsNew_i)) +
 *                  + lnGamma(Sum_i (weightsNew_i)) - lnGamma(Sum_i (weightsOld_i)) +
 *                  + Sum_i ((weightsNew_i - weightsOld_i) * log(x_i))
 */
double Dist_dir::lnProbRatioParams() {

    RbVector* x           = (RbVector*)  stochasticNode->getValue();
    RbSimplex* weightsNew = (RbSimplex*) arguments[0]->getValue();
    RbSimplex* weightsOld = (RbSimplex*) arguments[0]->getStoredValue();

    int dim0       = x->size();
    int dim1       = weightsNew->size();
    int simplexDim = (*x)[0].size();

    double lnRatio = 0.0;
    for (int i=0; i<dim0; i++) {
        double lnGammaSumNew = 0.0;
        double lnGammaSumOld = 0.0;
        double sumWeightsNew = 0.0;
        double sumWeightsOld = 0.0;
        double sumLnXWeights = 0.0;
        for (int j=0; j<simplexDim; j++) {
            double aPrim = (*weightsNew)[i%dim1][j];
            double a     = (*weightsOld)[i%dim1][j];
            lnGammaSumNew += RbMath::lnGamma(aPrim);
            lnGammaSumOld += RbMath::lnGamma(a);
            sumWeightsNew += aPrim;
            sumWeightsOld += a;
            sumLnXWeights += (aPrim - a) * (*x)[i][j];
        }
        lnRatio += lnGammaSumOld - lnGammaSumNew;
        lnRatio += RbMath::lnGamma(sumWeightsNew) - RbMath::lnGamma(sumWeightsOld);
        lnRatio += sumLnXWeights;
    }

    return lnRatio;
}

/** Probability density function */
RbVector *Dist_dir::pdf() {

    RbSimplex* x         = (RbSimplex*) arguments[0]->getValue();
    RbVector*   weights  = (RbVector*)  arguments[1]->getValue();
    RbBool*     logs     = (RbBool*)    arguments[3]->getValue();

    int dim1 = x->size();
    int dim2 = logs->size();
    vec->resize(dim1);

    for (int i=0; i<dim1; i++) {
        if ( logs[i%dim2] )
            (*vec)[i] = RbStatistics::Dirichlet:lnPdf((*weights), (*x)[i]);
        else
            (*vec)[i] = RbStatistics::Dirichlet:pdf  ((*weights), (*x)[i]);
    }

    return vec;
}

/** Generate random variables */
RbDataType *Dist_dir::rv() {

    RbInt*    num     = (RbInt*)     arguments[0]->getValue();
    RbVector* weights = (RbVector*)  arguments[1]->getValue();

    int simplexDim = weights->size();
    int n;
    if (num->size() == 1)
        n = (*num)[0];
    else
        n = num->size();
    vec->resize(n);

    vector<double> z = vector<double>(simplexDim);
    for (int i=0; i<n; i++) {
        for (int j=0; j<simplexDim; j++) {
            z[j] = RbStatistics::Gamma::rv((*weights)[j]);
            (*vec)[i].set(z);
        }
    }

    return vec;
}
