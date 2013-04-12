/**
 * @file AbstractRbObject.h
 *
 *
 * @brief Declaration of AbstractRbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-03-07 15:57:47 +0100 (Wed, 07 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-05-09, version 1.0
 *
 * $Id: RbPtr.h 1325 2012-03-07 14:57:47Z hoehna $
 */

#ifndef AbstractRbObject_H
#define AbstractRbObject_H

#include <string>

namespace RevBayesCore {
    
    template <typename objType>
    class AbstractRbObject {
        
    public:
        AbstractRbObject(objType* inPtr = NULL);
        ~AbstractRbObject(void);
        AbstractRbObject(const AbstractRbObject<objType>& src);
        AbstractRbObject&                           operator=(const AbstractRbObject<objType>& rhs);
        objType&                                    operator*(void) const;
        objType*                                    operator->(void) const;
        operator                                    objType*(void) const { return mPtr; }
        
    private:
        
        objType*                                    mPtr;
    };
    
}

#include <iostream>


template <typename objType>
RevBayesCore::AbstractRbObject<objType>::AbstractRbObject(objType* inPtr) : mPtr( inPtr ) {
    
}

template <typename ptrType>
RevBayesCore::AbstractRbObject<ptrType>::AbstractRbObject(const AbstractRbObject<ptrType>& src) {
    mPtr = src.mPtr->clone();
}

template <typename ptrType>
RevBayesCore::AbstractRbObject<ptrType>::~AbstractRbObject(void) {
    
    delete mPtr;
}

template <typename objType>
RevBayesCore::AbstractRbObject<objType>& RevBayesCore::AbstractRbObject<objType>::operator=(const AbstractRbObject<objType>& rhs) {
    
    if (this == &rhs) {
        return (*this);
    }
    mPtr = rhs.mPtr->clone();
    
    return (*this);
}


template <typename objType>
objType* RevBayesCore::AbstractRbObject<objType>::operator->(void) const {
    
    return (mPtr);
}

template <typename objType>
objType& RevBayesCore::AbstractRbObject<objType>::operator*(void) const {
    
    if ( mPtr == NULL ) {
        std::cerr << "Major BUG: Dereferencing NULL pointer!!!" << std::endl;
    }
    
    return (*mPtr);
}


#endif

