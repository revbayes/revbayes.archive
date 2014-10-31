/**
 * @file
 * This file contains the declaration of general univariate function.
 * A general univariate function is a univariate (continuous) function on the
 * real numbers whose computation is instance specific, i.e. 
 *
 * @brief Declaration of general univariate functions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-10-30, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef GeneralUnivariateFunction_H
#define GeneralUnivariateFunction_H

#include "ConstantNode.h"
#include "UnivariateFunction.h"

#include <iostream>

namespace RevBayesCore {
    
    class GeneralUnivariateFunction : public UnivariateFunction {
        
    public:
        // constructors and destructor
        GeneralUnivariateFunction(double *v, const TypedDagNode<double> *x, TypedDagNode<double> *result, const std::set<DagNode*> &n);
        GeneralUnivariateFunction(const GeneralUnivariateFunction &f);
        virtual                            ~GeneralUnivariateFunction(void) {}
                
        // public methors
        GeneralUnivariateFunction*          clone(void) const;                                                          //!< Clone the function
        double                              evaluate(double x) const;                                                   //!< Compute f(x)
        double                              integrate(double l, double u, NumericalIntegration& i) const;               //!< Compute integral for interval [l,u]
        void                                update(void);                                                               //!< Clone the function
     
    protected:
        virtual void                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        std::set<DagNode*>                  nodes;
        ConstantNode<double>*               parameter;
        TypedDagNode<double>*               y;
    };
    
}

#endif
