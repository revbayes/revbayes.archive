/**
 * @file
 * This file contains the declaration of VectorComplex, a complex type
 * used to hold complex vectors.
 *
 * @brief Declaration of VectorComplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#ifndef VectorComplex_H
#define VectorComplex_H

#include "Complex.h"
#include "Vector.h"
#include <iostream>
#include <vector>



const std::string VectorComplex_name = "Complex vector";

class VectorComplex : public Vector {

    public:
                                           VectorComplex(void);                                        //!< Default constructor (empty vector)
                                           VectorComplex(const std::string& elemType);                 //!< Default constructor for derived classes
                                           VectorComplex(double x);                                    //!< Construct vector with one double x
                                           VectorComplex(size_t n, double x);                          //!< Construct vector with n doubles x
                                           VectorComplex(const std::vector<double>& x);                //!< Constructor from double vector
                                           VectorComplex(const std::vector<std::complex<double> >& x); //!< Constructor from double vector
                                           VectorComplex(const VectorComplex& x);                      //!< Constructor from VectorComplex
                                           VectorComplex(size_t n, std::complex<double> x);            //!< Construct vector with n complex x

        // Overloaded operators
        std::complex<double>&              operator[](size_t i);                                       //!< Index op allowing change
        const std::complex<double>&        operator[](size_t i) const;                                 //!< Const index op
        bool                               operator==(const VectorComplex& x) const;                   //!< Equals comparison
        bool                               operator!=(const VectorComplex& x) const;                   //!< Not equals comparison
        
        // Basic utility functions
        VectorComplex*                     clone(void) const;                                          //!< Clone object
        const VectorString&                getClass(void) const;                                       //!< Get class
        void                               printValue(std::ostream& o) const;                          //!< Print value (for user)
        std::string                        richInfo(void) const;                                       //!< Complete info about object

        // Vector functions, including STL-like functions
        std::vector<std::complex<double> > getValue(void) const;                                       //!< Get value
        void                               push_back(std::complex<double> x);                          //!< Append element to end
        void                               push_front(std::complex<double> x);                         //!< Add element in front
        void                               setValue(const VectorComplex& x);                           //!< Set the value
        void                               setValue(const std::vector<std::complex<double> >& x);      //!< Set the value

    protected:

};

#endif

