/**
 * @file
 * This file contains the declaration of the step function, which is a real value function that
 * can be broken into small constant interval. This general version does not assume equal interval sizes.
 *
 * @brief Declaration of a step-function.
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

#ifndef StepFunction_H
#define StepFunction_H

#include "UnivariateFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    class StepFunction : public UnivariateFunction {
        
    public:
        StepFunction(double *val, const TypedDagNode<double> *x, const std::vector<double> &vals);
        
        virtual StepFunction*           clone(void) const = 0;                                      //!< Create a clon.
//        virtual double              evaluate(double x) const = 0;                               //!< Evaluate function for value x and return f(x)
//        bool                        isDiscrete(void) const;                                     //!< Is this function discrete?
        
        // getters and setters
        //        const std::vector<double>&  getIntervals(void) const;
        std::vector<double>&            getPathValues(void);
        const std::vector<double>&      getPathValues(void) const;
//        void                                            setPathValues(const std::vector<double>& x);
        
    protected:
        // members
        std::vector<double>             values;
//        std::vector<double>         intervals;
    };
    
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const StepFunction& x);                                //!< Overloaded output operator
    
}

#endif
