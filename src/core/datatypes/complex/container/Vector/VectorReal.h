/**
 * @file
 * This file contains the declaration of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Declaration of VectorReal
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

#ifndef VectorReal_H
#define VectorReal_H

#include "Real.h"
#include "Vector.h"

#include <iostream>
#include <vector>

const std::string VectorReal_name = "Real vector";

class VectorRealPos;


/**
 * @brief Class used for real vectors.
 *
 * @note VectorRealPos cannot be derived
 *       from this class without giving
 *       inappropriate access to elements.
 */

class VectorReal : public Vector {

    public:
                                    VectorReal(void);                                           //!< Default constructor (empty vector)
                                    VectorReal(const std::string& elemType);                    //!< Default constructor for derived classes
                                    VectorReal(double x);                                       //!< Construct vector with one double x
                                    VectorReal(size_t n, double x);                             //!< Construct vector with n doubles x
                                    VectorReal(const std::vector<double>& x);                   //!< Constructor from double vector
                                    VectorReal(const VectorRealPos& x);                         //!< Constructor from VectorRealPos

        // Overloaded operators
        double&                     operator[](size_t i);                                       //!< Index op allowing change
        const double&               operator[](size_t i) const;                                 //!< Const index op
        bool                        operator==(const VectorReal& x) const;                      //!< Equals comparison
        bool                        operator!=(const VectorReal& x) const;                      //!< Not equals comparison
        
        // Basic utility functions
        VectorReal*                 clone(void) const;                                          //!< Clone object
        RbLanguageObject*           convertTo(const std::string& type) const;                   //!< Convert to type
        const VectorString&         getClass(void) const;                                       //!< Get class
        bool                        isConvertibleTo(const std::string& type, bool once) const;      //!< Is convertible to type?
        void                        printValue(std::ostream& o) const;                          //!< Print value (for user)
        std::string                 richInfo(void) const;                                       //!< Complete info about object

        // Vector functions, including STL-like functions
        std::vector<double>         getValue(void) const;                                       //!< Get value
        void                        push_back(double x);                                        //!< Append element to end
        void                        push_front(double x);                                       //!< Add element in front
        void                        setValue(const VectorReal& x);                              //!< Set the value
        void                        setValue(const std::vector<double>& x);                     //!< Set the value
        void                               sort( void );                                           //!< sorts the vector    
        void                               unique(void);                                           //!< Removes consecutive duplicates and resizes the vector

    
    
};

#endif

