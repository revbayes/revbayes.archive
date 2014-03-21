/**
 * @file
 * This file contains the declaration of the interface for all univariate continuous functions. 
 * Univariate functions are continuous functions with only one single parameter, which is of type real.
 * Univariate functions provide a method for computing the integral in the interval [a,b].
 *
 * @brief Declaration of continous functions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-02, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef UnivariateFunction_H
#define UnivariateFunction_H

#include "ContinuousFunction.h"
#include "NumericalIntegration.h"
#include "TypedDagNode.h"

#include <iostream>

namespace RevBayesCore {
    
    class UnivariateFunction : public ContinuousFunction {
        
    public:
        // constructors and destructor
        virtual                            ~UnivariateFunction(void) {}
        
        // public methods
        const TypedDagNode<double>*         getParameter(void) const;                                                       //!< Get the parameter for convienent access.
        
        // pure virtual public methors
        virtual UnivariateFunction*         clone(void) const = 0;                                                          //!< Clone the function
        virtual double                      evaluate(double x) const = 0;                                                   //!< Compute f(x)
        virtual double                      integrate(double l, double u, NumericalIntegration& i) const = 0;               //!< Compute integral for interval [l,u]
        virtual void                        update(void) = 0;                                                               //!< Clone the function
        
    protected:
        UnivariateFunction(double *v, const TypedDagNode<double> *x);
        
        virtual void                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
        const TypedDagNode<double>*         x;
    };
    
}

#endif
