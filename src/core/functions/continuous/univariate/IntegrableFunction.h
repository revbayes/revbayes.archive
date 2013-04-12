/**
 * @file
 * This file contains the declaration of the interface for all integrable functions. 
 *
 * All integrable functions need to implement the function integrate.
 *
 *
 * @brief Declaration of univariate continous functions.
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

#ifndef IntegrableFunction_H
#define IntegrableFunction_H

#include <iostream>

namespace RevBayesCore {
    
    class IntegrableFunction {
        
    public:
        // constructors and destructor
        virtual                            ~IntegrableFunction(void) {}
        
        // pure virtual public methors
        virtual IntegrableFunction*         clone(void) const = 0;                                  //!< Clone the function
        virtual double                      integrate(double l, double u) const = 0;                //!< Compute integral for interval [l,u]
        
    protected:
        IntegrableFunction(void) {}
        
    };
    
    // Global functions using the class
    std::ostream&               operator<<(std::ostream& o, const IntegrableFunction& x);           //!< Overloaded output operator

}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const IntegrableFunction& f) {
    
    o << "integral(x)";
    
    return o;
}

#endif
