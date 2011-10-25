/**
 * @file
 * This file contains the declaration of Vector, a container type
 * that acts as a base class for all constant vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id$
 */

#ifndef Vector_H
#define Vector_H

#include "AbstractVector.h"

#include <iostream>
#include <vector>


const std::string Vector_name = "Vector";

class Vector : public AbstractVector {

public:
    Vector();                                                                                       //!< Default constructor with type RbLanguageObject
    Vector(const TypeSpec &elemType);                                                               //!< Set type spec of container from type of elements
    Vector(const Vector &v);                                                                        //!< Copy Constructor

    virtual                                        ~Vector(void);                                                   //!< Virtual destructor 

    // Basic utility functions you have to override
    Vector*                                         clone(void) const;                                              //!< Clone object
    const VectorString&                             getClass(void) const;                                           //!< Get class
    virtual const TypeSpec&                         getTypeSpec(void) const;                                        //!< Get language type of the object
    void                                            printValue(std::ostream& o) const;                              //!< Print value for user
    std::string                                     richInfo(void) const;                                           //!< Complete info about object
    
    RbPtr<RbLanguageObject>                         operator[](size_t i);                                           //!< Index op allowing change
    const RbPtr<RbLanguageObject>                   operator[](size_t i) const;                                     //!< Const index op
    Vector&                                         operator=(const Vector& x);                                     //!< Assignment operator
    
    // Vector functions
    virtual void                                    clear(void);                                                    //!< Clear
    virtual RbPtr<RbObject>                         getElement(size_t index) const;                                 //!< Get element
    const std::vector<RbPtr<RbLanguageObject> >&    getValue(void) const;                                           //!< Get the stl vector of elements
    void                                            pop_back(void);                                                 //!< Drop element at back
    void                                            pop_front(void);                                                //!< Drop element from front
    virtual void                                    push_back(RbPtr<RbObject> x);                                   //!< Append element to end
    virtual void                                    push_front(RbPtr<RbObject> x);                                  //!< Add element in front
    virtual void                                    resize(size_t n);                                               //!< Resize to new AbstractVector of length n
    virtual void                                    setElement(const size_t index, RbPtr<RbLanguageObject> elem);   //!< Set element with type conversion
    void                                            sort(void);                                                     //!< sort the AbstractVector
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractVector
    void                                            unique(void);                                                   //!< removes consecutive duplicates
    
    protected:
    
    std::vector<RbPtr<RbLanguageObject> >           elements;
    TypeSpec                                        typeSpec;
};

#endif

