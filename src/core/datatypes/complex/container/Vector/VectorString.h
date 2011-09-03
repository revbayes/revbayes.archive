/**
 * @file
 * This file contains the declaration of VectorString, a complex type
 * used to hold string vectors.
 *
 * @brief Declaration of VectorString
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#ifndef VectorString_H
#define VectorString_H

#include "RbString.h"
#include "AbstractVector.h"

#include <iostream>
#include <string>
#include <vector>


const std::string VectorString_name = "String vector";

class VectorString : public AbstractVector {

public:
                                VectorString(void);                                             //!< Default constructor (empty vector)
                                VectorString(const std::string& x);                             //!< Construct vector with one string x
                                VectorString(const std::vector<std::string>& x);                //!< Constructor from string vector

    // Overloaded operators
    std::string&                operator[](size_t i);                                           //!< Index op allowing change
    const std::string&          operator[](size_t i) const;                                     //!< Const index op
    bool                        operator==(const VectorString& x) const;                        //!< Equality
    bool                        operator!=(const VectorString& x) const;                        //!< Inequality
    VectorString                operator+(const RbString& x) const;                             //!< Concatenate
    VectorString                operator+(const VectorString& x) const;                         //!< Concatenate
    VectorString                operator+(const std::string& x) const;                          //!< Concatenate

    // Basic utility functions
    VectorString*               clone(void) const;                                              //!< Clone object
    const VectorString&         getClass(void) const;                                           //!< Get class
    const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
    std::string                 richInfo(void) const;                                           //!< Complete info about object

    // Vector functions
    void                        clear(void);                                                    //!< Clear
    RbString*                   getElement(size_t index) const;                                 //!< Get element
    std::vector<std::string>    getValue(void) const;                                           //!< Get the value vector as a std::vector
    void                        pop_back(void);                                                 //!< Drop element at back
    void                        pop_front(void);                                                //!< Drop element from front
    void                        push_back(RbObject* x);                                         //!< Append element to end
    void                        push_back(const std::string &x);                                //!< Append element to end
    void                        push_front(RbObject* x);                                        //!< Add element in front
    void                        resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    void                        setElement(const size_t index, RbLanguageObject* elem);         //!< Set element with type conversion
    void                        setValue(const VectorString& x);                               //!< Set the value
    void                        setValue(const std::vector<std::string>& x);                    //!< Set the value
    void                        sort(void);                                                     //!< sort the AbstractVector
    size_t                      size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                        unique(void);                                                   //!< removes consecutive duplicates
    

    
private:
    std::vector<std::string>    elements;
    static const TypeSpec       typeSpec;
};

#endif

