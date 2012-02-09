/**
 * @file
 * This file contains the declaration of VectorInteger, a complex type
 * used to hold int vectors.
 *
 * @brief Declaration of VectorInteger
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

#ifndef VectorInteger_H
#define VectorInteger_H

#include "Integer.h"
#include "AbstractVector.h"

#include <iostream>
#include <vector>


const std::string VectorInteger_name = "Integer vector";

class VectorInteger : public AbstractVector {

    public:
    // Constructors and destructor
                                VectorInteger(void);                                            //!< Default constructor (empty vector)
                                VectorInteger(int x);                                           //!< Construct vector with one int x
                                VectorInteger(size_t n, int x);                                 //!< Construct vector with n ints x
                                VectorInteger(const std::vector<int>& x);                       //!< Constructor from int vector
                                VectorInteger(const std::vector<size_t>& x);                    //!< Constructor from unsigned int vector
                                VectorInteger(const VectorNatural& x);                          //!< Constructor from vector of natural numbers

    // Overloaded operators
    int&                        operator[](size_t i);                                           //!< Index op allowing change
    const int&                  operator[](size_t i) const;                                     //!< Const index op
    bool                        operator==(const VectorInteger& x) const;                       //!< Equals comparison
    bool                        operator!=(const VectorInteger& x) const;                       //!< Not equals comparison
    
    // Basic utility functions
    VectorInteger*              clone(void) const;                                              //!< Clone object
    RbObject*                   convertTo(const TypeSpec& type) const;                          //!< Convert to type
    const VectorString&         getClass(void) const;                                           //!< Get class
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
    bool                        isConvertibleTo(const TypeSpec& type) const;                    //!< Is convertible to type?

        // Vector functions, including STL-like functions
    void                        clear(void);                                                    //!< Clear
    const RbObject&             getElement(size_t index) const;                                 //!< Get element
    RbObject&                   getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    std::vector<int>            getValue(void) const;                                           //!< Get value as STL int vector
    void                        pop_back(void);                                                 //!< Drop element at back
    void                        pop_front(void);                                                //!< Drop element from front
    void                        push_back(RbObject* x);                                         //!< Append element to end
    void                        push_back(int x);                                               //!< Append element to end
    void                        push_front(RbObject* x);                                        //!< Add element in front
    void                        push_front(int x);                                              //!< Add element in front
    void                        resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                        setElement(const size_t index, RbLanguageObject* elem);         //!< Set element with type conversion
    void                        setValue(const VectorInteger& x);                               //!< Set the value
    void                        setValue(const std::vector<int>& x);                            //!< Set the value
    void                        sort(void);                                                     //!< sort the AbstractVector
    size_t                      size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                        unique(void);                                                   //!< removes consecutive duplicates


    private:
    std::vector<int>            elements;
    static const TypeSpec       typeSpec;
    
    
};

#endif


