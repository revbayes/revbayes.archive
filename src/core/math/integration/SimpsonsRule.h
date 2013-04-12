/**
 * @file
 * This file contains the declaration of Simpson's rule for numerical intergration
 * of univariate functions.
 *
 * @brief Declaration of Simpson's rule for numerical integration.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id: RandomNumberFactory.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef SimpsonsRule_H
#define SimpsonsRule_H

#include "UnivariateFunction.h"
#include "NumericalIntegration.h"

namespace RevBayesCore {
    
    class SimpsonsRule : public NumericalIntegration {
        
    public:
        
        double          integrate(const UnivariateFunction &f, double a, double b) const;                       //!< Evaluate the integral of the function; \int_a^b f(x) dx
    
    private:
        double          integrateInterval(const UnivariateFunction &f, double a, double m, double b, double f_a, double f_m, double f_b, double integral) const;
    };
}

#endif
