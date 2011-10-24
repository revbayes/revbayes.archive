//
//  RbPtr.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 10/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "RbPtr.h"

#include "RbException.h"
#include "RbMemoryManager.h"

template <typename T>
RbPtr<T>::RbPtr(T* inPtr) {
    
    initializePointer(inPtr);
}

template <typename T>
RbPtr<T>::RbPtr(const RbPtr<T>& src) {
    
    initializePointer(src.mPtr);
}

template <typename T>
RbPtr<T>& RbPtr<T>::operator=(const RbPtr<T>& rhs) {
    
    if (this == &rhs) 
    {
        return (*this);
    }
    finalizePointer();
    initializePointer(rhs.mPtr);
    return (*this);
}


template <typename T>
void RbPtr<T>::initializePointer(T* inPtr) {
    
    mPtr = inPtr;
    RbMemoryManager& myMemoryManager = RbMemoryManager::rbMemoryManager();
    myMemoryManager.incrementCountForAddress(mPtr);
}

template <typename T>
void RbPtr<T>::finalizePointer(void) {
    
    RbMemoryManager& myMemoryManager = RbMemoryManager::rbMemoryManager();
    myMemoryManager.decrementCountForAddress(mPtr);
}

template <typename T>
const T* RbPtr<T>::operator->(void) const {
    
    return (mPtr);
}

template <typename T>
const T& RbPtr<T>::operator*(void) const {
    
    return (*mPtr);
}

template <typename T>
T* RbPtr<T>::operator->(void) {
    
    return (mPtr);
}

template <typename T>
T& RbPtr<T>::operator*(void) {
    
    return (*mPtr);
}
