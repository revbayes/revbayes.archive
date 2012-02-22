/**
 * @file
 * This file contains the declaration of VectorNatural, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Declaration of VectorNatural
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef VectorNatural_H
#define VectorNatural_H

#include "Natural.h"
#include "AbstractVector.h"

#include <iostream>
#include <vector>


/**
 * Vector of natural numbers
 *
 * This class is not derived from VectorInteger for several reasons,
 * including the fact that doing so would make it impossible to
 * protect the subscript operator return.
 */

class VectorNatural : public AbstractVector {

    public:
                                VectorNatural(void);                                            //!< Default constructor (empty vector)
                                VectorNatural(const int x);                                     //!< Construct vector with one number x
                                VectorNatural(const unsigned int x);                            //!< Construct vector with one number x
                                VectorNatural(size_t n, int x);                                 //!< Construct vector with n numbers x
                                VectorNatural(size_t n, unsigned int x);                        //!< Construct vector with n numbers x
                                VectorNatural(const std::vector<int>& x);                       //!< Constructor from int vector
                                VectorNatural(const std::vector<unsigned int>& x);              //!< Constructor from unsigned int vector
                                VectorNatural(const VectorInteger& x);                          //!< Constructor from vector of Integer

    // Overloaded operators
    unsigned int                operator[](size_t i) const;                                     //!< Index op (value, no reference - no modification)
    bool                        operator==(const VectorNatural& x) const;                       //!< Equals comparison
    bool                        operator!=(const VectorNatural& x) const;                       //!< Not equals comparison
    
    // Basic utility functions
    VectorNatural*              clone(void) const;                                              //!< Clone object
    RbObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
    static const std::string&   getClassName(void);                                             //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
    bool                        isConvertibleTo(const TypeSpec& type) const;                    //!< Is convertible to type?

        // Vector functions, including STL-like functions
    void                        clear(void);                                                    //!< Clear
    std::vector<unsigned int>   getValue(void) const;                                           //!< Get value as STL unsigned int vector
    const RbObject&             getElement(size_t index) const;                                 //!< Get element
    RbObject&                   getElement(size_t index);                                       //!< Get element
    void                        pop_back(void);                                                 //!< Drop element at back
    void                        pop_front(void);                                                //!< Drop element from front
    void                        push_back(RbObject* x);                                         //!< Append element to end
    void                        push_back(unsigned int x);                                      //!< Append element to end
    void                        push_front(RbObject* x);                                        //!< Add element in front
    void                        push_front(unsigned int x);                                     //!< Add element in front
    void                        resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                        setElement(const size_t index, RbLanguageObject* elem);         //!< Set element with type conversion
    void                        setValue(const std::vector<int>& x);                            //!< Set the value using STL vector of int
    void                        setValue(const std::vector<unsigned int>& x);                   //!< Set the value using STL vector of unsigned int
    void                        setValue(const VectorNatural& x);                               //!< Set the value using VectorNatural
    void                        setValue(const VectorInteger& x);                               //!< Set the value using VectorInteger
    void                        sort(void);                                                     //!< sort the AbstractVector
    size_t                      size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                        unique(void);                                                   //!< removes consecutive duplicates

    
    
    private:
    std::vector<unsigned int>   elements;
    
};

#endif

