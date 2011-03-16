/**
 * @file
 * This file contains the declaration of Simplex, a complex type
 * used to hold a simplex vector.
 *
 * @brief Declaration of Simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Simplex_H
#define Simplex_H

#include "VectorRealPos.h"

#include <ostream>
#include <string>
#include <vector>

class Simplex : public VectorRealPos {

    public:
                                    Simplex(void) : VectorRealPos() {}                  //!< Empty simplex
                                    Simplex(const size_t n);                            //!< Simplex of length (size) n
                                    Simplex(const std::vector<double>& x);              //!< Simplex from double vector
                                    Simplex(const VectorRealPos& x);                    //!< Simplex from positive real vector

        // Basic utility functions
        Simplex*                    clone(void) const;                                     //!< Clone object
        bool                        equals(const RbObject* obj) const;                     //!< Equals comparison
        const VectorString&         getClass(void) const;                                  //!< Get class
        std::string                 toString(void) const;                                  //!< Complete info about object

        // Overloaded operators and built-in functions
        double&                     operator[](size_t i);                                   //!< Index op allowing change - throw error
        void                        pop_back(void);                                         //!< Drop element - throw error
        void                        push_back(double x);                                    //!< Append element to end - throw error 
        void                        push_front(double x);                                   //!< Add element - throw error

        // Regular functions
        void                        setValue(const VectorRealPos& x);                       //!< Set value from VectorRealPos & rescale
        void                        setValue(const std::vector<double>& x);                 //!< Set value from vector<double>, check & rescale
        const std::vector<double>&  getValue(void) const { return value; }                  //!< Get value

        // Element access functions for parser
        void                        resize(const VectorInteger& len);                       //!< Resize - throw error
        void                        setElement(const VectorInteger& index, RbObject* val);  //!< Set element - throw error

    private:
        void                        rescale(void);                                          //!< Rescale the simplex
};

#endif

