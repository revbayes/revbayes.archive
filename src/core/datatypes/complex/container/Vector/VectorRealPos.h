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

#include "AbstractVector.h"
#include "RealPos.h"
#include "VectorReal.h"

#include <iostream>
#include <string>
#include <vector>


/**
 * This class holds a vector of real positive numbers in a convenient
 * way for use in the RevBayes source code.
 */
const std::string VectorRealPos_name = "+Real vector";

class VectorRealPos : public AbstractVector {

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
    RbObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
    const VectorString&         getClass(void) const;                                           //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
    bool                        isConvertibleTo(const TypeSpec& type) const;                    //!< Is convertible to type?
    void                        printValue(std::ostream& o) const;                              //!< Print value (for user)

    // Vector functions, including STL-like functions
    void                        clear(void);                                                    //!< Clear
    const RbObject&             getElement(size_t index) const;                                 //!< Get element
    RbObject&                   getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    std::vector<double>         getValue(void) const;                                           //!< Get value as STL double vector
    void                        pop_back(void);                                                 //!< Drop element at back
    void                        pop_front(void);                                                //!< Drop element from front
    void                        push_back(RbObject* x);                                         //!< Append element to end
    void                        push_back(double x);                                            //!< Append element to end
    void                        push_front(RbObject* x);                                        //!< Add element in front
    void                        push_front(double x);                                           //!< Add element in front
    void                        resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                        setElement(const size_t index, RbLanguageObject* elem);         //!< Set element with type conversion
    void                        setValue(const std::vector<double>& x);                         //!< Set the value using STL vector of int
    void                        setValue(const VectorInteger& x);                               //!< Set the value using VectorInteger
    void                        setValue(const VectorNatural& x);                               //!< Set the value using VectorNatural
    void                        setValue(const VectorReal& x);                                  //!< Set the value using VectorReal
    void                        setValue(const VectorRealPos& x);                               //!< Set the value using VectorRealPos
    void                        sort(void);                                                     //!< sort the AbstractVector
    size_t                      size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                        unique(void);                                                   //!< removes consecutive duplicates
    
protected:
    std::vector<double>         elements;
    
private:
    static const TypeSpec       typeSpec;
};

#endif


