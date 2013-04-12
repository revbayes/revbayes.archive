/**
 * @file
 * This file contains the declaration of the constant step function, which is broken into
 * equally sized intervals and the value is computed as a constant function for each interval.
 *
 * @brief Declaration of the constant step function.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-13, version 1.0
 *
 * $Id: RandomNumberFactory.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef ConstantStepFunction_H
#define ConstantStepFunction_H

#include "UnivariateFunction.h"
#include "StepFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class ConstantStepFunction : public StepFunction {
        
    public:
        ConstantStepFunction(const TypedDagNode<double> *x, const std::vector<double> &vals, double f, double t);
        
        ConstantStepFunction*                       clone(void) const;                                                  //!< Create a clon.
        double                                      evaluate(double x) const;                                           //!< Compute f(x)
        double                                      integrate(double l, double u, NumericalIntegration& i) const;
        void                                        update(void);                                                       //!< Recompute the value
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters

    private:
        double                                      from;
        double                                      to;
    };
}

#endif
