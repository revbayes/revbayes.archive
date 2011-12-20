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
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-08-19, version 1.0
 *
 * $Id$
 */

#ifndef RbPtr_H
#define RbPtr_H

//#include <map>
//#include <string>


template <typename T>
class RbPtr {
    
public:
    RbPtr(T* inPtr = 0);
    ~RbPtr(void);
    RbPtr(const RbPtr<T>& src);
    RbPtr<T>&                   operator=(const RbPtr<T>& rhs);
    T&                          operator*(void) const;
    T*                          operator->(void) const;
    operator                    T*(void) const { return mPtr; }
    
    static RbPtr<T>             getNullPtr(void)                                           //!< Get the null pointer
    {
        static RbPtr theNullPtr = RbPtr(0);
        return theNullPtr;
    }
    
protected:
    void                        finalizePointer(void);
    void                        initializePointer(T* inPtr);
    T*                          mPtr;
};

#include "RbMemoryManager.h"

#include <iostream>
#include <typeinfo>

template <typename T>
RbPtr<T>::RbPtr(T* inPtr) {
    
    initializePointer(inPtr);
//    if (RbMemoryManager::rbMemoryManager().countForAddress(inPtr) == 1) {
//        std::cerr << "Initialize pointer " << mPtr << " of type " << typeid(mPtr).name() << "|" << typeid(*mPtr).name() << std::endl;
//    }
}

template <typename T>
RbPtr<T>::RbPtr(const RbPtr<T>& src) {
    
    initializePointer(src.mPtr);
}

template <typename T>
RbPtr<T>::~RbPtr(void) {
    
    finalizePointer();
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
    
    if ( myMemoryManager.decrementCountForAddress(mPtr) ) { 
        delete mPtr;
//        std::cerr << "Destroying pointer " << mPtr << " of type " << typeid(mPtr).name() << std::endl;
    }
}

template <typename T>
T* RbPtr<T>::operator->(void) const {
    
    return (mPtr);
}

template <typename T>
T& RbPtr<T>::operator*(void) const {
    
    return (*mPtr);
}


#endif

