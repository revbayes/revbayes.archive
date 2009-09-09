/**
 * @file
 * This file contains the implementation of DistGamma, the
 * REvBayes gamma distribution function.
 *
 * @brief Implementation of DistGamma
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

#include "DistGamma.h"
#include "RbStatistics.h"
#include <cmath>


/** Define argument rules */
DistGamma::argRules[] = {
    ArgumentRule("alpha", RbVector()),
    ArgumentRule("beta", RbVector()),
    ArgumentRule()
};

/** Add to symbol table */
symbolTable.addDistribution("gamma", DistGamma());

/** Cumulative density function */
RbVector *DistGamma::cdf() {

    RbVector*     q = (RbVector*) arguments[0]->getValue();
    RbVector* alpha = (RbVector*) arguments[1]->getValue();
    RbVector* beta  = (RbVector*) arguments[2]->getValue();
    RbBool*   logs  = (RbBool*)   arguments[3]->getValue();
    RbBool*   lower = (RbBool*)   arguments[4]->getValue();

    int dim1 = q->size();
    int dim2 = alpha->size();
    int dim3 = beta->size();
    int dim4 = logs->size();
    int dim5 = lower->size();
    int dim0 = max(dim1, dim2, dim3, dim4, dim5);
    vec->resize(dim0);

    for (int i=0; i<dim0; i++) {
        (*vec)[i] = RbStatistics::Gamma:cdf((*q)[i%dim1], (*alpha)[i%dim2], (*beta)[i%dim3]);
        if ( lower[i%dim4] )
            (*vec)[i] = 1.0 - (*vec)[i];
        if ( logs[i%dim5] )
            (*vec)[i] = log((*vec)[i]);
    }

    return vec;
}

/** Ln probability ratio of observation(s) in stochasticNode
 *
 *  We have
 *
 *             (beta^alpha)/(Gamma(alpha)) * xNew^(alpha-1) * exp(-beta * xNew)
 *     ratio = ----------------------------------------------------------------
 *             (beta^alpha)/(Gamma(alpha)) * xOld^(alpha-1) * exp(-beta * xOld)
 *
 *  This simplifies to
 *
 *     log(ratio) = (alpha-1) * log(xNew/xOld) + beta * (xOld - xNew)
 */
RbVector *DistGamma::lnProbRatioObs() {

    RbVector* xOld  = (RbVector*) stochasticNode->getStoredValue();
    RbVector* xNew  = (RbVector*) stochasticNode->getValue();
    RbVector* alpha = (RbVector*) arguments[0]->getValue();
    RbVector* beta  = (RbVector*) arguments[1]->getValue();

    int dim0 = xOld->size();
    int dim1 = alpha->size();
    int dim2 = beta->size();

    double lnRatio = 0.0;
    for (int i=0; i<dim0; i++) {
        double xPrim = (*xNew)[i];
        double x     = (*xOld)[i];
        lnRatio += ((*alpha)[i%dim1] - 1.0) * (log(xPrim) - log(x)) + (*beta)[i%dim2] * (x - xPrim);
    }

    return lnRatio;
}

/** Ln probability ratio of parameters given observation(s) in stochasticNode
 *
 *  We have
 *
 *             (betaNew^alphaNew)/(Gamma(alphaNew)) * x^(alphaNew-1) * exp(-betaNew * x)
 *     ratio = -------------------------------------------------------------------------
 *             (betaOld^alphaOld)/(Gamma(alphaOld)) * x^(alphaOld-1) * exp(-betaOld * x)
 *
 *  This simplifies to
 *
 *     log(ratio) = alphaNew * log(betaNew) - alphaOld * log(betaOld) +
 *                  + lnGamma(alphaOld) - lnGamma(alphaNew) +
 *                  + (alphaNew - alphaOld) * log(x) +
 *                  + (betaOld - betaNew) * x
 */
RbVector *DistGamma::lnProbRatioParams() {

    RbVector* x        = (RbVector*) stochasticNode->getValue();
    RbVector* alphaNew = (RbVector*) arguments[0]->getValue();
    RbVector* alphaOld = (RbVector*) arguments[0]->getStoredValue();
    RbVector* betaNew  = (RbVector*) arguments[1]->getValue();
    RbVector* betaOld  = (RbVector*) arguments[1]->getStoredValue();

    int dim0 = x->size();
    int dim1 = alphaNew->size();
    int dim2 = betaNew->size();

    double lnRatio = 0.0;
    for (int i=0; i<dim0; i++) {
        double x     = (*x)[i];
        double aPrim = (*alphaNew)[i];
        double a     = (*alphaOld)[i];
        double bPrim = (*beta)[i];
        double b     = (*betaOld)[i];
        lnRatio += aPrim * log(bPrim) - a * log(b);
        lnRatio += RbMath::lnGamma(a) - RbMath::lnGamma(aPrim);
        lnRatio += (aPrim - a) * log(x);
        lnRatio += (b - bPrim) * x;
    }

    return lnRatio;
}

/** Probability density function */
RbVector *DistGamma::pdf() {

    RbVector*     x = (RbVector*) arguments[0]->getValue();
    RbVector* alpha = (RbVector*) arguments[1]->getValue();
    RbVector* beta  = (RbVector*) arguments[2]->getValue();
    RbBool*   logs  = (RbBool*)   arguments[3]->getValue();

    int dim1 = x->size();
    int dim2 = alpha->size();
    int dim3 = beta->size();
    int dim4 = logs->size();
    int dim0 = max(dim1, dim2, dim3, dim4);
    vec->resize(dim0);

    for (int i=0; i<dim0; i++) {
        if ( logs[i%dim4] )
            (*vec)[i] = RbStatistics::Gamma:lnPdf((*x)[i%dim1], (*alpha)[i%dim2], (*beta)[i%dim3]);
        else
            (*vec)[i] = RbStatistics::Gamma:pdf  ((*x)[i%dim1], (*alpha)[i%dim2], (*beta)[i%dim3]);
    }

    return vec;
}

/** Calculate quantiles */
RbVector *DistGamma::quantile() {

    RbVector*     p = (RbVector*) arguments[0]->getValue();
    RbVector* alpha = (RbVector*) arguments[1]->getValue();
    RbVector* beta  = (RbVector*) arguments[2]->getValue();
    RbBool*   logs  = (RbBool*)   arguments[3]->getValue();
    RbBool*   lower = (RbBool*)   arguments[4]->getValue();

    int dim1 = p->size();
    int dim2 = alpha->size();
    int dim3 = beta->size();
    int dim4 = logs->size();
    int dim5 = lower->size();
    int dim0 = max(dim1, dim2, dim3, dim4, dim5);
    vec->resize(dim0);

    for (int i=0; i<dim0; i++) {
        double prob = (*p)[i%dim1];
        if ( logs[i%dim4] )
            prob = exp(prob);
        if ( !lower[i%dim5] )
            prob = 1.0 - prob;
        (*vec)[i] = RbStatistics::Gamma:cdf(prob, (*alpha)[i%dim2], (*beta)[i%dim3]);
    }

    return vec;
}

/** Generate random variables */
RbVector *DistGamma::rv() {

    RbInt*    n     = (RbInt*)    arguments[0]->getValue();
    RbVector* alpha = (RbVector*) arguments[1]->getValue();
    RbVector* beta  = (RbVector*) arguments[2]->getValue();

    int dim1 = n->size();
    int dim2 = alpha->size();
    int dim3 = beta->size();
    int dim0 = max(dim1, dim2, dim3);
    vec->resize(dim0);

    for (int i=0; i<dim0; i++) {
        (*vec)[i] = RbStatistics::Gamma:rv((*alpha)[i%dim2], (*beta)[i%dim3]);
    }

    return vec;
}
