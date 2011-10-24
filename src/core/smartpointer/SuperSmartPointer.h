/**
 * @file SuperSmartPointer.h 
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
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef SuperSmartPointer_H
#define SuperSmartPointer_H

#include "ReferenceCount.h"

#include <iostream>
#include <stdexcept>

template <typename T>
class SuperSmartPointer
{
public:
	explicit SuperSmartPointer(T* inPtr);
	virtual ~SuperSmartPointer();
    
	SuperSmartPointer(const SuperSmartPointer<T>& src);
	SuperSmartPointer<T>& operator=(const SuperSmartPointer<T>& rhs);
    
	const T& operator*() const;
	const T* operator->() const;
	T& operator*();
	T* operator->();
    
    SuperSmartPointer<T> clone() const;
    
	operator void*() const { return mPtr; }
    
protected:
	T* mPtr;
    
	void finalizePointer();
	void initPointer(T* inPtr);
};

template <typename T>
SuperSmartPointer<T>::SuperSmartPointer(T* inPtr)
{
	initPointer(inPtr);
}

template <typename T>
SuperSmartPointer<T>::SuperSmartPointer(const SuperSmartPointer<T>& src)
{
	initPointer(src.mPtr);
}

template <typename T>
SuperSmartPointer<T>& 
SuperSmartPointer<T>::operator=(const SuperSmartPointer<T>& rhs)
{
	if (this == &rhs) {
		return *this;
	}
	finalizePointer();
	initPointer(rhs.mPtr);
    
	return *this;
}

template <typename T>
SuperSmartPointer<T>::~SuperSmartPointer()
{
	finalizePointer();
}

template<typename T>
SuperSmartPointer<T> SuperSmartPointer<T>::clone() const {
    return SuperSmartPointer(mPtr->clone());
}

template<typename T>
void SuperSmartPointer<T>::initPointer(T* inPtr) {
	mPtr = inPtr;
	
    ReferenceCount::getInstance().increaseReferenceCount(inPtr);
}

template<typename T>
void SuperSmartPointer<T>::finalizePointer() {
    
    ReferenceCount::getInstance().decreaseReferenceCount(mPtr);
	
	if (ReferenceCount::getInstance().getNumberReferences(mPtr) == 0) {
		// no No more references to this object -- delete it and remove from map
		ReferenceCount::getInstance().erase(mPtr);
		delete mPtr;
		mPtr = NULL;
	}
}

template <typename T>
const T* SuperSmartPointer<T>::operator->() const
{
	return mPtr;
}

template <typename T>
const T& SuperSmartPointer<T>::operator*() const
{
	return *mPtr;
}

template <typename T>
T* SuperSmartPointer<T>::operator->() 
{
	return mPtr;
}

template <typename T>
T& SuperSmartPointer<T>::operator*() 
{
	return *mPtr;
}

#endif

