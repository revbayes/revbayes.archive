/**
 * @file
 * This file contains the declaration of the interface for numerical intergration
 * of univariate functions.
 *
 * @brief Declaration of numerical integration interface.
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

#ifndef NumericalIntegration_H
#define NumericalIntegration_H


namespace RevBayesCore {

    class UnivariateFunction;

    class NumericalIntegration {
        
    public:
        virtual                ~NumericalIntegration(void) {}
        
        virtual double          integrate(const UnivariateFunction &f, double a, double b) const = 0;                   //!< Evaluate the integral of the function; \int_a^b f(x) dx
    };
}

#endif
