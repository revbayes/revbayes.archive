/**
 * @file
 * This file contains the declaration of VectorRealPos,
 * a vector type used to hold positive real numbers.
 *
 * @brief Declaration of VectorRealPos
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

#ifndef VectorRealPos_H
#define VectorRealPos_H

#include "RealPos.h"
#include "VectorInteger.h"
#include "VectorReal.h"

#include <iostream>
#include <string>
#include <vector>


/**
 * This class holds a vector of real positive numbers in a convenient
 * way for use in the RevBayes source code.
 */
const std::string VectorRealPos_name = "+Real vector";

class VectorRealPos : public Vector {

    public:
                                    VectorRealPos(void);                                            //!< Default constructor (empty vector)
                                    VectorRealPos(double x);                                        //!< Construct vector with one double x
                                    VectorRealPos(size_t n, double x);                              //!< Construct vector with n doubles x
                                    VectorRealPos(const std::vector<double>& x);                    //!< Constructor from double vector
                                    VectorRealPos(const VectorReal& x);                             //!< Constructor from VectorReal

        // Overloaded operators
        double                      operator[](size_t i) const;                                     //!< Index op (value, no reference - no modification)
        bool                        operator==(const VectorRealPos& x) const;                       //!< Equals comparison
        bool                        operator!=(const VectorRealPos& x) const;                       //!< Not equals comparison
        
        // Basic utility functions, pure virtual in MemberObject and implemented here
        VectorRealPos*              clone(void) const;                                              //!< Clone object
        RbLanguageObject*           convertTo(const std::string& type) const;                       //!< Convert to type
        const VectorString&         getClass(void) const;                                           //!< Get class vector
        bool                        isConvertibleTo(const std::string& type, bool once) const;      //!< Is convertible to type?
        void                        printValue(std::ostream& o) const;                              //!< Print value (for user)
        std::string                 richInfo(void) const;                                           //!< Complete info about object

        // Vector functions, including STL-like functions
        std::vector<double>         getValue(void) const;                                           //!< Get value as STL double vector
        void                        push_back(double x);                                            //!< Append element to end
        void                        push_front(double x);                                           //!< Add element in front
        void                        setValue(const std::vector<double>& x);                         //!< Set the value using STL vector of int
        void                        setValue(const VectorInteger& x);                               //!< Set the value using VectorInteger
        void                        setValue(const VectorNatural& x);                               //!< Set the value using VectorNatural
        void                        setValue(const VectorReal& x);                                  //!< Set the value using VectorReal
        void                        setValue(const VectorRealPos& x);                               //!< Set the value using VectorRealPos

    protected:
//        RbObject*                   getDefaultElement(void) const { return new RealPos(); }          //!< Get default element for empty slots

};

#endif


