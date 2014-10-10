/**
 * @file
 * This file contains the declaration of Complex, which is the
 * primitive RevBayes type for complex numbers.
 *
 * @brief Declaration of Complex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Complex_H
#define Complex_H

#include "ModelObject.h"
#include "TypedDagNode.h"

#include <complex>
#include <ostream>
#include <string>

namespace RevLanguage {

    class Complex : public ModelObject<std::complex<double> > {

        public:
            Complex(void);                                                                                      //!< Default constructor (0.0, 0.0)
            Complex(const Complex& c);                                                                          //!< Copy constructor
            Complex(const double x);                                                                            //!< Construct from double
            Complex(const int x);                                                                               //!< Construct from int 
            Complex(const std::complex<double>& c);                                                             //!< Construct from complex

            
            // Basic utility functions
            Complex*                                clone(void) const;                                          //!< Clone object
            static const std::string&               getClassType(void);                                         //!< Get Rev type
            static const TypeSpec&                  getClassTypeSpec(void);                                     //!< Get class type spec
            const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
            void                                    printValue(std::ostream& o) const;                          //!< Print value (for user)
    
    };
        
}

#endif

