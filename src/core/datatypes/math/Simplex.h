/**
 * @file
 * This file contains the declaration of Matrix,
 * a container type used to hold value matrices for the inference machinery.
 *
 * @brief Declaration of Matrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-05-08, version 1.0
 *
 * $Id: Matrix.h 1327 2012-03-10 11:55:11Z hoehna $
 */

#ifndef Simplex_H
#define Simplex_H

#include "Cloneable.h"
#include "MemberObject.h"
#include "RbVector.h"

#include <cstddef>
#include <iostream>
#include <vector>

namespace RevBayesCore {
    
    class EigenSystem;
    
    class Simplex : public RbVector<double> {
        
    public:
        Simplex(void);                       //!< Default constructor required by revlanguage use of this class
        Simplex(size_t n);
        Simplex(size_t n, double v);
        Simplex(const std::vector<double> &v);
        Simplex(const Simplex& m);
        virtual                                ~Simplex(void);
        
        Simplex*                                clone(void) const;
        
        // utility funcions
        virtual void                            initFromString(const std::string &s);
        
    protected:
        
        void                                    normalize(void);
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Simplex& x);                                           //!< Overloaded output operator
    
    
}

#endif

