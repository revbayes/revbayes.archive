/**
 * @file RbPtr.h
 *
 * SuperSmartPointer:
 *
 * From Chapter 25 of Professional C++ 
 * by Nicholas A. Solter and Scott J. Kleper 
 * (Wrox, 2005) 
 *
 * @brief Declaration of ReferenceCount
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

#ifndef RbPtr_H
#define RbPtr_H

#include <string>

template <typename ptrType>
class RbPtr {
    
public:
    RbPtr(ptrType* inPtr = NULL);
    ~RbPtr(void);
    RbPtr(const RbPtr<ptrType>& src);
    RbPtr&                                      operator=(const RbPtr<ptrType>& rhs);
    ptrType&                                    operator*(void) const;
    ptrType*                                    operator->(void) const;
    operator                                    ptrType*(void) const { return mPtr; }
    static bool                                 decrementCountForAddress(ptrType* qPtr);
    static void                                 incrementCountForAddress(ptrType* qPtr);
    
private:
    size_t                                      countForAddress(const ptrType* qPtr);
    void                                        finalizePointer(void);
    void                                        initializePointer(ptrType* inPtr);
    
    ptrType*                                    mPtr;
};

#include <iostream>


template <typename ptrType>
RbPtr<ptrType>::RbPtr(ptrType* inPtr) {
    
    initializePointer(inPtr);
}

template <typename ptrType>
RbPtr<ptrType>::RbPtr(const RbPtr<ptrType>& src) {
    
    initializePointer(src.mPtr);
}

template <typename ptrType>
RbPtr<ptrType>::~RbPtr(void) {
    
    finalizePointer();
}

template <typename ptrType>
RbPtr<ptrType>& RbPtr<ptrType>::operator=(const RbPtr<ptrType>& rhs) {
    
    if (this == &rhs) {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


template <typename ptrType>
void RbPtr<ptrType>::initializePointer(ptrType* inPtr) {
    
    mPtr = inPtr;
    
    incrementCountForAddress(mPtr);
}

template <typename ptrType>
void RbPtr<ptrType>::finalizePointer(void) {
    
    if ( decrementCountForAddress(mPtr) ) { 
        delete mPtr;
    }
}

template <typename ptrType>
ptrType* RbPtr<ptrType>::operator->(void) const {
    
    return (mPtr);
}

template <typename ptrType>
ptrType& RbPtr<ptrType>::operator*(void) const {
    
    if ( mPtr == NULL ) {
        std::cerr << "Major BUG: Dereferencing NULL pointer!!!" << std::endl;
    }
    
    return (*mPtr);
}


template <typename ptrType>
size_t RbPtr<ptrType>::countForAddress(const ptrType* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return -1;
    
    return qPtr->getReferenceCount();
}


template <typename ptrType>
void RbPtr<ptrType>::incrementCountForAddress(ptrType* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return;
    
    qPtr->incrementReferenceCount();
}

template <typename ptrType>
bool RbPtr<ptrType>::decrementCountForAddress(ptrType* qPtr) {
    
    // check if we got the NULL pointer
    if (qPtr == 0) return false;
    
    size_t refCount = qPtr->decrementReferenceCount();
    
    return refCount == 0;
}


#endif

