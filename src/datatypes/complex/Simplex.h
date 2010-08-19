/**
 * @file
 * This file contains the declaration of Simplex, a complex type
 * used to hold a simplex vector.
 *
 * @brief Declaration of Simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Simplex_H
#define Simplex_H

#include "VectorReal.h"

#include <ostream>
#include <string>
#include <vector>

class Simplex : public VectorReal {

    public:
                                    Simplex(void) : VectorReal() {}                        //!< Empty simplex
                                    Simplex(int n);                                    //!< Simplex of length (size) n
                                    Simplex(const std::vector<double>& x);             //!< Simplex from double vector

        // Basic utility functions
        Simplex*                    clone(void) const;                                 //!< Clone object
        bool                        equals(const RbObject* obj) const;                 //!< Equals comparison
        const VectorString&         getClass(void) const;                              //!< Get class
        std::string                 toString(void) const;                              //!< Complete info about object

        // Overloaded operators and built-in functions
        double&                     operator[](int i);                                 //!< Index op allowing change
        void                        pop_back(void);                                    //!< Drop element
        void                        push_back(double x);                               //!< Append element to end
        void                        push_front(double x);                              //!< Add element in front

        // Regular functions
        void                        setValue(const VectorReal& x);                         //!< Set value from Vector
        void                        setValue(const std::vector<double>& x);            //!< Set value from vector<double>
        const std::vector<double>&  getValue(void) const { return value; }             //!< Get value

        // Element access functions for parser
        void                        resize(const VectorInteger& len);                      //!< Resize
        void                        setElement(const VectorInteger& index, RbObject* val); //!< Set element

    private:
        void                        rescale(void);                                     //!< Rescale the simplex
};

#endif


